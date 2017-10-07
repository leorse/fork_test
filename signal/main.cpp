#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

//#define SIGNAL
#define SIGNATION
#define SIGBLOCK

using namespace std;
void handler(int sigtype)
{
    printf("signal reçu:%d\n", sigtype);
    return;
}

int main(int argc, char**argv)
{
    int i = -5000;//pour faire une boucle longue
    #ifdef SIGNAL
    #pragma message "Compilé pour les signaux avec 'signal'"
    //il faut faire un ps pour trouver le pid de la commande signal.exe
    //ensuite on envoie kill -SIGTERM, ou -SIGQUIT
    signal(SIGINT, handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    #endif

    #ifdef SIGNATION
    #pragma message "Compilé pour la commande 'signation'"
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);
    #endif

    #ifdef SIGBLOCK
    #pragma message "Compilé avec un blocage"
    i=0; //pour faire une boucle courte
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGHUP);
    sigprocmask(SIG_BLOCK, &set, NULL);
    #endif
    while(i<20)
    {
        printf("ça roule\n");
        sleep(1);
        i++;
    }
    #ifdef SIGBLOCK
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    //après cette ligne, si on effectue un kill -SIGHUP, le traitement s'arrête.
    //mais pas avant
    while(1)
    {
        printf("ça roule non bloqué\n");
        sleep(1);
    }
    #endif
    return 1;
}