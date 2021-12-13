#ifndef _LOADJOBS
#define _LOADJOBS


#include "jobs.h"

extern char **fila;
extern char **argumentos;
extern int queueSize;

int get_escalonador (FILE *arq);
void loadJobs(char* file);
void print_jobsQueue();



#endif
