#ifndef SERVER_HPP
#define SERVER_HPP

#define TIMEOUT 15
#define MAXEVENT 1000
#define MAXCLIENT 1000
#define MAXSIZEBYTE 165000

#include "include.hpp"
class Client;

struct location;
class Response;

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

    bool headersParsed;
    
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
        std::map<int, std::string> buffer;
        std::map<int, Request> requests;
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
    
    std::vector<std::string>                indexFile;
    unsigned int                            port;
    std::string                             ipAdress;
    std::string                             returnP;
    size_t                                  bodyMaxByte;
    std::string                              root;
    int upload;
    std::string                             uploadPath;
    std::string                             cgiPath;
    std::string                             cgiExten;
    bool                                    infoFull;
    int                                     cgiStatus;
        std::vector<std::string>            allowedMethods;
        int                                  outoIndex;
        int                                 fd;
        int                                 returnCode ;
        struct sockaddr_in                  addressServer;
        std::string                         ServerName;
        std::string                         ClientMaxBody;
        std::vector<location>               objLocation;
        std::map<int, std::string >         D_ErrorPages; //exit code with error page to that error 

        // for me  (mohamed)
        RequestParser               *parser;
        Response                    *respone;
        map<std::string, std::string>       cgiConfig;
        int                                 CreateServer(int port, std::string ipaddress );
        void                                listenFunction();
        Client                              acceptClient();
    };
    
typedef std::vector<Server> servers;
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


int makeNonBlockingFD(int socket);
void eventLoop(maptype config );
int addSockettoEpoll(int fdEp, struct epoll_event  data);
size_t countBuffersize(std::string buffer, Client &connect);

// Helper to get server from client (me again)
Server*     getServerFromClient(maptype& config, Client& client);
void        costumThrow(std::string des, std::string invalid);
bool    checkTimeout(time_t prevTime, time_t timeout );
void    handlingOFCGi(maptype &data, Server *srv, _Cgi *cg, Client *connect);
#endif