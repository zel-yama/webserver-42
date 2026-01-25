#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <map>
#include "unistd.h"

std::map<std::string, std::string> env;

void buildEnv()
{
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["REDIRECT_STATUS"]  = "200";
    env["REQUEST_METHOD"]   = "POST";
    env["SCRIPT_FILENAME"]  = "/home/aayad/Desktop/dddd/script.py";
    // env["QUERY_STRING"]     = r;
    env["CONTENT_LENGTH"]   = "100";
    env["PATH_INFO"] = ".";
    env["CONTENT_TYPE"]     = "php";
    env["SERVER_PROTOCOL"]  = "HTTP/1.0";
}
char **envto2Darray()
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

std::string execute(const std::string& cgiPath,
                         const std::string& scriptPath)
{
    int inPipe[2];
    int outPipe[2];

    buildEnv();
    std::string body = "print(23)";
    pipe(inPipe);    // parent -> child (stdin)
    pipe(outPipe);   // child -> parent (stdout)

    char **envp = envto2Darray();
    pid_t pid = fork();

    if (pid == 0)
    {
        // ===== CHILD (CGI) =====

        dup2(inPipe[0], STDIN_FILENO);    // stdin from parent
        dup2(outPipe[1], STDOUT_FILENO);  // stdout to parent

        close(inPipe[1]);
        close(outPipe[0]);
        close(inPipe[0]);
        close(outPipe[1]);

        char *argv[] = {
            (char*)cgiPath.c_str(),
            (char*)scriptPath.c_str(),
            NULL
        };

        execve(cgiPath.c_str(), argv, envp);
        _exit(1);
    }
    else
    {
        // ===== PARENT (SERVER) =====

        close(inPipe[0]);
        close(outPipe[1]);

        // send request body to CGI (POST data)
        write(inPipe[1], body.c_str(), body.size());
        close(inPipe[1]); // IMPORTANT: EOF

        // read CGI output
        std::string output;
        char buffer[1024];
        ssize_t r;

        while ((r = read(outPipe[0], buffer, sizeof(buffer))) > 0)
            output.append(buffer, r);

        close(outPipe[0]);
        waitpid(pid, NULL, 0);

        // free envp
        for (size_t i = 0; envp[i]; i++)
            delete[] envp[i];
        delete[] envp;

        return output;
    }
}

int main() {
    // std::string s = execute("/home/aayad/Desktop/dddd/test", "/home/aayad/Desktop/dddd/script.py");
    // std::cout << s << std::endl;

    std::string str1 = " abdellah";
    std::string str2 = "ayad";
    str2.append(str1);

    std::cout << str1 << std::endl;
    std::cout << str2 << std::endl;
}