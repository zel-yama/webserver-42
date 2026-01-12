#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
<<<<<<< HEAD
#include "../webserv.hpp"
#include "../server/Server.hpp"
=======
#include "../request/RequestParser.hpp"
// #include "../webserv.hpp"
>>>>>>> 1b3052310a3c050f0c43e700afad5866ff9f4475

// struct Request;
class Server;

class Response {
private:
    int                 statusCode;
    std::string         statusMessage;
    std::string         protocol;
    std::string         version;
    std::string         body;
    std::string         raw;
    std::map<std::string, std::string> headers;
    std::map<int, std::string> statusMap;

public:
    Response();
    ~Response();

    void setStatus(int code, const std::string& message);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void setContentLength(const std::string& path);
    void setContentType(const std::string& path);
    void setVersion(const std::string& version);
    int getStatusCode() const;
    
    const std::string& getStatusMessage() const;
    const std::string& getBody() const;
    const std::map<std::string, std::string>& getHeaders() const;
    std::string getMimeType(const std::string& extension) const;
    std::string getFileExtention(const std::string &path) const;
    void processRequest(Request& req, Server& ser);
    void handleGet(const std::string& path, const Request& req, const Server& srv);

    void sendError(int code, const std::string& message);
    void servErrorPage(int code);
    void servFile(const std::string& payh);
    std::string readFile(const std::string& path) const;
    bool existFile(const char *path) const;
    bool isDirectory(const char *path) const;
    std::string build(); 
};

#endif
