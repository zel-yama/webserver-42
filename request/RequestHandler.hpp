#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "Request.hpp"
#include "RequestParser.hpp"

class RequestHandler {
    private:
        RequestParser parser;
        
        Location* findMatchingLocation(const std::string& uri, const std::vector<Location>& locations);
        bool isMethodAllowed(const std::string& method, const std::vector<std::string>& allowedMethods);
        void validateMethod(Request& req, const Location* location);
        void checkRedirection(Request& req, const Location* location);
        void validateBodySize(Request& req, size_t maxBodySize);
        
    public:
        RequestHandler();
        Request handleRequest(int fd, const std::string& rawData, const ServerConfig& config);
};

#endif