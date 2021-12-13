
#include "jobs.h"
#include <unistd.h>




void print_jobsQueue(){
	int ind;	
	
	printf("\n\n PROCESSOS NA FILA! \n\n");
	for(ind = 0; ind < queueSize; ind++)
		printf(" %s ", fila[ind]);

}


void iniciarProcesso(int index){

	pid_t pid;	

	pid = fork();

	if(pid == 0){
		execlp(fila[index], argumentos[index],(char *)NULL);
		kill(pid, SIGSTOP); 
		

	}else{
		printf("spid [%d] = %d \n",pidx, spid[pidx]);
		spid[pidx] = pid;
		pidx++;	

	}
	
	
}
