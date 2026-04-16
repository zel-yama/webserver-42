#include "cgi.hpp"
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <sstream>
#include "../server/include/Client.hpp"


Cgi::Cgi(const Request &req)
{
    body = req.body;
    uploadPath = "./";
    buildEnv(req);
}

void Cgi::buildEnv(const Request &req)
{
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["REDIRECT_STATUS"] = "200";
    env["REQUEST_METHOD"] = req.method;
    env["SCRIPT_FILENAME"] = req.path;
    env["PATH_INFO"] = "zel-yama";
    env["QUERY_STRING"] = req.query;
    std::ostringstream oss;
    oss << body.size();
    env["CONTENT_LENGTH"] = oss.str();

    std::map<std::string, std::string>::const_iterator it;
    it = req.headers.find("content-type");
    if (it != req.headers.end())
        env["CONTENT_TYPE"] = it->second;

    it = req.headers.find("cookie");
    if (it != req.headers.end())
        env["HTTP_COOKIE"] = it->second;

    env["SERVER_PROTOCOL"] = "HTTP/1.1";
}

char **Cgi::envto2Darray() const
{
    char **envp = new char *[env.size() + 1];
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

void Cgi::freeEnvp(char **envp)
{
    for (size_t i = 0; envp[i]; i++)
        delete[] envp[i];
    delete[] envp;
}

Cgihandle Cgi::execute(const std::string &cgiPath, const std::string &scriptPath)
{
    Cgihandle handle;
    int inPipe[2];
    int outPipe[2];
    int ErrPipe[2];

    if (pipe(inPipe) == -1)
        return handle;
    if (pipe(outPipe) == -1 )
    {
        close(outPipe[1]);
        close(outPipe[0]);
        close(inPipe[0]);
        close(inPipe[1]);
        return handle;
    }
    if (pipe(ErrPipe) == -1){
        close(inPipe[0]);
        close(inPipe[1]);
        close(outPipe[0]);
        close(outPipe[1]);
        close(ErrPipe[0]);
        close(ErrPipe[1]);
    }

    char **envp = envto2Darray();

    pid_t pid = fork();
    if (pid == -1)
    {
        freeEnvp(envp);
        close(inPipe[0]);
        close(inPipe[1]);
        close(outPipe[0]);
        close(outPipe[1]);
        return handle;
    }

    if (pid == 0)
    {

        if (dup2(inPipe[0], STDIN_FILENO) == -1)
        {
            freeEnvp(envp);
            close(inPipe[0]);
            close(inPipe[1]);
            close(outPipe[0]);
            close(outPipe[1]);
            close(ErrPipe[0]);
            close(ErrPipe[1]);
            exit(1);
        }

        if (dup2(outPipe[1], STDOUT_FILENO) == -1)
        {
            freeEnvp(envp);
            close(ErrPipe[0]);
            close(ErrPipe[1]);
            close(inPipe[0]);
            close(inPipe[1]);
            close(outPipe[0]);
            close(outPipe[1]);
            exit(1);
        }
        if (dup2(ErrPipe[1], STDERR_FILENO) == -1)
        {
            freeEnvp(envp);
            close(ErrPipe[0]);
            close(ErrPipe[1]);
            close(inPipe[0]);
            close(inPipe[1]);
            close(outPipe[0]);
            close(outPipe[1]);
            exit(1);
        }
        close(ErrPipe[0]);
        close(inPipe[0]);
        close(outPipe[1]);
        close(inPipe[1]);
        close(outPipe[0]);
     

        std::string dir = scriptPath.substr(0, scriptPath.find_last_of('/'));
        chdir(dir.c_str());
        char *argv[] = {(char *)cgiPath.c_str(), (char *)scriptPath.c_str(), NULL};
        execve(cgiPath.c_str(), argv, envp);
        freeEnvp(envp);
        exit(1);
    }

    close(inPipe[0]);
    close(outPipe[1]);
    close(ErrPipe[1]);
    handle.ErrFd = ErrPipe[0];
    handle.readFd = outPipe[0];
    handle.writeFd = inPipe[1];
    handle.pid = pid;

    freeEnvp(envp);
    return handle;
}
