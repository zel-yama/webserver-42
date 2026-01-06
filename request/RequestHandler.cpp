#include "RequestHandler.hpp"
#include <algorithm>
#include <iostream>

RequestHandler::RequestHandler() {}

Request RequestHandler::handleRequest(
    int fd, std::string& raw, ServerConfig& config)
{
    Request req;

    try {
        req = parser.parse(fd, raw);
    } catch (...) {
        req.status = 400;
        req.complete = true;
        return req;
    }

    if (!req.complete)
        return req;

    Location* loc = findMatchingLocation(req.path, config.locations);
    if (!loc) {
        req.status = 404;
        return req;
    }

    checkRedirection(req, loc);
    if (req.status == 301)
        return req;

    validateMethod(req, loc);
    if (req.status == 405)
        return req;

    validateBodySize(req, config.max_body_size);
    return req;
}


Location* RequestHandler::findMatchingLocation(std::string& uri, std::vector<Location>& locations) {
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

bool RequestHandler::isMethodAllowed(std::string& method, std::vector<std::string>& allowedMethods) {
    for (size_t i = 0; i < allowedMethods.size(); i++) {
        if (allowedMethods[i] == method) {
            return true;
        }
    }
    return false;
}

void RequestHandler::validateMethod(Request& req, Location* location) {
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

void RequestHandler::checkRedirection(Request& req, Location* location) {
    if (!location->redirect.empty()) {
        req.status = 301;
        // req.redirectUrl = location->redirect;
    }
}