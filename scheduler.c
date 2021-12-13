#include "scheduler.h"
#include "load_jobs.h"

bool checka_completo (int vet[]) {
	int i, cont;
	cont = 0;
	for (i = 0; i < queueSize ; i++) {
		if ( vet[i] <= 0 )
			cont++;
	}
	if ( cont == queueSize )
		return true;
	else
		return false;
}

void gera_ordem (int vet[], int dest[], int tipo) {
	if (tipo == 0) {
		int i, j, maior, pos_index;
		int aux[queueSize];
		pos_index = -1;
		for (i = 0; i < queueSize; i++)
			aux[i] = vet[i];
		for (i = queueSize - 1; i >= 0; i--) {
			maior = -1;
			for (j = 0; j < queueSize; j++) {
				if (aux[j] > maior) {
					maior = aux[j];
					pos_index = j;
				}
			}
			dest[i] = pos_index;
			aux[pos_index] = -1;
		}
	}
	if (tipo == 1) {
		int i, j, maior, pos_index;
		int aux[queueSize];
		pos_index = -1;
		for (i = 0; i < queueSize; i++)
			aux[i] = vet[i];
		for (i = 0; i < queueSize; i++) {
			maior = -1;
			for (j = 0; j < queueSize; j++) {
				if (aux[j] > maior) {
					maior = aux[j];
					pos_index = j;
				}
			}
			dest[i] = pos_index;
			aux[pos_index] = -1;
		}
	}
	if (tipo == 2) {
		int i, j, maior, pos_index;
		int aux[queueSize];
		pos_index = -1;
		for (i = 0; i < queueSize; i++)
			aux[i] = vet[i];
		for (i = queueSize - 1; i >= 0; i--) {
			maior = -1;
			for (j = 0; j < queueSize; j++) {
				if (aux[j] >= maior) {
					maior = aux[j];
					pos_index = j;
				}
			}
			dest[i] = pos_index;
			aux[pos_index] = -1;
		}
	}
}

int gera_rand (int min, int max) {
	srand(time(0));
	return (rand() % (max - min + 1)) + min;
}

void scheduler_init(char* jobs, float quantum) {

	pidx = 0;
	receive = 0;

	loadJobs(jobs);

	print_jobsQueue();

	int i, l;

	tempo = (int*) malloc ( queueSize * sizeof(int) );
	prior = (int*) malloc ( queueSize * sizeof(int) );

	for (l = 0; l < queueSize*2; l += 2) {
		tempo [l/2] = atoi (argumentos[l]);
		prior [l/2] = atoi (argumentos[l + 1]);
	}

	for(i = 0; i < queueSize; i++) {
		iniciarProcesso(i);
		kill(spid[i], SIGSTOP);
	}

	alterna = -1;

	switch (tipo_esc) {
		case 0:													/* Round Robin */
			round_robin(quantum);
			break;
		case 1:													/* First Come First Server: */
			fcfs();
			break;
		case 2:													/* Shortest Job First */
			sjf();
			break;
		case 3:													/* Shortest Job First - Approximation */
			sjf_apx();
			break;
		case 4:													/* Prioridade Estatica Preemptivo */
			pep();
			break;
	}
}

void round_robin (float quantum) {
	int tempo_rest[queueSize];
	int l;
	for (l = 0; l < queueSize; l++)
		tempo_rest[l] = tempo[l];

	printf ("\nRound Robin:\n\n");
	while(1) {
		if ( checka_completo(tempo_rest) ) {
			kill(spid[alterna], SIGSTOP);
			printf("\nRound Robin finalizado!\n\n");
			break;
		}
		signal(SIGALRM, alternaTarefa);
		if ( alterna < 0 )
			alarm(1);
		else {

			if( tempo_rest[alterna] <= 0 )
				alternaTarefa(0);

			else if ( tempo_rest[alterna] < quantum ) {
				alarm(tempo_rest[alterna]);
				tempo_rest[alterna] -= quantum;
			}

			else {
				alarm(quantum);
				tempo_rest[alterna] -= quantum;
			}
		}

		while(!receive) pause();
		receive = 0;
	}
}

