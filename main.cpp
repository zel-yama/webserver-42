#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sstream>

// 🔹 Simple Request struct (for test)
struct Request {
    std::string method;
    std::string path;
    std::string query;
    std::string body;
    std::string pathInfo;
    std::map<std::string, std::string> headers;
};

class Cgi {
private:
    std::map<std::string, std::string> env;
    std::string body;

public:
    Cgi(const Request &req) {
        body = req.body;
        buildEnv(req);
    }

    // ------------------------
    // Build ENV
    // ------------------------
    void buildEnv(const Request &req)
    {
        env["GATEWAY_INTERFACE"] = "CGI/1.1";
        env["REQUEST_METHOD"] = req.method;
        env["SCRIPT_FILENAME"] = req.path;
        env["QUERY_STRING"] = req.query;
        env["PATH_INFO"] = "";
        env["SERVER_PROTOCOL"] = "HTTP/1.1";
        env["REDIRECT_STATUS"] = "200";

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
    }

    // ------------------------
    // Convert env → char**
    // ------------------------
    char **envToArray() const
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

    void freeEnvp(char **envp)
    {
        for (size_t i = 0; envp[i]; i++)
            delete[] envp[i];
        delete[] envp;
    }

    // ------------------------
    // Execute CGI
    // ------------------------
    std::string execute(const std::string &cgiPath, const std::string &scriptPath)
    {
        int inPipe[2];
        int outPipe[2];

        pipe(inPipe);
        pipe(outPipe);

        char **envp = envToArray();

        pid_t pid = fork();
        if (pid == 0)
        {
            // 🔹 CHILD

            dup2(inPipe[0], STDIN_FILENO);
            dup2(outPipe[1], STDOUT_FILENO);

            close(inPipe[1]);
            close(outPipe[0]);

            std::string dir = scriptPath.substr(0, scriptPath.find_last_of('/'));
            chdir(dir.c_str());

            char *argv[] = {
                (char *)cgiPath.c_str(),
                (char *)scriptPath.c_str(),
                NULL
            };

            execve(cgiPath.c_str(), argv, envp);
            exit(1);
        }

        // 🔹 PARENT
        close(inPipe[0]);
        close(outPipe[1]);

        // ✅ WRITE BODY TO CGI
        if (!body.empty())
        {
            write(inPipe[1], body.c_str(), body.size());
        }
        close(inPipe[1]); // VERY IMPORTANT

        // ✅ READ CGI OUTPUT
        std::string result;
        char buffer[1024];
        ssize_t n;

        while ((n = read(outPipe[0], buffer, sizeof(buffer))) > 0)
        {
            result.append(buffer, n);
        }

        close(outPipe[0]);

        waitpid(pid, NULL, 0);

        freeEnvp(envp);

        return result;
    }
};

// ------------------------
// TEST MAIN
// ------------------------
int main()
{
    Request req;
    req.method = "POST";
    req.path = "/workspaces/webserver-42/YoupiBanane/youpi.bla";
    req.query = "";
    req.body = "ykguytyuktuytkuy";
    req.headers["content-type"] = "text/plain";

    Cgi cgi(req);

    std::string output = cgi.execute(
        "/workspaces/webserver-42/cgi_tester",
        "/workspaces/webserver-42/YoupiBanane/youpi.bla"
    );

    std::cout << "CGI OUTPUT:\n" << output << std::endl;

    return 0;
}