#pragma once
#include <map>
#include <string>
#include <ctime>

struct Session {
    std::string                         id;
    std::map<std::string, std::string>  data;
    time_t                              createdAt;
    time_t                              lastAccess;
    time_t                              maxAge;

    Session();

    bool isExpired() const;
    void touch();
};

class SessionManager {
public:
    static void init();
    static std::string generateId();
    static std::string create();
    static Session* get(const std::string& id);
    static void destroy(const std::string& id);
    static void evictExpired();
    static size_t count();

private:
    static std::map<std::string, Session> _sessions;
};