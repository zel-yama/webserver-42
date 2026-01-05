
#include <iostream>
#include <vector>
#include "RequestHandler.hpp"
#include "Request.hpp"

void printRequest(const Request& req, int testNum) {
    std::cout << "\n========== TEST " << testNum << " =========="  << std::endl;
    std::cout << "Method:   " << req.method << std::endl;
    std::cout << "Path:     " << req.path << std::endl;
    std::cout << "Version:  " << req.version << std::endl;
    std::cout << "Status:   " << req.status << " ";
    

    switch(req.status) {
        case 200: std::cout << "(OK)"; break;
        case 301: std::cout << "(Moved Permanently)"; break;
        case 400: std::cout << "(Bad Request)"; break;
        case 404: std::cout << "(Not Found)"; break;
        case 405: std::cout << "(Method Not Allowed)"; break;
        case 413: std::cout << "(Payload Too Large)"; break;
        case 414: std::cout << "(Request-URI Too Long)"; break;
        case 501: std::cout << "(Not Implemented)"; break;
        case 505: std::cout << "(HTTP Version Not Supported)"; break;
        default: std::cout << "(Unknown)"; break;
    }
    std::cout << std::endl;
    
    std::cout << "Complete: " << (req.complete ? "Yes" : "No") << std::endl;
    std::cout << "Headers:  " << req.headers.size() << std::endl;
    
    if (!req.body.empty()) {
        std::cout << "Body:     " << req.body.length() << " bytes" << std::endl;
    }
}

// Setup a mock configuration
ServerConfig setupConfig() {
    ServerConfig config;
    config.max_body_size = 1000; // 1KB for testing
    
    // Location 1: Root - allows GET only
    Location root("/");
    root.allowed_methods.push_back("GET");
    root.root = "/var/www/html";
    config.locations.push_back(root);
    
    // Location 2: Upload - allows GET, POST
    Location upload("/upload");
    upload.allowed_methods.push_back("GET");
    upload.allowed_methods.push_back("POST");
    upload.root = "/var/www/uploads";
    upload.upload_enabled = true;
    config.locations.push_back(upload);
    
    // Location 3: API - allows all methods
    Location api("/api");
    api.allowed_methods.push_back("GET");
    api.allowed_methods.push_back("POST");
    api.allowed_methods.push_back("DELETE");
    api.root = "/var/www/api";
    config.locations.push_back(api);
    
    // Location 4: Old path with redirect
    Location oldPath("/old");
    oldPath.redirect = "/new";
    oldPath.allowed_methods.push_back("GET");
    config.locations.push_back(oldPath);
    
    return config;
}

int main() {
    std::cout << "==================================" << std::endl;
    std::cout << "REQUEST HANDLER TEST SUITE" << std::endl;
    std::cout << "==================================" << std::endl;
    
    RequestHandler handler;
    ServerConfig config = setupConfig();
    
    // TEST 1: Valid GET request to root
    {
        std::string request1 = 
            "GET / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "User-Agent: TestClient/1.0\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(1, request1, config);
        printRequest(req, 1);
        std::cout << "Expected: 200 OK" << std::endl;
    }
    
    // TEST 2: POST to root (not allowed)
    {
        std::string request2 = 
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 10\r\n"
            "\r\n"
            "test=hello";
        
        Request req = handler.handleRequest(2, request2, config);
        printRequest(req, 2);
        std::cout << "Expected: 405 Method Not Allowed" << std::endl;
    }
    
    // TEST 3: GET to non-existent location
    {
        std::string request3 = 
            "GET /nonexistent HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(3, request3, config);
        printRequest(req, 3);
        std::cout << "Expected: 404 Not Found" << std::endl;
    }
    
    // TEST 4: Request with redirect
    {
        std::string request4 = 
            "GET /old/page.html HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(4, request4, config);
        printRequest(req, 4);
        std::cout << "Expected: 301 Moved Permanently" << std::endl;
    }
    
    // TEST 5: POST to /upload (allowed)
    {
        std::string request5 = 
            "POST /upload HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 5\r\n"
            "\r\n"
            "hello";
        
        Request req = handler.handleRequest(5, request5, config);
        printRequest(req, 5);
        std::cout << "Expected: 200 OK" << std::endl;
    }
    
    // TEST 6: POST with body too large
    {
        std::string largeBody(1500, 'x'); // 1.5KB body
        std::string request6 = 
            "POST /upload HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 1500\r\n"
            "\r\n" + largeBody;
        
        Request req = handler.handleRequest(6, request6, config);
        printRequest(req, 6);
        std::cout << "Expected: 413 Payload Too Large" << std::endl;
    }
    
    // TEST 7: Invalid HTTP method
    {
        std::string request7 = 
            "PATCH / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(7, request7, config);
        printRequest(req, 7);
        std::cout << "Expected: 501 Not Implemented" << std::endl;
    }
    
    // TEST 8: Invalid HTTP version
    {
        std::string request8 = 
            "GET / HTTP/2.0\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(8, request8, config);
        printRequest(req, 8);
        std::cout << "Expected: 505 HTTP Version Not Supported" << std::endl;
    }
    
    // TEST 9: URI too long
    {
        std::string longUri(2100, 'a'); // 2100 characters
        std::string request9 = 
            "GET /" + longUri + " HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(9, request9, config);
        printRequest(req, 9);
        std::cout << "Expected: 414 Request-URI Too Long" << std::endl;
    }
    
    // TEST 10: Malformed request (no version)
    {
        std::string request10 = 
            "GET /\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(10, request10, config);
        printRequest(req, 10);
        std::cout << "Expected: 400 Bad Request" << std::endl;
    }
    
    // TEST 11: DELETE to /api (allowed)
    {
        std::string request11 = 
            "DELETE /api/user/123 HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(11, request11, config);
        printRequest(req, 11);
        std::cout << "Expected: 200 OK" << std::endl;
        std::cout << req.path << std::endl;
        std::cout << req.method << std::endl;
    }
    
    // TEST 12: Invalid URI characters
    {
        std::string request12 = 
            "GET /test<script> HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(12, request12, config);
        printRequest(req, 12);
        std::cout << "Expected: 400 Bad Request" << std::endl;
    }
    
    // TEST 13: POST without Content-Length or Transfer-Encoding
    {
        std::string request13 = 
            "POST /upload HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(13, request13, config);
        printRequest(req, 13);
        std::cout << "Expected: 400 Bad Request" << std::endl;
    }
    
    // TEST 14: Transfer-Encoding with invalid value
    {
        std::string request14 = 
            "POST /upload HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: gzip\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(14, request14, config);
        printRequest(req, 14);
        std::cout << "Expected: 501 Not Implemented" << std::endl;
    }
    
    // TEST 15: Longest location match test
    {
        std::string request15 = 
            "GET /upload/file.txt HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        
        Request req = handler.handleRequest(15, request15, config);
        printRequest(req, 15);
        std::cout << "Expected: 200 OK (matched /upload, not /)" << std::endl;
    }
    
    std::cout << "\n==================================" << std::endl;
    std::cout << "ALL TESTS COMPLETED" << std::endl;
    std::cout << "==================================" << std::endl;
    
    return 0;
}