*This project has been created as part of the 42 curriculum by aayad, zel-yama, mohidbel.*

## Description

**webserv** is a lightweight HTTP web server implementation written in C++. The project aims to create a fully functional web server from scratch, capable of handling multiple client connections, parsing HTTP requests, processing configuration files, and serving web content. This project demonstrates deep understanding of network programming, HTTP protocol, system-level I/O operations, and multi-client handling.

### Key Features

- **Multi-client handling**: Event-driven architecture using select/poll for managing multiple simultaneous connections
- **HTTP Protocol Support**: Full implementation of GET, POST, DELETE methods with proper status codes and headers
- **Configuration Parsing**: Custom configuration file parser supporting server blocks and location definitions
- **CGI Support**: Common Gateway Interface implementation for dynamic content generation
- **Request Handling**: Complete HTTP request parsing with support for headers, body parsing, and multipart form data
- **Session Management**: Cookie-based session management and tracking
- **Error Handling**: Proper HTTP error responses and handling of edge cases
- **File Serving**: Static file serving with support for directory indexing and custom error pages

### Architecture

The server is organized into several modules:

- **server/**: Core server logic, event loop, and client management
- **request/**: HTTP request parsing, validation, and session management
- **Response/**: Response generation and CGI execution
- **config/**: Example configuration files for testing

## Instructions

### Compilation

The project uses a Makefile for building. To compile:

```bash
make
```

This will create the `webserv` executable.

### Clean Build

```bash
make re        # Full rebuild (clean + compile)
make clean     # Remove object files
make fclean    # Remove all generated files
```

### Running the Server

```bash
./webserv [config_file]
```

If no configuration file is specified, the server will look for a default configuration.

### Configuration

Example configuration files are provided in the `config/` directory. A basic configuration looks like:

```
server {
    listen 8080;
    server_name example.com;
    root /var/www/html;
    
    location / {
        allowed_methods GET POST DELETE;
        body_max_byte 1000000;
    }
}
```

### Testing

```bash
curl http://localhost:8080/        # Run basic tests
```

## Resources

### HTTP Protocol References
- [RFC 1945 - HTTP/1.0 Message Syntax and Routing](https://datatracker.ietf.org/doc/html/rfc1945)
- [tutorialspoint - HTTP TUTORIAL](https://www.tutorialspoint.com/http/)

### System Programming
- [Non-Blocking Sockets and I/O Multiplexing with epoll in C](https://medium.com/%40hajorda/non-blocking-sockets-and-i-o-multiplexing-with-epoll-in-c-bd3d8e54c20a)
- [ Nonblocking I/O pdf](https://web.stanford.edu/class/archive/cs/cs110/cs110.1206/lectures/19-slides.pdf)
- [ Nonblocking I/O site](https://www.ibm.com/docs/en/i/7.2.0?topic=designs-example-nonblocking-io-select)
- [Reading unkown number of bytes from a non-blocking socket](https://cplusplus.com/forum/beginner/285198/)
- [USING EPOLL](https://www.dumais.io/page_c5b8d03166dd641c5c1c67d47a46bb4a.html#:~:text=In%20Level,to%20the%20TX%20buffer%2C%20you)
- [Linux man pages - socket(2)](https://man7.org/linux/man-pages/man2/socket.2.html)


### CGI Specification
- [RFC 3875 - Common Gateway Interface](https://tools.ietf.org/html/rfc3875)


## Team Contributions

- **ayad**: Response handling and CGI implementation
- **zel-yama**: Configuration parsing and server core functionality
- **mohidbel**: Request handling, session management, and cookie implementation

