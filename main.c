#include "jobs.h"
#include "scheduler.h"



int main(int argc, char* argv[])
{

	if(argc != 2){
		printf("\n Erro de utilização \n\n Uso:\n\t ./broker 'lista_de_processos' \n\n"); 
		return 1;
	}

	

	scheduler_init(argv[1], 1);

	
	
    
	return 0;
}




