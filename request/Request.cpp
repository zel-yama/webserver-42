#include "Request.hpp"
#include <algorithm>
#include <cctype>

Request::Request() : headerComplete(false), complete(false) {}

std::string Request::getHeader(const std::string& key) const {
    std::string k = key;

    for (size_t i = 0; i < k.size(); ++i)
        k[i] = std::tolower(k[i]);

    std::map<std::string, std::string>::const_iterator it = headers.begin();
    while (it != headers.end()) {
        if (it->first == k)
            return it->second;
        ++it;
    }

    return "";
}

bool Request::hasHeader(const std::string& key) const {
    std::string k = key;
    
    for (size_t i = 0; i < k.size(); ++i)
        k[i] = std::tolower(k[i]);

    return headers.find(k) != headers.end();
}
