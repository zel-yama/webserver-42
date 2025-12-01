#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <map>
#include <string>
#include "Request.hpp"

class RequestParser {
    private:
        std::map<int, std::string> buffer;
        
        std::string trim(const std::string& str);
        std::string toLower(const std::string& str);
        bool isValidMethod(const std::string& method);
        bool isValidVersion(const std::string& version);
        bool isValidUriChar(char c);
        bool isValidUri(const std::string& uri);

    public:
        Request parse(int fd, const std::string &data);
};

#endif