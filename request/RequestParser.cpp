#include "RequestParser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <iostream>

std::string RequestParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");

    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string RequestParser::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool RequestParser::isValidMethod(const std::string& method) {
    return method == "GET" || method == "POST" || method == "DELETE";
}

bool RequestParser::isValidVersion(const std::string& version) {
    return version == "HTTP/1.1" || version == "HTTP/1.0";
}

bool RequestParser::isValidUriChar(char c) {
    if (std::isalnum(c))
        return true;
    
    std::string Special = "-._~:/?#[]@!$&'()*+,;=%";
    return Special.find(c) != std::string::npos;
}

bool RequestParser::isValidUri(const std::string& uri) {
    if (uri.length() > 2048)
        return false;
    
    for (size_t i = 0; i < uri.length(); i++) {
        if (!isValidUriChar(uri[i]))
            return false;
    }
    
    return true;
}

Request RequestParser::parse(int fd, const std::string &data) {
    Request req;
    req.status = 200; 
    
    buffer[fd] += data;
    std::string &b = buffer[fd];

    size_t pos = b.find("\r\n\r\n");
    if (pos == std::string::npos)
        return req;

    req.headerComplete = true;
    std::string headerSection = b.substr(0, pos);
    
    std::vector<std::string> lines;
    size_t lineStart = 0;
    size_t lineEnd = 0;
    
    while (lineStart < headerSection.size()) {
        lineEnd = headerSection.find("\r\n", lineStart);
        if (lineEnd == std::string::npos) {
            lines.push_back(headerSection.substr(lineStart));
            break;
        }
        lines.push_back(headerSection.substr(lineStart, lineEnd - lineStart));
        lineStart = lineEnd + 2;
    }

    if (lines.empty()) {
        req.status = 400;
        req.complete = true;
        return req;
    }

    std::stringstream ss(lines[0]);
    ss >> req.method >> req.path >> req.version;

    if (req.method.empty() || req.path.empty() || req.version.empty()) {
        req.status = 400;
        req.complete = true;
        return req;
    }

    if (!isValidMethod(req.method)) {
        req.status = 501; 
        req.complete = true;
        return req;
    }

    if (!isValidVersion(req.version)) {
        req.status = 505;
        req.complete = true;
        return req;
    }

    if (!isValidUri(req.path)) {
        if (req.path.length() > 2048)
            req.status = 414;
        else
            req.status = 400;
        req.complete = true;
        return req;
    }

    for (size_t i = 1; i < lines.size(); i++) {
        size_t colon = lines[i].find(":");
        if (colon != std::string::npos) {
            std::string key = trim(lines[i].substr(0, colon));
            std::string value = trim(lines[i].substr(colon + 1));
            req.headers[toLower(key)] = value;
        }
    }

    if (req.hasHeader("transfer-encoding")) {
        req.status = 501;
        req.complete = true;
        return req;
    }

    if (req.method == "POST") {
        if (!req.hasHeader("content-length")) {
            req.status = 400;
            req.complete = true;
            return req;
        }
    }

    size_t contentLen = 0;
    if (req.headers.count("content-length")) {
        contentLen = atoi(req.headers["content-length"].c_str());
    }
    
    if (b.size() >= pos + 4 + contentLen) {
        if (contentLen > 0) {
            req.body = b.substr(pos + 4, contentLen);
        }
        req.complete = true;
        buffer.erase(fd);
    }

    return req;
}