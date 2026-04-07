#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid2 = 0;
    pid_t pid = fork();
    
    if(pid == 0)
        pid2 = fork();
    if(pid2 == 0)
        printf("%d\n", pid);

    if (pid == 0)
        printf("I am child\n");
    else if (pid2 == 0)
        printf("I am parent\n");
    return 0;
}