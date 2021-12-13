#include "load_jobs.h"

int get_escalonador (FILE *arq) {
  char str[100];
  fgets (str, 100, arq);
  str[strcspn(str, "\n")] = '\0';
  return atoi (str);
}

void loadJobs(char* file)
{



  int count=0,index=0,index2=0,l,c,k;

  FILE *arquivo;
  char job[10] = "";
  char ch;
  int args = 0;              /*contador para os argumentos*/

  int progXargs[100][100];


  fila = malloc(100);
  argumentos = malloc(100);


  arquivo = fopen(file, "r");


  for(l=0; l < 100; l++){
	for(c=0; c < 100; c++)
		progXargs[l][c] = 0;
  }

  tipo_esc = get_escalonador(arquivo);

  while((ch=fgetc(arquivo)) != EOF){
		if(ch != ' ' && ch != '\n'){
			job[count] = ch;
			count++;
		}else{
			printf("%s", job);
			if(job[0] == '.'){
				fila[index] = malloc(((size_t) strlen(job))*sizeof(char));
				strcpy(fila[index],job);
				memset(job, 0, 10);
				index++;
			}else if(strcmp(job, "")){
				argumentos[index2] = malloc(((size_t) strlen(job))*sizeof(char));
				strcpy(argumentos[index2],job);
				memset(job, 0, 10);
				progXargs[index-1][index2] = 1;
				index2++;
			}

			count = 0;
		}
  }


  for(k=0; k < index; k++)
		printf("\nProcesso %d: %s", k, fila[k]);

  printf("\n\n\n");


  for(l=0; l < index; l++){

		args = 0;

		for(c=0; c < index2; c++){
			if(progXargs[l][c] && strcmp(argumentos[c],""))
					args++;
		}
  }

  queueSize = index;

  fclose(arquivo);

}
