#include "../include/Server.hpp"
#include "../include/tools.hpp"

Server* getServerForClient(maptype& config, int serverId);
void validateRequest(Request& req, Server* srv);

void checkClientsTimeout(maptype& config, int fdEp) 
{
    Config *data;
    Client *connect;
    
    for (ConfigIter i = config.begin(); i != config.end(); i++) {
        if (i->second->name == "client") {
            connect = dynamic_cast<Client*>(i->second);
            
            if ((time(NULL) - connect->prevTime) > (connect->timeout - 10)) {
                printf("Client [%d] close to timeout about 10s\n", connect->fd);
            }
            if ((time(NULL) - connect->prevTime) > (connect->timeout + 2)) {
                deleteClient(config, connect->fd, fdEp);
                return;
            }
        }
    }
}

void checkClientConnection(maptype &config, Client &connect) {
    if (connect.sending) {
        connect.buffer = "";
        setClientSend(connect.fdEp, connect);
        return;
    }
    
    // Check if connection should close (from parsed request)
    if (connect.parsedRequest.should_close || !connect.keepAlive) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    // Keep-alive: reset for next request
    connect.prevTime = time(NULL);
    connect.requestFinish = false;
    connect.headersOnly = false;
    connect.bodysize = 0;
    connect.byteSent = 0;
    connect.bodySizeStatus = false;
    connect.buffer = "";
    
    // Reset parsed request
    connect.parsedRequest = Request();
    
    setClientRead(connect.fdEp, connect);
}

std::string buildErrorResponse(int status, bool shouldClose) {
    std::string statusText;
    std::string body;
    
    switch (status) {
        case 400:
            statusText = "Bad Request";
            body = "<html><body><h1>400 Bad Request</h1></body></html>";
            break;
        case 404:
            statusText = "Not Found";
            body = "<html><body><h1>404 Not Found</h1></body></html>";
            break;
        case 405:
            statusText = "Method Not Allowed";
            body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
            break;
        case 413:
            statusText = "Payload Too Large";
            body = "<html><body><h1>413 Payload Too Large</h1></body></html>";
            shouldClose = true;  // Always close on 413
            break;
        case 500:
            statusText = "Internal Server Error";
            body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
            break;
        default:
            statusText = "Error";
            body = "<html><body><h1>Error</h1></body></html>";
    }
    
    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << statusText << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "Connection: " << (shouldClose ? "close" : "keep-alive") << "\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}

void sendResponse(maptype &config, Client &connect) {
    std::string response;
    
    // Get the server configuration
    Server* srv = getServerForClient(config, connect.serverId);
    
    if (srv && connect.parsedRequest.complete) {
        // Validate request against server config (location, methods, body size)
        if (connect.parsedRequest.status == 200) {
            validateRequest(connect.parsedRequest, srv);
        }
        
        // Build response based on status
        if (connect.parsedRequest.status == 200) {
            // TODO: Implement actual file serving here
            // For now, return a simple success response
            std::string body = "<html><body><h1>Hello from WebServ!</h1><p>Path: " + 
                              connect.parsedRequest.path + "</p></body></html>";
            
            std::ostringstream resp;
            resp << "HTTP/1.1 200 OK\r\n";
            resp << "Content-Type: text/html\r\n";
            resp << "Content-Length: " << body.length() << "\r\n";
            resp << "Connection: " << (connect.parsedRequest.should_close ? "close" : "keep-alive") << "\r\n";
            resp << "\r\n";
            resp << body;
            
            
            response = resp.str();
        }
        else if (connect.parsedRequest.status == 301) {
            // Redirect
            std::ostringstream resp;
            resp << "HTTP/1.1 301 Moved Permanently\r\n";
            resp << "Location: " << connect.parsedRequest.headers["location"] << "\r\n";
            resp << "Content-Length: 0\r\n";
            resp << "Connection: " << (connect.parsedRequest.should_close ? "close" : "keep-alive") << "\r\n";
            resp << "\r\n";
            
            response = resp.str();
        }
        else {
            // Error response
            response = buildErrorResponse(connect.parsedRequest.status, 
                                        connect.parsedRequest.should_close);
        }
    } else {
        // No parsed request or server not found - send 500
        response = buildErrorResponse(500, true);
    }
    
    // Send the response
    int n = send(connect.fd, response.c_str(), response.length(), 0);
    
    if (n <= 0) {
        deleteClient(config, connect.fd, connect.fdEp);
        return;
    }
    
    std::cout << "Sent " << n << " bytes (HTTP " << connect.parsedRequest.status << ")" << std::endl;
    
    connect.sending = (n < (int)response.length());
    
    checkClientConnection(config, connect);
}