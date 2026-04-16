#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include "../server/include/Client.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

struct Request;
class Server;

class Response
{
private:
    int statusCode;
    std::string statusMessage;
    std::string version;
    std::string body;
    std::map<std::string, std::string> headers;
    std::map<int, std::string> statusMap;
    bool LargeFile;
    Server *srv;
    Request *req;
    bool keepStatus;
    size_t fileSize;
    std::string filePath;
    
    bool cgiPending;
    
    std::vector<std::string> queryCom;
    
public:
    Response();
    ~Response();
    pid_t cgiPid;
    // location *loc;
    int  cgiReadFd;
    int  cgiWriteFd;
    int   cgiError;
    std::string logMethod;
    std::string logIpAdress;
    std::string logPath;
    std::string logUserAgent;
    
    void setStatus(int code, const std::string &message);
    void setHeader(const std::string &key, const std::string &value);
    void setBody(const std::string &body);
    void setContentType(const std::string &path);
    void setVersion(const std::string &version);
    int getStatusCode() const;

    const std::string &getStatusMessage() const;
    const std::string &getBody() const;
    const std::map<std::string, std::string> &getHeaders() const;
    std::string getMimeType(const std::string &extension) const;
    std::string getFileExtention(const std::string &path) const;
    void processRequest(Request &req, Server &ser);
    void handleGet(std::string &path, const Request &req, const Server &srv);

    void sendError(int code, const std::string &message);
    void servErrorPage(int code);
    void servFile(const std::string &payh);
    std::string readFile(const std::string &path) const;
    bool existFile(const char *path) const;
    bool isDirectory(const char *path) const;
    void handleDirectory(std::string &path, const Request &req, const Server &srv);
    void generateautoindex(const std::string &path);
    std::string build();
    bool isLargeFile() const;
    std::string& getFilePath();
    size_t getFileSize() const;
    void setContext(Request *r, Server *s);
    bool hasReadPermission(const std::string &path) const;
    bool hasDirPermission(const std::string &path) const;
    void handlePost(const std::string &path, const Request &req, const Server &srv);
    void handleDelete(const std::string &path,
        const Request &req,
        const Server &srv);
        void handleMultipartUpload(const Request &req, const Server &srv);
        
        // for cgi fds
    void applyCgiResponse(const std::string &cgiOutput);
    bool isCgipending() const;
    int getcgiReadFd() const;
    int getcgiWriteFd() const;
    pid_t getcgiPid() const;

    void resetCgiFlag();
};

#endif
