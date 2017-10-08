#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdarg.h>

using namespace std;

#define NBFILS  52

int tab[][20] = 
{
    {15,16,28,45,32,22,34, 7,9 ,27,11,36,17,44,26, 0},
    {25,26,38,55,24,32,44,17,19,37,21,16,51,54,76, 0},
    {35,36,48,15,7, 42,54,27,29,47,31,26,33,64,46, 0},
    {45,46,58,15,17,52,64,37,39,57,41,36, 7,14,28, 0},
    {0}
};


unsigned int mempartagee[NBFILS];
FILE* journaux[NBFILS+1];

int initJournaux()
{
    for(int i =0;i<NBFILS+1;i++)
    {
        if(journaux[i]==NULL)
        {
            char nomfic[21];
            
            sprintf(nomfic, "fic_%02d", i);
            cout<<"fichier '"<<nomfic<<"' à créer"<<endl;
            journaux[i] = fopen(nomfic, "w");
        }
    }
    return 1;
}

int journal(int nbJournal, const char* str, va_list arg)
{
    vfprintf(journaux[nbJournal], str, arg);
    fflush(journaux[nbJournal]);
    return 1;
}

int journalPere(const char* str, ...)
{
    va_list arg;
    va_start(arg, str);
    journal(0, str, arg);
    va_end(arg);

    return 1;
}

int journalFils(int numFils, const char* str, ...)
{
    va_list arg;
    va_start(arg, str);
    journal(numFils+1, str, arg);
    va_end(arg);

    return 1;
}



int fermerJournaux()
{
    for(int i=0;i<NBFILS+1;i++)
    {
        if(journaux[i]!=NULL)
        {
            fclose(journaux[i]);
        }
    }

    return 1;
}

int main(int argc, char **argv)
{
    int numPil = 0;
    int numDat = 0;
    int nbFils = 0;
    int status  =0;
    
    memset(&journaux, 0, sizeof(FILE*)*(NBFILS+1));
    initJournaux();
    cout << "oui oui c'est moi" << endl;


    while(tab[numPil][0] != 0)
    {
        journalPere("IP    nbPil:%d\n",numPil+1);
        numDat=0;
        while(tab[numPil][numDat]!=0)
        {
            journalPere("IP    nbDat:%d\n",tab[numPil][numDat]);
            //cout << "    je vais confier ça au fils:"<<numDat%NBFILS<<endl;
            
            if(nbFils>=NBFILS)
            {
                journalPere("IP    oula j'ai trop de fils (%d), j'attends un peu\n", nbFils);
                int fils_pid = wait(&status);
                if (WIFEXITED(status))
                {
                    journalPere("IPT   le fils %d s'est bien terminé, status:%d\n", fils_pid, WEXITSTATUS(status));
                }
                if (WIFSIGNALED(status))
                {
                    journalPere("EPT   le fils %d s'est terminé avec un signal, status:%d\n", fils_pid, WTERMSIG(status));
                }
                nbFils--;
            }
            pid_t pid = fork();
            if (pid == 0)
            {
                int attente = tab[numPil][numDat];
                journalFils(nbFils, "IFC   Je suis le fils numéro %d, %d/%d/%d et j'attends %ds\n", nbFils, getpid(), numPil,numDat,attente);
                sleep(attente);
                journalFils(nbFils, "IFC   Je suis le fils numéro %d, %d/%d/%d et j'ai terminé\n", nbFils, getpid(), numPil,numDat);
                exit(1);
            }
            else
            {
                journalPere("IPC   je viens de créer le fils:%d/%d\n", pid, nbFils);
                nbFils++;
            }
            numDat++;
        }
        numPil++;
    }
    while(nbFils>0)
    {
        journalPere("IP    il me reste %d fils a attendre, j'attends un peu\n", nbFils);
        int fils_pid = wait(&status);
        if (WIFEXITED(status))
        {
            journalPere("IPT   le fils %d s'est bien terminé, status:%d\n", fils_pid, WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status))
        {
            journalPere("EPT   le fils %d s'est terminé avec un signal, status:%d\n", fils_pid, WTERMSIG(status));
        }
        nbFils--;
    }

    fermerJournaux();
    journalPere("EP    Traitement terminé\n");
    cout<<"Traitement terminé"<<endl;

    return 1;
}