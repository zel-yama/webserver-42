// #include "../server/include/Server.hpp"
#include "../server/include/Client.hpp"
// #include "RequestParser.hpp"

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

    if (loc->locationPath != "/") {
        if (reqPath.compare(0, loc->locationPath.length(), loc->locationPath) == 0)
            suffix = reqPath.substr(loc->locationPath.length());
    } else {
        suffix = reqPath;
    }

    if (!suffix.empty() && suffix[0] != '/')
        suffix = "/" + suffix;

    std::string fullpath = root;
    if (!fullpath.empty() && fullpath[fullpath.size() - 1] == '/' && !suffix.empty() && suffix[0] == '/')
        fullpath.erase(fullpath.size() - 1);
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

    if (req.status != 200)
        return;

    location *loc;
    loc = findLocation(srv, req.path); //fix if there is not location in config ?
    req.loc = *loc;
    
    if (!loc) {
        req.status = 404;
        return;
    }
    req.fullpath = joinPathWithLocation(srv, loc, req.path);

    if (loc && !loc->returnP.empty()) {
        int code = (loc->returnCode > 0) ? loc->returnCode : 301;
        req.status = code;
        req.headers["location"] = loc->returnP;
        return;
    }

    if (!srv->returnP.empty()) {
        int code = (srv->returnCode > 0) ? srv->returnCode : 301;
        req.status = code;
        req.headers["location"] = srv->returnP;
        return;
    }
    
    if (!isMethodAllowed(req.method, loc->allowedMethods)) {
        req.status = 405;
        return;
    }

    if ((req.method == "POST" || req.method == "DELETE")) {
        size_t maxSize = (loc->bodyMaxByte > 0) ? loc->bodyMaxByte : srv->bodyMaxByte;
        
        if (req.body.size() > maxSize) {
            req.status = 413;
            return;
        }
    }
}

