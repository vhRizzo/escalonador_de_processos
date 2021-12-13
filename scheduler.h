#ifndef _SCHEDULER
#define _SCHEDULER
#define UNUSED(x) (void)(x)

#include "jobs.h"

extern int spid[100];
extern int queueSize;
int alterna;
int receive;
int fcfs_aux = -1;
int *tempo;
int *prior;

bool checka_zero (int *vet);
void bubble_sort (int vetor[], int n);
void gera_ordem (int vet[], int dest[], int tipo);
void alternaTarefa(int signum);
void scheduler_init(char* jobs, float quantum);
void round_robin(float quantum);
void fcfs();
void sjf();
void sjf_apx();
void pep();

#endif
