#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sstream>

std::string joinPathWithLocation(std::string root, std::string locationPath, std::string &reqPath)
{
    std::string suffix;

    if (locationPath != "/")
    {
        if (reqPath.compare(0, locationPath.length(), locationPath) == 0)
            suffix = reqPath.substr(locationPath.length());
    }
    else
    {
        suffix = reqPath;
    }

    if (!suffix.empty() && suffix[0] != '/')
        suffix = "/" + suffix;

    std::string fullpath = root;
    if (!fullpath.empty() && fullpath[fullpath.size() - 1] == '/' && !suffix.empty() && suffix[0] == '/')
        fullpath.erase(fullpath.size() - 1);
    fullpath += suffix;

    return fullpath;
}


int main()
{
    std::string output = joinPathWithLocation();

    std::cout << "CGI OUTPUT:\n" << output << std::endl;

    return 0;
}