#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char**argv)
{
    char *arg_execv[] = {"ls", "-a", 0};
    char *envp[] = {"EDITOR=vi", "TZ=:EST", 0};
    cout << "oui oui c'est moi" << endl;

    execl("bin/ls", "ls", "-a", (char*)0);
    execle("/bin/ls", "ls", "-a", (char*)0, envp);
    execv("/bin/ls", arg_execv);
    execve("/bin/ls", arg_execv, envp);
    execlp("ls", "ls", "-a", (char*)0);
    execvp("ls", arg_execv);
    execvpe("ls", arg_execv, envp);
    pid_t i=fork();
    return 1;
}