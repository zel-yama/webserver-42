#include "RequestParser.hpp"
#include "../server/include/Server.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <stdexcept>

std::string RequestParser::extractBoundary(std::string& contentType) {
    std::string lower = toLower(contentType);
    size_t pos = lower.find("boundary=");
    
    if (pos == std::string::npos)
        return "";
    
    pos += 9;

    if (pos < contentType.size() && contentType[pos] == '"') {
        pos++;
        size_t endQuote = contentType.find('"', pos);
        if (endQuote != std::string::npos)
            return contentType.substr(pos, endQuote - pos);
    }

    size_t end = contentType.find_first_of("; \t\r\n", pos);
    if (end == std::string::npos)
        return contentType.substr(pos);
    
    return contentType.substr(pos, end - pos);
}

bool RequestParser::parseMultipart(std::string& body, std::string& boundary, Request& req) {

    std::string delimiter = "--" + boundary;
    std::string endDelimiter = delimiter + "--";

    size_t pos = 0;

    while (true) {
        size_t start = body.find(delimiter, pos);
        if (start == std::string::npos) break;

        start += delimiter.size();

        // End boundary
        if (body.compare(start, 2, "--") == 0)
            break;

        // skip CRLF
        if (body.compare(start, 2, "\r\n") == 0) start += 2;
        else if (body[start] == '\n') start += 1;

        size_t next = body.find(delimiter, start);
        if (next == std::string::npos) break;

        std::string part = body.substr(start, next - start);

        size_t headersEnd = part.find("\r\n\r\n");
        size_t sepLen = 4;
        if (headersEnd == std::string::npos) {
            headersEnd = part.find("\n\n");
            sepLen = 2;
        }

        if (headersEnd == std::string::npos) {
            pos = next;
            continue;
        }

        std::string headersSection = part.substr(0, headersEnd);
        std::string content = part.substr(headersEnd + sepLen);

        // trim end CRLF
        while (!content.empty() && (content.back() == '\n' || content.back() == '\r'))
            content.pop_back();

        MultipartPart mp;
        std::istringstream hs(headersSection);
        std::string line;

        while (std::getline(hs, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            std::string lower = toLower(line);

            if (lower.find("content-disposition:") == 0) {
                size_t n = line.find("name=\"");
                if (n != std::string::npos) {
                    n += 6;
                    size_t e = line.find('"', n);
                    mp.name = line.substr(n, e - n);
                }

                size_t f = line.find("filename=\"");
                if (f != std::string::npos) {
                    f += 10;
                    size_t e = line.find('"', f);
                    mp.filename = line.substr(f, e - f);
                }
            }
            else if (lower.find("content-type:") == 0) {
                mp.contentType = trim(line.substr(line.find(':')+1));
            }
        }

        mp.content = content;
        req.multipartData.push_back(mp);

        pos = next;
    }

    return true;
}
