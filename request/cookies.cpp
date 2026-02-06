#include "RequestParser.hpp"

std::string setCookie(std::string key, std::string value)
{
    return "Set-Cookie: " + key + "=" + value + "; Path=/; HttpOnly\r\n";
}

void RequestParser::parseCookies(Request& req)
{
    if (!req.headers.count("cookie"))
        return;

    std::string cookieHeader = req.headers["cookie"];

    std::stringstream ss(cookieHeader);
    std::string pair;

    while (std::getline(ss, pair, ';')) {
        size_t eq = pair.find('=');
        if (eq == std::string::npos)
            continue;

        std::string key = pair.substr(0, eq);
        std::string val = pair.substr(eq + 1);

        key = RequestParser::trim(key);
        val = RequestParser::trim(val);

        req.cookies[key] = val;
    }
}