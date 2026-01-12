#include "../server/include/Server.hpp"
#include "RequestParser.hpp"

Server* getServerFromClient(maptype& config, Client& client) {
    if (config.find(client.serverId) != config.end()) {
        Config* cfg = config[client.serverId];
        if (cfg->name == "Server") {
            return dynamic_cast<Server*>(cfg);
        }
    }
    return NULL;
}


location* findLocation(Server* srv, const std::string& path) {
    location* bestMatch = NULL;
    size_t longestMatch = 0;
    
    for (size_t i = 0; i < srv->objLocation.size(); i++) {
        location& loc = srv->objLocation[i];
        
        // Check if path starts with location path
        if (path.find(loc.locationPath) == 0) {
            size_t locLen = loc.locationPath.length();
            bool isValidMatch = false;
            
            // Exact match
            if (path.length() == locLen) {
                isValidMatch = true;
            }
            // Path continues with /
            else if (path[locLen] == '/') {
                isValidMatch = true;
            }
            // Location ends with /
            else if (locLen > 1 && loc.locationPath[locLen - 1] == '/') {
                isValidMatch = true;
            }
            
            if (isValidMatch && locLen > longestMatch) {
                bestMatch = &loc;
                longestMatch = locLen;
            }
        }
    }
    
    return bestMatch;
}

// Check if method is allowed in location
bool isMethodAllowed(const std::string& method, const std::vector<std::string>& allowed) {
    for (size_t i = 0; i < allowed.size(); i++) {
        if (allowed[i] == method) {
            return true;
        }
    }
    return false;
}

// Validate request against server configuration
void validateRequest(Request& req, Server* srv) {
    // Find matching location
    location* loc = findLocation(srv, req.path);
    
    if (!loc) {
        req.status = 404;
        return;
    }
    
    // Check for redirection
    if (!loc->returnP.empty()) {
        req.status = 301;
        req.headers["location"] = loc->returnP;
        return;
    }
    
    // Check if method is allowed
    if (!isMethodAllowed(req.method, loc->allowedMethods)) {
        req.status = 405;
        return;
    }
    
    // Check body size for POST/DELETE
    if ((req.method == "POST" || req.method == "DELETE")) {
        size_t maxSize = (loc->bodyMaxByte > 0) ? loc->bodyMaxByte : srv->bodyMaxByte;
        
        if (req.body.size() > maxSize) {
            req.status = 413;
            return;
        }
    }
}

Server* getServerForClient(maptype& config, int serverId) {
    if (config.find(serverId) != config.end()) {
        Config* cfg = config[serverId];
        if (cfg->name == "Server") {
            return dynamic_cast<Server*>(cfg);
        }
    }
    return NULL;
}