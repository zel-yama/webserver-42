#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "Request.hpp"
#include "RequestParser.hpp"

class RequestHandler {
    private:
        RequestParser parser;
        
        Location* findMatchingLocation(std::string& uri, std::vector<Location>& locations);
        bool isMethodAllowed(std::string& method, std::vector<std::string>& allowedMethods);
        void validateMethod(Request& req, Location* location);
        void checkRedirection(Request& req, Location* location);
        void validateBodySize(Request& req, size_t maxBodySize);
        
    public:
        RequestHandler();
        Request handleRequest(int fd, std::string& rawData, ServerConfig& config);
};

#endif