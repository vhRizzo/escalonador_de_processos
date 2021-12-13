#include "scheduler.h"
#include "load_jobs.h"



void scheduler_init(char* jobs, float quantum){

	pidx = 0;
	alterna = 1;
	receive=0;	

	loadJobs(jobs);

	print_jobsQueue();

	int i;

	for(i = 0; i < queueSize; i++){
			iniciarProcesso(i);
			kill(spid[i], SIGSTOP);
	}		
	
	
	while(1){	
		  signal(SIGALRM, alternaTarefa);
		  alarm(quantum);
		  while(!receive)
			pause();
		  receive = 0; 
		  
    }


}


void alternaTarefa(int signum){	
	UNUSED(signum);

	receive = 1;	
	kill(spid[alterna], SIGSTOP);
	
	if(!alterna)
		alterna++;
	else
		alterna = 0;	
	
	kill(spid[alterna], SIGCONT);

}


