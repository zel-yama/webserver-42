#ifndef SERVER_HPP
#define SERVER_HPP

#define TIMEOUT 30
#define MAX_URI 2048
#define TIMEOUTCGI 4
#define MAXEVENT 1000
#define MAXCLIENT 1000
#define MAXSIZEBYTE 16500

#include "include.hpp"

class Client;


class Response;
struct location {
 
    location();
    int                                  ex;
    int                                  fdEp;
    int                                  upload;
    int                                  outoIndex;
    int                                  cgiStatus;
    int                                  returnCode;
    size_t                               bodyMaxByte;
    std::string                          root;
    std::string                          returnP;
    std::string                          cgiPath;
    std::string                          cgiExten;
    std::string                          uploadPath;
    std::string                          locationPath;
    std::vector<std::string>             indexFile;
    std::vector<std::string>             allowedMethods;
    std::map<int, std::string >          D_ErrorPages;
    std::map<std::string, std::string>   CgiCofing;
};

struct MultipartPart {
    std::string name;
    std::string filename;
    std::string contentType;
    std::string content;
};


struct Request {
    int status;
    std::string ip;
    std::string path;
    std::string body;
    std::string query;
    std::string method;
    std::string version;
    
    std::string     sessionId;
    std::vector<MultipartPart> multipartData;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> cookies;

    
    bool headersParsed;
    bool complete;
    bool keepalive;
    std::string fullpath;
    location loc;

    Request();
    ~Request();
};

class RequestParser {
    public:
        Request parse(int fd);
        std::map<int, std::string> buffer;
        std::map<int, Request> requests;
        std::string resolveSession(Request& req);
    private:

        std::string trim(const std::string& s);
        std::string toLower(const std::string& s);

        bool isValidMethod(const std::string& m);
        bool isValidVersion(const std::string& v);

        bool isValidUriChar(char c);
        bool isValidUri(const std::string& uri);
        std::string normalizePath(const std::string& path);

        size_t parseContentLength(const std::string& v);
        bool decodeChunked(std::string& buf, std::string& out);
        bool parseBody(std::string& b, Request& req);
        bool parseHeaders(std::string& b, Request& req);
        
        std::string extractBoundary(std::string& contentType);
        bool parseMultipart(std::string& body, std::string& boundary, Request& req);
        void parseCookies(Request& req);
};
std::string setCookie(std::string key, std::string value);


typedef std::map<std::string, std::string>::iterator iter;
class Server : public Config {
    
    public:
    
        Server();
        ~Server();
        std::vector<std::string>                indexFile;
        unsigned int                            port;
        std::string                             ipAdress;
        std::string                             returnP;
        size_t                                  bodyMaxByte;
        std::string                              root;
        int upload;
        std::string                             uploadPath;
        std::string                             cgiPath;

 
        int                                  cgiStatus;
        std::vector<std::string>            allowedMethods;
        int                                  outoIndex;
        int                                 fd;
        int                                 returnCode ;
        struct sockaddr_in                  addressServer;
        std::string                         ServerName;
        std::string                         ClientMaxBody;
        std::vector<location>               objLocation;
        std::map<int, std::string >         D_ErrorPages; 

        RequestParser                       parser;
      
        std::map<std::string, std::string>       cgiConfig;
        int                                 CreateServer( std::string ipaddress );
        void                                listenFunction();
        Client                              acceptClient();
    };
    
typedef std::vector<Server> servers;




int             makeNonBlockingFD(int socket);
void            eventLoop(maptype &config );
int             addSockettoEpoll(int fdEp, struct epoll_event  data);
void            myThrow();
void            cleanUP(maptype &config);
Server*         getServerFromClient(maptype& config, Client& client);
void            costumThrow(std::string des, std::string invalid);
bool            checkTimeout(time_t prevTime, time_t timeout );
void            handlingOfCgi(maptype &data, int fd, int flag  );
Config          *returnElement(int fd, maptype &data);
std::string     findElement(maptype &config, int fd);
std::string     convertIpAdder( uint32_t ipaddres);
void            __displayTime();

#endif