#pragma once
#include <map>
#include <string>
#include <ctime>

struct Session {
    std::string                         id;
    time_t                              createdAt;
    time_t                              lastAccess;
    time_t                              maxAge;

    Session();

    bool isExpired() const;
    void touch();
};

class SessionManager {
public:
    static std::string generateId();
    static std::string create();
    static Session* get(const std::string& id);

private:
    static std::map<std::string, Session> _sessions;
};