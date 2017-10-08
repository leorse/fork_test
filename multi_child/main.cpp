#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

#define NBFILS  12

int tab[][20] = 
    {
        {15,16,28,45,87,22,34, 7,9 ,27,11,36,17,44,26, 0},
        {25,26,38,55,24,32,44,17,19,37,21,16,51,54,76, 0},
        {35,36,48,15,7, 42,54,27,29,47,31,26,33,64,46, 0},
        {45,46,58,15,17,52,64,37,39,57,41,36, 7,14,28, 0},
        {0}
    };

unsigned int mempartagee[NBFILS];

int main(int argc, char **argv)
{
    int numPil = 0;
    int numDat = 0;
    int nbFils = 0;
    int status  =0;
    cout << "oui oui c'est moi" << endl;

    while(tab[numPil][0] != 0)
    {
        cout << "IP    nbPil:"<<numPil+1<<endl;
        numDat=0;
        while(tab[numPil][numDat]!=0)
        {
            cout << "IP    nbDat:"<<tab[numPil][numDat]<<endl;
            //cout << "    je vais confier ça au fils:"<<numDat%NBFILS<<endl;
            
            if(nbFils>=NBFILS)
            {
                cout<<"IP    oula j'ai trop de fils ("<<nbFils<<"), j'attends un peu"<<endl;
                int fils_pid = wait(&status);
                if (WIFEXITED(status))
                {
                    cout << "IPT   le fils "<<fils_pid<<" s'est bien terminé, status:" << WEXITSTATUS(status) << endl;
                }
                if (WIFSIGNALED(status))
                {
                    cout << "EPT   le fils "<<fils_pid<<" s'est terminé avec un signal, status:" << WTERMSIG(status) << endl;
                }
                nbFils--;
            }
            pid_t pid = fork();
            if (pid == 0)
            {
                int attente = tab[numPil][numDat];
                cout<<"IFC   Je suis le fils '"<<getpid()<<"'/"<<numPil<<"/"<<numDat<<" et j'attends "<<attente<<"s"<<endl;
                sleep(attente);
                cout<<"IFT   Je suis le fils '"<<getpid()<<"'/"<<numPil<<"/"<<numDat<<" et j'ai terminé"<<endl;
                exit(1);
            }
            else
            {
                cout<<"IPC   je viens de créer le fils:"<<pid<<"/"<<nbFils<<endl;
                nbFils++;
            }
            numDat++;
        }
        numPil++;
    }

   /* pid_t pid = fork();



#ifdef MONTRERPB
#pragma message "Montrer le pb de l'exécution simultanée de deux process"
    if (pid == 0)
    {
        int i = 0;
        while (i < 10000)
        {
            cout << "FILS je suis un fils ->" << pid << endl;
            i++;
        }
    }
    else
    {
        int i = 0;
        while (i < 10000)
        {
            cout << "    PERE je suis le père " << pid << "!!" << endl;
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
    if (pid == 0)
    {
        //FILS
        cout << "FILS, moi je tourne (" << getpid() << ")" << endl;
#ifdef GENERESIGNAL
#pragma message "Génère un message d'erreur volontaire"
        *(int *)0 = 42; //on pointe volontairement dans une zone normalement innaccessible pour faire planter
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
        cout << "PERE je suis le père et j'attends le fils " << pid << "!!" << endl;
        wait(&status);
        if (WIFEXITED(status))
        {
            cout << "le fils c'est bien terminé, status:" << WEXITSTATUS(status) << endl;
        }
        if (WIFSIGNALED(status))
        {
            cout << "le fils c'est terminé avec un signal, status:" << WTERMSIG(status) << endl;
        }
    }
#endif
*/
    return 1;
}