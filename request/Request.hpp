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

};