void fcfs () {
	printf("\nFirst Come First Server:\n\n");
	while(1) {
		if (fcfs_aux == queueSize - 1) {
			kill(spid[alterna], SIGSTOP);
			printf("\nFirst Come First Server finalizado!\n\n");
			break;
		}
		signal(SIGALRM, alternaTarefa);
		if (alterna >= 0)
			alarm(tempo[alterna]);
		else
			alarm(1);
		while(!receive) pause();
		receive = 0;
	}
}

void sjf () {
	int ordem_exe[queueSize];
	int l = 0;
	gera_ordem (tempo, ordem_exe, 0);
	printf("\nShortest Job First:\n\n");
	while(1) {
		if (l == queueSize) {
			kill(spid[alterna], SIGSTOP);
			printf("\nShortest Job First finalizado!\n\n");
			break;
		}
		signal(SIGALRM, alternaTarefa);
		if (alterna < 0)
			alarm(1);
		else {
			if ( alterna != ordem_exe[l] )
				alternaTarefa(0);
			else {
				alarm(tempo[alterna]);
				l++;
			}
		}
		while(!receive) pause();
		receive = 0;
	}
}

void sjf_apx () {
	float fator = 0.5;
	int ult_tempo_exe[queueSize];
	int est[queueSize];

	int tempo_rest[queueSize];
	int ordem_exe[queueSize];
	int l;
	for (l = 0; l < queueSize; l++) {
		tempo_rest[l] = tempo[l];
		est[l] = 1;
	}
	gera_ordem(est, ordem_exe, 2);
	printf("\nShortest Job First - Approximation:\n\n");
	while(1) {
		if ( checka_completo(tempo_rest) ) {
			kill(spid[alterna], SIGSTOP);
			printf("\nShortest Job First - Approximation finalizado!\n\n");
			break;
		}
		signal(SIGALRM, alternaTarefa);
		if (alterna < 0)
			alarm(1);
		else {
			l = 0;
			gera_ordem(est, ordem_exe, 2);
			while ( tempo_rest[ordem_exe[l]] <= 0 )
				l++;
			if ( alterna != ordem_exe[l] )
				alternaTarefa(0);
			else {
				ult_tempo_exe[alterna] = gera_rand(1, tempo_rest[alterna]);
				tempo_rest[alterna] -= ult_tempo_exe[alterna];
				printf("%d - %d - %d\n", alterna + 1, ult_tempo_exe[alterna], tempo_rest[alterna]);
				alarm(ult_tempo_exe[alterna]);
				est[alterna] = (int) floor ( (double) (fator * ult_tempo_exe[alterna] + (1 - fator) * est[alterna]) );
			}
		}
		while(!receive) pause();
		receive = 0;
	}
}

void pep () {
	int ordem_exe[queueSize];
	int l = 0;
	gera_ordem (prior, ordem_exe, 1);
	printf("\nPrioridade Estatica Preemptivo:\n\n");
	while(1) {
		if (l == queueSize) {
			kill(spid[alterna], SIGSTOP);
			printf("\nPrioridade Estatica Preemptivo finalizado!\n\n");
			break;
		}
		signal(SIGALRM, alternaTarefa);
		if (alterna < 0)
			alarm(1);
		else {
			if ( alterna != ordem_exe[l] )
				alternaTarefa(0);
			else {
				alarm(tempo[alterna]);
				l++;
			}
		}
		while(!receive) pause();
		receive = 0;
	}
}

void alternaTarefa(int signum){
	UNUSED(signum);

	receive = 1;

	if(alterna < queueSize - 1){
		if (alterna >= 0) kill(spid[alterna], SIGSTOP);
		alterna++;
	}
	else {
		kill(spid[alterna], SIGSTOP);
		alterna = 0;
		fcfs_aux = queueSize - 1;
	}

	kill(spid[alterna], SIGCONT);
}
