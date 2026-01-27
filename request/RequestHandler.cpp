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
        size_t locLen = loc.locationPath.length();

        if (loc.locationPath == "/") {                                                                                                                                                                                  
            if (locLen > longestMatch) {
                bestMatch = &loc;
                longestMatch = locLen;
            }
            continue;
        }

        if (path.compare(0, locLen, loc.locationPath) == 0) {
            if (path.length() == locLen || path[locLen] == '/') {
                if (locLen > longestMatch) {
                    bestMatch = &loc;
                    longestMatch = locLen;
                }
            }
            continue;
        }

        if (path.compare(0, locLen, loc.locationPath) == 0) {
            if (path.length() == locLen || path[locLen] == '/') {
                if (locLen > longestMatch) {
                    bestMatch = &loc;
                    longestMatch = locLen;
                }
            }
        }
    }
    return bestMatch;
}

bool isMethodAllowed(const std::string& method, const std::vector<std::string>& allowed) {
    for (size_t i = 0; i < allowed.size(); i++) {
        if (allowed[i] == method) {
            return true;
        }
    }
    return false;
}

void validateRequest(Request& req, Server* srv) {

    req.loc = findLocation(srv, req.path);
    
    
    if (!req.loc || (req.loc->root.empty() && srv->root.empty())) {
        req.status = 404;
        return;
    }
    std::string root;
    if (req.loc->root.empty()) root = srv->root;
    else root = req.loc->root;
    req.fullpath = root + req.path;


    if (!req.loc->returnP.empty()) {
        req.status = 301;
        req.headers["location"] = req.loc->returnP;
        return;
    }
    
    if (!isMethodAllowed(req.method, req.loc->allowedMethods)) {
        req.status = 405;
        return;
    }

    if ((req.method == "POST" || req.method == "DELETE")) {
        size_t maxSize = (req.loc->bodyMaxByte > 0) ? req.loc->bodyMaxByte : srv->bodyMaxByte;
        
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