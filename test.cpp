#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        std::cerr << "Fork failed\n";
        return 1;
    }

    if (pid == 0)
    {
        // CHILD PROCESS
        std::cout << "Child executing Python...\n";

        char* args[] = {
            (char*)"python3",
            (char*)"./site/timeout.py",
            (char*)"arg1",
            NULL
        };
        close(2);
        execvp("python3", args);

        // if exec fails
        printf("execvp failed\n");
        exit(1);
    }
    else
    {
      
        // PARENT PROCESS
        int status;
        // sleep(2);
        int re = waitpid(pid, &status, WNOHANG );
        printf("bool %d, status %d return value %d \n",WIFEXITED(status),  WEXITSTATUS(0), re);
        std::cout << "Child finished\n";
    }

    return 0;
}