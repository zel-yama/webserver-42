#include "cgi.hpp"
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <sstream>
#include "../request/RequestParser.hpp"
#include "../webserv.hpp"

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
    env["QUERY_STRING"] = req.query;
    std::ostringstream oss;
    oss << body.size();
    env["CONTENT_LENGTH"] = oss.str();

    std::map<std::string, std::string>::const_iterator it = req.headers.find("content-type");
    if (it != req.headers.end())
        env["CONTENT_TYPE"] = it->second;

    env["SERVER_PROTOCOL"] = "HTTP/1.0";
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

// std::string Cgi::execute(const std::string &cgiPath,
//                          const std::string &scriptPath)
// {
//     int inPipe[2];
//     int outPipe[2];

//     if (pipe(inPipe) == -1 || pipe(outPipe) == -1)
//         return "Status: 500 Internal Server Error\r\n\r\nCGI Pipe error";

//     char **envp = envto2Darray();
//     pid_t pid = fork();
//     if (pid == -1)
//         return "Status: 500 Internal Server Error\r\n\r\nCGI Fork error";

//     if (pid == 0)
//     {
//         dup2(inPipe[0], STDIN_FILENO);
//         dup2(outPipe[1], STDOUT_FILENO);

//         close(inPipe[1]);
//         close(outPipe[0]);

//         char *argv[] = {(char *)cgiPath.c_str(),
//                         (char *)scriptPath.c_str(),
//                         NULL};

//         execve(cgiPath.c_str(), argv, envp);
//         exit(1);
//     }

//     close(inPipe[0]);
//     close(outPipe[1]);

//     // write POST body
//     if (!body.empty())
//         write(inPipe[1], body.c_str(), body.size());
//     close(inPipe[1]);

//     fcntl(outPipe[0], F_SETFL, O_NONBLOCK);

//     std::string output;
//     char buffer[1024];
//     ssize_t r;
//     int status;
//     time_t start = time(NULL);

//     while (true)
//     {
//         while ((r = read(outPipe[0], buffer, sizeof(buffer))) > 0)
//             output.append(buffer, r);

//         pid_t res = waitpid(pid, &status, WNOHANG);
//         if (res == pid)
//             break;

//         if (time(NULL) - start > 5) // timeout
//         {
//             kill(pid, SIGKILL);
//             waitpid(pid, NULL, 0);
//             close(outPipe[0]);
//             return "Status: 504 Gateway Timeout\r\n\r\nCGI Timeout";
//         }

//         usleep(10000); // 10ms
//     }

//     close(outPipe[0]);

//     for (size_t i = 0; envp[i]; i++)
//         delete[] envp[i];
//     delete[] envp;

//     if (WIFEXITED(status) && WEXITSTATUS(status) != 0 && output.empty())
//         return "Status: 500 Internal Server Error\r\n\r\nCGI Execution Failed";

//     return output;
// }

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

    if (pipe(inPipe) == -1 || pipe(outPipe) == -1)
    {
        return handle;
    }

    char **envp = envto2Darray();

    pid_t pid = fork();
    if (pid == -1)
    {
        freeEnvp(envp);
        return handle;
    }

    if (pid == 0)
    {
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);

        close(inPipe[1]);
        close(outPipe[0]);

        char *argv[] = {(char *)cgiPath.c_str(), (char *)scriptPath.c_str(), NULL};

        execve(cgiPath.c_str(), argv, envp);
        exit(1);
    }

    close(inPipe[0]);
    close(outPipe[1]);

    fcntl(outPipe[0], F_SETFL, O_NONBLOCK);
    fcntl(inPipe[1], F_SETFL, O_NONBLOCK);

    handle.readFd = outPipe[0];
    handle.writeFd = inPipe[1];
    handle.pid = pid;

    freeEnvp(envp);
    return handle;
}