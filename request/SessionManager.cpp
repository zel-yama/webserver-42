#include "SessionManager.hpp"
#include <sstream>
#include <cstdlib>

std::map<std::string, Session> SessionManager::_sessions;

Session::Session()
    : createdAt(time(NULL)),
      lastAccess(time(NULL)),
      maxAge(1800)
{}

bool Session::isExpired() const {
    return (time(NULL) - lastAccess) >= maxAge;
}

void Session::touch() {
    lastAccess = time(NULL);
}


std::string SessionManager::generateId() {
    std::ostringstream ss;
    for (int i = 0; i < 4; ++i) {
        unsigned int part = static_cast<unsigned int>(rand());
        ss << std::hex;
        ss.width(8);
        ss.fill('0');
        ss << part;
    }
    return ss.str();
}

std::string SessionManager::create() {
    std::string id = generateId();
    while (_sessions.count(id))
        id = generateId();

    Session s;
    s.id = id;
    _sessions[id] = s;
    return id;
}

Session* SessionManager::get(const std::string& id) {
    std::map<std::string, Session>::iterator it = _sessions.find(id);
    if (it == _sessions.end())
        return NULL;

    if (it->second.isExpired()) {
        _sessions.erase(it);
        return NULL;
    }

    it->second.touch();
    return &it->second;
}