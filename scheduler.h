#ifndef _SCHEDULER
#define _SCHEDULER
#define UNUSED(x) (void)(x)


#include "jobs.h"

extern int spid[100];
extern int queueSize;
int alterna;
int receive;


void alternaTarefa(int signum);
void scheduler_init(char* jobs, float quantum);




#endif
