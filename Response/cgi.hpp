#ifndef CGI_HPP
#define CGI_HPP


#include "Response.hpp"
#include <sys/wait.h>

struct Cgihandle
{
    int writeFd;
    int readFd;
    pid_t pid;

    Cgihandle() : writeFd(-1),
                  readFd(-1),
                  pid(-1){}
};

class Cgi{
    private:
        std::string body;
        std::string uploadPath;
        std::map<std::string, std::string> env;
        void buildEnv(const Request& req);
    public:
        Cgihandle cgihanlde;
        char **envto2Darray() const;
        void freeEnvp(char **envp);
        Cgi(const Request& req);
        Cgihandle execute(const std::string& cgiPath,
                        const std::string& scriptPath);

};

#endif