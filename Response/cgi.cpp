#include "cgi.hpp"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include "../request/RequestParser.hpp"


Cgi::Cgi(const Request& req)
{
    body = req.body;
    buildEnv(req);
}

void Cgi::buildEnv(const Request& req)
{
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["REDIRECT_STATUS"]  = "200";
    env["REQUEST_METHOD"]   = req.method;
    env["SCRIPT_FILENAME"]  = req.path;
    env["QUERY_STRING"]     = req.query;
    std::ostringstream oss;
    oss << body.size();
    env["CONTENT_LENGTH"]   = oss.str();
    // env["CONTENT_TYPE"]     = req.headers["Content-Type"];
    env["SERVER_PROTOCOL"]  = "HTTP/1.0";
}
char **Cgi::envto2Darray() const
{
    char **envp = new char*[env.size() + 1];
    size_t i = 0;

    for (std::map<std::string, std::string>::const_iterator it = env.begin();
         it != env.end(); ++it)
    {
        std::string line = it->first + "=" + it->second;
        envp[i] = new char[line.size() + 1];
        std::strcpy(envp[i], line.c_str());
        i++;
    }
    envp[i] = NULL;
    return envp;
}

std::string Cgi::execute(const std::string& cgiPath,
                        const std::string& scriptPath)
{
    int inPipe[2];
    int outPipe[2];
    pipe(inPipe);
    pipe(outPipe);

    char **env = envto2Darray();
    pid_t pid = fork();

    if (pid == 0)
    {
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);
        
        close(inPipe[1]);
        close(inPipe[0]);
        close(outPipe[0]);
        close(outPipe[1]);

        char *argv[] = {
            (char*)cgiPath.c_str(),
            (char*)scriptPath.c_str(),
            NULL
        };

        execve(cgiPath.c_str(), argv, env);
        exit(1);
    }
    else
    {
        close(inPipe[0]);
        close(outPipe[1]);

        write(inPipe[1], body.c_str(), body.size());
        close(inPipe[1]);

        std::string output;
        char buffer[1024];
        ssize_t r;

        while ((r = read(outPipe[0], buffer, sizeof(buffer))) > 0)
            output.append(buffer, r);
        
        close(outPipe[0]);
        waitpid(pid, NULL, 0);
        
        for (size_t i = 0; env[i]; i++)
            delete[] env[i];
        delete[] env;
        
        return output;
    }
    return "";
}