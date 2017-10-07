#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;


int main(int argc, char**argv)
{
    int p[2];
    pipe(p);
    pid_t pid = fork();
    if(pid==0)
    {
        //child
        dup2(p[1], 1);
        close(p[0]);
        execlp("ls", "ls", (char*)0);
    }
    //parent
    dup2(p[0], 0);
    close(p[1]);
    execlp("sort", "sort", "-r", (char*)0);
    return 1;
}