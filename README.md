*This project has been created as part of the 42 curriculum by ayad, zel-yama, mohidbel.*

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

Test files and test cases are available in the `site/` directory:

```bash
cd site
./test.sh        # Run basic tests
python3 test.py  # Run Python-based tests
```

## Resources

### HTTP Protocol References
- [RFC 7230 - HTTP/1.1 Message Syntax and Routing](https://tools.ietf.org/html/rfc7230)
- [RFC 7231 - HTTP/1.1 Semantics and Content](https://tools.ietf.org/html/rfc7231)
- [MDN Web Docs - HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)

### System Programming
- [Linux man pages - select(2)](https://man7.org/linux/man-pages/man2/select.2.html)
- [Linux man pages - socket(2)](https://man7.org/linux/man-pages/man2/socket.2.html)
- [POSIX Sockets Programming](https://www.ibm.com/docs/en/i/7.4?topic=programming-sockets)

### CGI Specification
- [RFC 3875 - Common Gateway Interface](https://tools.ietf.org/html/rfc3875)

### Configuration Parsing
- [NGINX Configuration Documentation](https://nginx.org/en/docs/http/ngx_http_core_module.html)

## Team Contributions

- **ayad**: Response handling and CGI implementation
- **zel-yama**: Configuration parsing and server core functionality
- **mohidbel**: Request handling, session management, and cookie implementation

## AI Usage

AI assistance was utilized for:
- Code review and optimization suggestions
- Bug fixing and debugging complex issues
- Documentation and code comments generation
- Best practices and design pattern recommendations
- Testing edge cases and validation logic

