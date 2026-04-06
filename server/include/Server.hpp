#ifndef SERVER_HPP
#define SERVER_HPP

#define TIMEOUT 62
#define MAX_URI 2048
#define TIMEOUTCGI 30
#define MAXEVENT 1000
#define MAXCLIENT 1000
#define MAXSIZEBYTE 165000

#include "include.hpp"

class Client;

// struct location;
class Response;
struct location {
 
    location();
    int                                 fdEp;
    int                                  upload;
    map<std::string, std::string>        CgiCofing;
    std::string                          uploadPath;
    std::string                          cgiPath;
    std::string                          cgiExten;
    int                                  cgiStatus;
    std::vector<std::string>             allowedMethods;
    std::string                          root;
    std::string                          returnP;
    int                                  returnCode;
    std::string                          locationPath;
    std::vector<std::string>             indexFile;
    size_t                               bodyMaxByte;
    int                                  outoIndex;
    int                                  ex;
    std::map<int, std::string >          D_ErrorPages;
};

struct MultipartPart {
    std::string name;
    std::string filename;
    std::string contentType;
    std::string content;
};


struct Request {
    std::string method;
    std::string path;
    std::string query;
    std::string version;
    std::string body;
    int status;
    
    std::map<std::string, std::string> headers;
    std::vector<MultipartPart> multipartData;
    std::map<std::string, std::string> cookies;
    std::string     sessionId;

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


typedef map<string, string>::iterator iter;
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
        std::map<int, std::string >         D_ErrorPages; //exit code with error page to that error 

        RequestParser                       parser;
       // Response                            *respone;
        map<std::string, std::string>       cgiConfig;
        int                                 CreateServer( std::string ipaddress );
        void                                listenFunction();
        Client                              acceptClient();
    };
    
typedef std::vector<Server> servers;



// Helper to get server from client (me again)
int             makeNonBlockingFD(int socket);
void            eventLoop(maptype &config );
int             addSockettoEpoll(int fdEp, struct epoll_event  data);
void            myThrow();
void            cleanUP(maptype &config);
Server*         getServerFromClient(maptype& config, Client& client);
void            costumThrow(std::string des, std::string invalid);
bool            checkTimeout(time_t prevTime, time_t timeout );
void            handlingOFCGi(maptype &data, int fd, int flag, Response &respone );
Config          *returnElement(int fd, maptype &data);
std::string     findElement(maptype &config, int fd);
std::string     convertIpAdder( uint32_t ipaddres);
void            __displayTime();
void printStrings(std::string str1, std::string str2, std::string str3, int str4);

#endif