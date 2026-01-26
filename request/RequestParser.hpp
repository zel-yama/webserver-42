#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <map>

struct location;

struct Request {
    std::string method;
    std::string path;
    std::string query;
    std::string version;
    std::string body;
    int status;
    
    std::map<std::string, std::string> headers;
    bool complete;
    bool keepalive;
    std::string fullpath;
    location *loc;

    Request();
    ~Request();
};

class RequestParser {
    public:
        Request parse(int fd, std::string& data);

    private:
        std::map<int, std::string> buffer;

        std::string trim(const std::string& s);
        std::string toLower(const std::string& s);

        bool isValidMethod(const std::string& m);
        bool isValidVersion(const std::string& v);

        bool isValidUriChar(char c);
        bool isValidUri(const std::string& uri);
        std::string normalizePath(const std::string& path);

        size_t parseContentLength(const std::string& v);
        bool decodeChunked(std::string& buf, std::string& out);
};

#endif
