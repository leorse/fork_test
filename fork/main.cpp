#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

//#define MONTRERPB
//#define MONTREREXIT
#define MONTREREXEC

int main(int argc, char**argv)
{
    cout << "oui oui c'est moi" << endl;


    
    pid_t pid=fork();

#ifdef MONTREREXEC  
#pragma message "Montrer l'exécution d'un exec en mode fork"
    int status;
    if(pid==0)
    {
        //FILS
        cout<<"FILS, moi je tourne ("<<getpid()<<")"<<endl;
        execlp("ls", "-a", (char*)0);
        cout<<"FILS, et j'ai fini"<<endl;
        sleep(3);//on attends quand même
        exit(0);
    }
    else
    {
        //PERE
        cout<<"PERE je suis le père et j'attends le fils "<<pid<<"!!"<<endl;
        wait(&status);
        if(WIFEXITED(status))
        {
            cout<<"le fils c'est bien terminé, status:"<<WEXITSTATUS(status)<<endl;
        }
        if(WIFSIGNALED(status))
        {
            cout<<"le fils c'est terminé avec un signal, status:"<<WTERMSIG(status)<<endl;
        }
    }
#endif

#ifdef MONTRERPB 
#pragma message "Montrer le pb de l'exécution simultanée de deux process"   
    if(pid==0)
    {
        int i =0;
        while(i<10000)
        {
            cout << "FILS je suis un fils ->"<<pid<<endl;
            i ++;
        }
    }
    else
    {
        int i=0;
        while(i<10000)
        {
            cout<<"    PERE je suis le père "<<pid<<"!!"<<endl;
            i++;
        }
    }
#endif

#ifdef MONTREREXIT  
#pragma message "Montrer le retour d'exécution d'un fork"
//temps long pour avoir le temps de lancer un signal avec la commande kill
#define TEMPLONG  
#define GENERESIGNAL 
int status;
if(pid==0)
{
    //FILS
    cout<<"FILS, moi je tourne ("<<getpid()<<")"<<endl;
    #ifdef GENERESIGNAL
    #pragma message "Génère un message d'erreur volontaire"
    *(int*)0=42;//on pointe volontairement dans une zone normalement innaccessible pour faire planter
    #endif
    #ifdef TEMPLONG
    #pragma message "compile avec un temps long"
    sleep(500);
    #else
    #pragma message "compile avec un temps court"
    sleep(5);
    #endif
    exit(3);
}
else
{
    //PERE
    cout<<"PERE je suis le père et j'attends le fils "<<pid<<"!!"<<endl;
    wait(&status);
    if(WIFEXITED(status))
    {
        cout<<"le fils c'est bien terminé, status:"<<WEXITSTATUS(status)<<endl;
    }
    if(WIFSIGNALED(status))
    {
        cout<<"le fils c'est terminé avec un signal, status:"<<WTERMSIG(status)<<endl;
    }
}
#endif

    return 1;
}