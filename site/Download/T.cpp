#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>

int main()
{
    int inPipe[2];   // parent -> child (stdin)
    int outPipe[2];  // child -> parent (stdout)

    pipe(inPipe);
    pipe(outPipe);

    pid_t pid = fork();

    if (pid == 0)
    {
        // ===== CHILD (CGI) =====

        // stdin comes from parent
        dup2(inPipe[0], STDIN_FILENO);

        // stdout goes to parent
        dup2(outPipe[1], STDOUT_FILENO);

        // close unused ends
        close(inPipe[1]);
        close(outPipe[0]);
        close(inPipe[0]);
        close(outPipe[1]);

        char *argv[] = { (char*)"cat", NULL };
        execve("/bin/cat", argv, NULL);
        _exit(1);
    }
    else
    {
        // ===== PARENT (SERVER) =====

        close(inPipe[0]);   // parent does not read stdin pipe
        close(outPipe[1]);  // parent does not write stdout pipe

        // send data to child stdin (like POST body)
        const char *msg = "Hello from parent\n";
        write(inPipe[1], msg, strlen(msg));
        close(inPipe[1]); // IMPORTANT: send EOF

        // read child stdout
        char buf[1024];
        ssize_t r;
        while ((r = read(outPipe[0], buf, sizeof(buf))) > 0)
            write(STDOUT_FILENO, buf, r);

        close(outPipe[0]);
        waitpid(pid, NULL, 0);
    }
}