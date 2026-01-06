#include "RequestParser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stdexcept>

static const size_t MAX_HEADER_SIZE = 8192;

/* ================= utils ================= */

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

/* accept 1.1 input, behave like 1.0 */
bool RequestParser::isValidVersion(const std::string& v) {
    return v == "HTTP/1.0" || v == "HTTP/1.1";
}

/* ================= URI ================= */

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

/* ================= body ================= */

size_t RequestParser::parseContentLength(const std::string& v) {
    size_t n = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (!isdigit(v[i]))
            throw std::runtime_error("bad content-length");
        n = n * 10 + (v[i] - '0');
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

/* ================= PARSER ================= */

Request RequestParser::parse(int fd, const std::string& data)
{
    Request req;
    buffer[fd] += data;
    std::string& b = buffer[fd];

    size_t headerEnd = b.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        return req;

    if (headerEnd > MAX_HEADER_SIZE)
        throw std::runtime_error("headers too large");

    std::istringstream hs(b.substr(0, headerEnd));
    std::string line;

    /* request line */
    std::getline(hs, line);
    std::istringstream rl(line);
    rl >> req.method >> req.path >> req.version;

    if (!isValidMethod(req.method) || !isValidVersion(req.version))
        throw std::runtime_error("bad request line");

    /* query */
    size_t q = req.path.find('?');
    if (q != std::string::npos) {
        req.query = req.path.substr(q + 1);
        req.path = req.path.substr(0, q);
    }

    if (!isValidUri(req.path))
        throw std::runtime_error("bad uri");

    req.path = normalizePath(req.path);

    /* headers */
    while (std::getline(hs, line)) {
        if (line == "\r")
            break;
        size_t c = line.find(':');
        if (c == std::string::npos)
            throw std::runtime_error("bad header");

        req.headers[toLower(trim(line.substr(0, c)))] =
            trim(line.substr(c + 1));
    }

    /* ---- CONNECTION DECISION (CRITICAL) ---- */
    req.should_close = false;
    std::string conn = toLower(req.headers["connection"]);

    if (req.version == "HTTP/1.0") {
        if (conn != "keep-alive")
            req.should_close = true;
    } else { // HTTP/1.1
        if (conn == "close")
            req.should_close = true;
    }

    b.erase(0, headerEnd + 4);

    /* body */
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

    /* ---- CLEAN BUFFER IF CONNECTION CLOSES ---- */
    if (req.should_close)
        buffer.erase(fd);

    return req;
}

