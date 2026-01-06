#ifndef REQUEST_HPP
#define REQUEST_HPP
#pragma once

#include "../webserv.hpp"

class Request {
    private:
        std::string Method; // it's Http i think ?
        std::string Target; // imkn ex: GET /index.html >> index.html hiya target
        std::string http_version; // GET /index.html HTTP/1.1 >> 1.1 howa version
        std::map<std::string, std::string> headers; // homa ex: host: localhost:8080
        std::string body; // just in post and put
        

    public:
        Request();
        Request(const Request& other);
        Request& operator=(const Request& other);
        ~Request();

        std::string getMethod();
        std::string getTarget();

#include <string>
#include <map>
#include <vector>
// for testing
struct Location {
    std::string path;
    std::vector<std::string> allowed_methods;
    std::string root;
    std::string redirect;
    bool autoindex;
    std::vector<std::string> index_files;
    bool upload_enabled;
    std::string upload_path;
    
    Location(const std::string& p) : path(p), autoindex(false), upload_enabled(false) {}
};

struct ServerConfig {
    size_t max_body_size;
    std::vector<Location> locations;
    
    ServerConfig() : max_body_size(1000000) {}
};


#endif