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
    if (boundary.empty()) {
        std::cerr << "Error: Empty boundary in multipart" << std::endl;
        return false;
    }
    
    std::string delimiter = "--" + boundary;
    std::string endDelimiter = "--" + boundary + "--";
    
    size_t pos = 0;

    pos = body.find(delimiter);
    if (pos == std::string::npos) {
        std::cerr << "Error: No boundary found in multipart body" << std::endl;
        return false;
    }
    
    pos += delimiter.length();
    
    while (pos < body.size()) {
        if (pos + 1 < body.size() && body[pos] == '\r' && body[pos + 1] == '\n')
            pos += 2;
        else if (pos < body.size() && body[pos] == '\n')
            pos++;

        if (body.compare(pos - delimiter.length(), endDelimiter.length(), endDelimiter) == 0)
            break;
        
        size_t nextBoundary = body.find(delimiter, pos);
        if (nextBoundary == std::string::npos)
            break;

        std::string part = body.substr(pos, nextBoundary - pos);

        size_t headersEnd = part.find("\r\n\r\n");
        if (headersEnd == std::string::npos)
            headersEnd = part.find("\n\n");
        
        if (headersEnd != std::string::npos) {
            std::string headersSection = part.substr(0, headersEnd);

            size_t contentStart = headersEnd + 4;
            if (part.substr(headersEnd, 2) == "\n\n")
                contentStart = headersEnd + 2;

            std::string content = part.substr(contentStart);

            while (!content.empty() && (content[content.size() - 1] == '\n' || content[content.size() - 1] == '\r'))
                content.erase(content.size() - 1);

            std::istringstream hs(headersSection);
            std::string line;
            std::string name, filename, contentType;
            
            while (std::getline(hs, line)) {
                if (!line.empty() && line[line.size() - 1] == '\r')
                    line.erase(line.size() - 1);
                
                if (line.empty())
                    break;

                std::string lowerLine = toLower(line);
                if (lowerLine.find("content-disposition:") == 0) {
                    size_t namePos = line.find("name=\"");
                    if (namePos != std::string::npos) {
                        namePos += 6;
                        size_t nameEnd = line.find('"', namePos);
                        if (nameEnd != std::string::npos)
                            name = line.substr(namePos, nameEnd - namePos);
                    }

                    size_t filePos = line.find("filename=\"");
                    if (filePos != std::string::npos) {
                        filePos += 10;
                        size_t fileEnd = line.find('"', filePos);
                        if (fileEnd != std::string::npos)
                            filename = line.substr(filePos, fileEnd - filePos);
                    }
                } else if (lowerLine.find("content-type:") == 0) {
                    size_t typeStart = line.find(':') + 1;
                    contentType = trim(line.substr(typeStart));
                }
            }

            MultipartPart mpPart;
            mpPart.name = name;
            mpPart.filename = filename;
            mpPart.contentType = contentType;
            mpPart.content = content;
            
            req.multipartData.push_back(mpPart);
            
            std::cout << "  Multipart field: " << name;
            if (!filename.empty())
                std::cout << " (file: " << filename << ", " << content.size() << " bytes)";
            else
                std::cout << " (" << content.size() << " bytes)";
            std::cout << std::endl;
        }
        
        pos = nextBoundary + delimiter.length();
    }
    
    return true;
}
