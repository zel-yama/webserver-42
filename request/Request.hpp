#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>
// for testing
struct Location {
    std::string path;
    std::vector<std::string> allowed_methods;
    std::string root;
    std::string redirect;
    bool autoindex;
    std::vector<std::string> index_files;
    bool upload_enabled;
    std::string upload_path;
    
    Location(const std::string& p) : path(p), autoindex(false), upload_enabled(false) {}
};

struct ServerConfig {
    size_t max_body_size;
    std::vector<Location> locations;
    
    ServerConfig() : max_body_size(1000000) {}
};

class Request {
    public:
        int status;
        bool headerComplete;
        bool complete;
        std::string method;
        std::string path;
        std::string version;
        std::string body;
        std::map<std::string, std::string> headers;
        
        Location* matchedLocation;   // or testfor test
        std::string redirectUrl;      // f
        std::string resolvedPath;  // for test

        Request();
        std::string getHeader(const std::string& key) const;
        bool hasHeader(const std::string& key) const;
};

#endif