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


Server* getServerForClient(maptype& config, int serverId) {
    if (config.find(serverId) != config.end()) {
        Config* cfg = config[serverId];
        if (cfg->name == "Server") {
            return dynamic_cast<Server*>(cfg);
        }
    }
    return NULL;
}

location* findLocation(Server* srv, const std::string& path) {
    location* bestMatch = NULL;
    size_t longestMatch = 0;

    for (size_t i = 0; i < srv->objLocation.size(); ++i) {
        location& loc = srv->objLocation[i];
        size_t locLen = loc.locationPath.length();

        if (loc.locationPath == "/") {                                                                                                                                                                                  
            if (locLen > longestMatch) {
                bestMatch = &loc;
                longestMatch = locLen;
            }
            continue;
        }

        if (path.compare(0, locLen, loc.locationPath) == 0 &&
            (path.length() == locLen || path[locLen] == '/')) {
            if (locLen > longestMatch) {
                bestMatch = &loc;
                longestMatch = locLen;
            }
        }
    }

    return bestMatch;
}

std::string joinPathWithLocation(Server* srv, location *loc, const std::string& reqPath) {
    std::string root;
    if (loc->root.empty())
        root = srv->root;
    else
        root = loc->root;
    std::string suffix;

    // kanms7 loction mn path 
    if (loc->locationPath != "/") {
        if (reqPath.compare(0, loc->locationPath.length(), loc->locationPath) == 0)
            suffix = reqPath.substr(loc->locationPath.length());
    } else {
        suffix = reqPath;
    }

    // cheak ila kan badi / ola khawi
    if (!suffix.empty() && suffix[0] != '/')
        suffix = "/" + suffix;

    // Join path m3a root
    std::string fullpath = root;
    if (!fullpath.empty() && fullpath[fullpath.size() - 1] == '/' && !suffix.empty() && suffix[0] == '/')
        fullpath.erase(fullpath.size() - 1); // remove extra '/' ila kant
    fullpath += suffix;

    return fullpath;
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
    
    if (!req.loc) {
        req.status = 404;
        return;
    }
    req.fullpath = joinPathWithLocation(srv, req.loc, req.path);

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

