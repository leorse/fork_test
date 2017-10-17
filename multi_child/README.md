Explications du traitement
======================

Ce traitement a pour vocation de montrer l'implémentation des **fork** afin de paralléliser un traitement.

Principe de l'exemple
-----------
L'exemple ainsi créé simule un traitement de lecture de fichiers d'entête et de fichiers de lignes. Le but de cet exemple étant de mettre en place un fils à chaque lecture d'une ligne, la lecture de l'entête étant gérée par le père.

**Attention :** il s'agit d'un exemple foncionnel, compilable sous linux et Solaris mais qui est perfectible.
Sa sémantique n'est pas cohérente (un coup on utilise des classe, un coup non, un coup il y a des cout, un coup des printf...).
Alors il faut bien noter que cet exemple à un but purement démonstratif de l'utilisation des fonctions fork.

Le traitement commence par deux choses importantes:

```c
#define NBFILS  1
```
Qui identifie de suite le nombre de fils que l'on veut utiliser dans cet exemple
```c
int tab[][20] = 
{
    {15,16,28,45,32,22,34, 7,9 ,27,11,36,17,14,26, 0},
    {25,26,38,55,24,32,44,17,19,37,21,16,51,54,76, 0},
    {35,36,48,15,7, 42,54,27,29,47,31,26,33,64,46, 0},
    {45,46,58,15,17,52,64,37,39,57,41,36, 7,14,28, 0},
    {0}
};
```
La définition de notre base de travail, celle sur laquelle va travailler notre traitement.

Classement des fils
-----------
L'un des fondements du fonctionnement des fork dans cet exemple est la gestion des fils. Chacun a un numéro qui lui est propre.
Une place qui lui est affecté au moment de sa création, et qui est libéré à la fin de sa vie. Celle-ci est donc
libre pour un prochain fils.
C'est la classe **ClasserFils** qui se charge de cela.



Écriture des journaux
-----------
Les journaux des différents fils sont gérés de manière à ne pas entrer en conccurence.
Chaque fils accède à son propre journal, le père de son côté a le sien.
```c
int journal(int nbJournal, const char* str, va_list arg)
int journalPere(const char* str, ...)
int initJournaux()
int fermerJournaux()
```

Mémoire partagée
-----------
Pour que les fork puissent accéder à un segment de mémoire partagé, il convient au préalable de créer ce segmentau préalable.
C'est dans la foncion creerSegment que cela se fait.
Un nom unique est créé avec la fonction **ftok**, puis le segment est créé avec la fonction **shmget**.
La clé de **ftok** est utilisée et la taille demandée est ainsi est allouée.

Cependant cette mémoire n'est pas accéssible à un process en y accédent comme n'importe quelle variable. Le process doit s'y attacher.
La fonction **shmat** permet de s'y attacher. Le pointeur renvoyé pointe sur le premier élément de cette mémoire.

```c
unsigned int* mem = (unsigned int*)shmat(shmid,(void*)0,flag) ;
if (mem ==(unsigned int*)-1)
{
    perror("attachement impossible") ;
    exit(1) ;
}
```
Dans cet exemple, **mem**
