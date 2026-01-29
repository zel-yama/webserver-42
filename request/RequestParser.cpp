#include "RequestParser.hpp"
#include "../server/include/Server.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stdexcept>


Request::Request() : complete(false), status(200), keepalive(false), loc(NULL) {
    // loc = new location();
}

Request::~Request() {
    // delete loc;
}

std::string RequestParser::trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos)
        return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

std::string RequestParser::toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool RequestParser::isValidMethod(const std::string& m) {
    return m == "GET" || m == "POST" || m == "DELETE";
}

bool RequestParser::isValidVersion(const std::string& v) {
    return v == "HTTP/1.0" || v == "HTTP/1.1";
}

bool RequestParser::isValidUriChar(char c) {
    if (std::isalnum(c))
        return true;
    std::string s = "-._~:/?#[]@!$&'()*+,;=%";
    return s.find(c) != std::string::npos;
}

bool RequestParser::isValidUri(const std::string& uri) {
    if (uri.empty() || uri[0] != '/' || uri.size() > 2048)
        return false;

    for (size_t i = 0; i < uri.size(); i++) {
        if (uri[i] == '%') {
            if (i + 2 >= uri.size() ||
                !isxdigit(uri[i + 1]) ||
                !isxdigit(uri[i + 2]))
                return false;
            i += 2;
        } else if (!isValidUriChar(uri[i])) {
            return false;
        }
    }
    return true;
}

std::string RequestParser::normalizePath(const std::string& path) {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string item;

    while (std::getline(ss, item, '/')) {
        if (item.empty() || item == ".")
            continue;
        if (item == "..") {
            if (!parts.empty())
                parts.pop_back();
        } else {
            parts.push_back(item);
        }
    }

    std::string r = "/";
    for (size_t i = 0; i < parts.size(); i++) {
        r += parts[i];
        if (i + 1 < parts.size())
            r += "/";
    }
    return r;
}

size_t RequestParser::parseContentLength(const std::string& v) {
    size_t n = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (!isdigit(v[i]))
            throw std::runtime_error("bad content-length");
        n = n * 10 + (v[i] - '0'); //overflow ?
    }
    return n;
}

bool RequestParser::decodeChunked(std::string& buf, std::string& out) {
    while (true) {
        size_t p = buf.find("\r\n");
        if (p == std::string::npos)
            return false;

        size_t size = strtoul(buf.substr(0, p).c_str(), NULL, 16);
        if (buf.size() < p + 2 + size + 2)
            return false;

        if (size == 0) {
            buf.erase(0, p + 4);
            return true;
        }

        out.append(buf, p + 2, size);
        buf.erase(0, p + 2 + size + 2);
    }
}

void RequestParser::clearBuffer(int fd) {
    buffer.erase(fd);
}


std::string normalize(std::string &data) {
    std::string nor;
    nor.reserve(data.size() + data.size() / 10); // Reserve extra space
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i] == '\r') {
            nor += "\r\n";
            // Skip the next character if it's \n (CRLF -> CRLF)
            if (i + 1 < data.size() && data[i + 1] == '\n')
                i++;
        } else if (data[i] == '\n') {
            // Bare LF -> CRLF
            nor += "\r\n";
        } else {
            nor += data[i];
        }
    }
    return nor;
}

Request RequestParser::parse(int fd, std::string& data)
{
    Request req;
    //     const size_t MAX_BUFFER_SIZE = 10485760; // 10MB
    // if (buffer[fd].size() + data.size() > MAX_BUFFER_SIZE) {
    //     buffer.erase(fd);
    //     throw std::runtime_error("request too large");
    // }
    buffer[fd] += normalize(data);
    std::string& b = buffer[fd];

    size_t headerEnd = b.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        return req;

    std::istringstream hs(b.substr(0, headerEnd));
    std::string line;

    if (!std::getline(hs, line))
        throw std::runtime_error("empty request");
    
    // Remove trailing \r if present
    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    std::istringstream rl(line);
    rl >> req.method >> req.path >> req.version;

    if (req.method.empty() || req.path.empty() || req.version.empty())
        throw std::runtime_error("incomplete request line");

    if (!isValidMethod(req.method) || !isValidVersion(req.version))
        throw std::runtime_error("bad request line");

    size_t q = req.path.find('?');
    if (q != std::string::npos) {
        req.query = req.path.substr(q + 1);
        req.path = req.path.substr(0, q);
    }

    if (!isValidUri(req.path))
        throw std::runtime_error("bad uri");

    req.path = normalizePath(req.path);

    while (std::getline(hs, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty())
            break;

        size_t c = line.find(':');

        if (c == std::string::npos) {
            throw std::runtime_error("400: malformed header (missing colon)");
        }

        std::string headerName = trim(line.substr(0, c));
        std::string headerValue = trim(line.substr(c + 1));

        if (headerName.empty()) {
            throw std::runtime_error("400: empty header name");
        }

        headerName = toLower(headerName);

        if (headerName == "content-length" &&
            req.headers.count("content-length")) {
            throw std::runtime_error("400: duplicate content-length");
        }

        req.headers[headerName] = headerValue;
    }


    std::string conn;
    if (req.headers.count("connection"))
        conn = toLower(req.headers["connection"]);

    if (req.version == "HTTP/1.1") {
        req.keepalive = (conn != "close");
    }
    else {
        req.keepalive = (conn == "keep-alive");
    }


    b.erase(0, headerEnd + 4);


    if (req.headers["transfer-encoding"] == "chunked") {
        if (!decodeChunked(b, req.body))
            return req;
    }
    else if (req.headers.count("content-length")) {
        size_t len = parseContentLength(req.headers["content-length"]);
        if (b.size() < len)
            return req;
        req.body = b.substr(0, len);
        b.erase(0, len);
    }

    req.complete = true;

    if (req.headers.count("content-type")) {
        std::string contentType = req.headers["content-type"];
        std::string lowerCT = toLower(contentType);
        
        if (lowerCT.find("multipart/form-data") != std::string::npos) {
            std::string boundary = extractBoundary(contentType);
            
            if (!boundary.empty()) {
                std::cout << "Parsing multipart with boundary: " << boundary << std::endl;
                
                if (!parseMultipart(req.body, boundary, req)) {
                    std::cerr << "Warning: Failed to parse multipart data" << std::endl;
                }
            } else {
                std::cerr << "Warning: multipart/form-data without boundary" << std::endl;
            }
        }
    }
    // if (!req.keepalive)
    //     buffer.erase(fd);

    return req;
}