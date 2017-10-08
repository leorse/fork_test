#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdarg.h>

using namespace std;

#define NBFILS  8

int tab[][20] = 
{
    {15,16,28,45,32,22,34, 7,9 ,27,11,36,17,14,26, 0},
    //{25,26,38,55,24,32,44,17,19,37,21,16,51,54,76, 0},
    //{35,36,48,15,7, 42,54,27,29,47,31,26,33,64,46, 0},
    //{45,46,58,15,17,52,64,37,39,57,41,36, 7,14,28, 0},
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

//classe pour classer les fils selon un numéro réutilisable
class ClasserFils
{
private:
    pid_t liste[NBFILS];
public:
    ClasserFils(void);
    int rechercherPlace(void);
    void affecterPlace(pid_t pid, int place);
    void libererPlace(pid_t pid);
    void listerPlace(void);
};

ClasserFils::ClasserFils(void)
{
    memset(&this->liste, 0, sizeof(this->liste));
}

int ClasserFils::rechercherPlace(void)
{
    int i =0;
    while(i<NBFILS && liste[i]!=0)
    {
        i++;
    }
    if(i==NBFILS)
    {
        return -1;
    }
    return i;
}

void ClasserFils::affecterPlace(pid_t pid, int place)
{
    liste[place] = pid;
}

void ClasserFils::libererPlace(pid_t pid)
{
    int i =0;
    while(i<NBFILS && liste[i]!=pid)
    {
        i++;
    }
    if(i<NBFILS)
    {
        journalPere("EPT   le fils %d avait la place:%d\n", pid, i);
        liste[i] = 0;
    }
}

void ClasserFils::listerPlace(void)
{
    for(int i =0; i<NBFILS; i++)
    {
        journalPere("%03d: %5d - ", i+1, this->liste[i]);
        if( (i%8)==0 && i>0)
        {
            journalPere("\n");
        }
    }
    journalPere("\n");
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
    int numNouveauFils=0;

    ClasserFils cf;
    
    memset(&mempartagee, 0, sizeof(mempartagee));
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
                journalPere("IP    libération de la place pour le fils:%d\n", fils_pid);
                cf.libererPlace(fils_pid);
                nbFils--;
            }
            numNouveauFils = cf.rechercherPlace();
            journalPere("IP    une place pour un fils:%d\n", numNouveauFils);
            pid_t pid = fork();
            if (pid == 0)
            {
                int attente = tab[numPil][numDat];
                journalFils(numNouveauFils, "IFC   Je suis le fils numéro %d, %d/%d/%d et j'attends %ds\n", numNouveauFils, getpid(), numPil,numDat,attente);
                sleep(attente);
                mempartagee[numNouveauFils] += attente;
                journalFils(numNouveauFils, "IFC   Je suis le fils numéro %d, %d/%d/%d et j'ai terminé\n", numNouveauFils, getpid(), numPil,numDat);
                exit(1);
            }
            else
            {
                cf.affecterPlace(pid, numNouveauFils);
                journalPere("IPC   je viens de créer le fils:%d/%d\n", pid, numNouveauFils);
                nbFils++;
            }
            numDat++;
        }
        numPil++;
    }
    while(nbFils>0)
    {
        journalPere("IP    il me reste %d fils a attendre, j'attends un peu\n", nbFils);
        cf.listerPlace();
        int fils_pid = wait(&status);
        if (WIFEXITED(status))
        {
            journalPere("IPT   le fils %d s'est bien terminé, status:%d\n", fils_pid, WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status))
        {
            journalPere("EPT   le fils %d s'est terminé avec un signal, status:%d\n", fils_pid, WTERMSIG(status));
        }
        cf.libererPlace(fils_pid);
        nbFils--;
    }
    for(int i =0;i<NBFILS;i++)
    {
        journalPere("%02d: %3d - ", i, mempartagee[i]);    
    }
    journalPere("\n"); 
    journalPere("EP    Traitement terminé\n");
    fermerJournaux();
    
    cout<<"Traitement terminé"<<endl;

    return 1;
}