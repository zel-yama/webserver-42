#ifndef CGI_HPP
#define CGI_HPP


#include "Response.hpp"
#include <sys/wait.h>

class Cgi{
    private:
        std::string body;
        std::string uploadPath;
        std::map<std::string, std::string> env;

        void buildEnv(const Request& req);
    public:
        char **envto2Darray() const;
        Cgi(const Request& req);
        std::string execute(const std::string& cgiPath,
                        const std::string& scriptPath);

};








#endif