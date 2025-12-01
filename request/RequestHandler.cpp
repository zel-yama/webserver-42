#include "RequestHandler.hpp"
#include <algorithm>
#include <iostream>

RequestHandler::RequestHandler() {}

Request RequestHandler::handleRequest(int fd, const std::string& rawData, const ServerConfig& config) {
    Request req = parser.parse(fd, rawData);
    

    if (!req.complete || req.status != 200) {
        return req;
    }

    Location* matchedLocation = findMatchingLocation(req.path, config.locations);

    if (!matchedLocation) {
        req.status = 404;
        return req;
    }
    
    checkRedirection(req, matchedLocation);
    if (req.status == 301) {
        return req;
    }
    
    validateMethod(req, matchedLocation);
    if (req.status == 405) {
        return req;
    }
    
    validateBodySize(req, config.max_body_size);
    if (req.status == 413) {
        return req;
    }

    // if (req.method == "POST") {
    //     if (!req.hasHeader("content-length")) {
    //         std::cerr << "NOT FOUND - returning 400" << std::endl;
    //         req.status = 400;
    //         req.complete = true;
    //         return req;
    //     }
    //     std::cerr << "FOUND" << std::endl;
    // }

    return req;
}

Location* RequestHandler::findMatchingLocation(const std::string& uri, const std::vector<Location>& locations) {
    Location* bestMatch = NULL;
    size_t longestMatch = 0;
    
    for (size_t i = 0; i < locations.size(); i++) {
        const Location& loc = locations[i];
        
        if (uri.find(loc.path) == 0) {
            size_t locLen = loc.path.length();
        
            bool isValidMatch = false;
            
            if (uri.length() == locLen) {
                isValidMatch = true;
            } else if (uri[locLen] == '/') {
                isValidMatch = true;
            } else if (locLen > 1 && loc.path[locLen - 1] == '/') {
                isValidMatch = true;
            }
            
            if (isValidMatch && locLen > longestMatch) {
                bestMatch = const_cast<Location*>(&loc);
                longestMatch = locLen;
            }
        }
    }
    
    return bestMatch;
}

bool RequestHandler::isMethodAllowed(const std::string& method, 
                                     const std::vector<std::string>& allowedMethods) {
    for (size_t i = 0; i < allowedMethods.size(); i++) {
        if (allowedMethods[i] == method) {
            return true;
        }
    }
    return false;
}

void RequestHandler::validateMethod(Request& req, const Location* location) {
    if (!isMethodAllowed(req.method, location->allowed_methods)) {
        req.status = 405;
    }
}

void RequestHandler::validateBodySize(Request& req, size_t maxBodySize) {
    if ((req.method == "POST" || req.method == "PUT") && req.body.size() > maxBodySize) {
        req.status = 413; 
        req.complete = true;
    }
}

void RequestHandler::checkRedirection(Request& req, const Location* location) {
    if (!location->redirect.empty()) {
        req.status = 301;
        // req.redirectUrl = location->redirect;
    }
}