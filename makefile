all: broker
 
broker: main.o load_jobs.o jobs.o scheduler.o
	gcc -o broker main.o load_jobs.o jobs.o scheduler.o -lm
 
main.o: main.c jobs.h
	gcc -o main.o main.c -c -W -Wall -ansi -pedantic -lm
 
load_jobs.o: load_jobs.c load_jobs.h
	gcc -o load_jobs.o load_jobs.c -c -W -Wall -ansi -pedantic -lm

jobs.o: jobs.c jobs.h
	gcc -o jobs.o jobs.c -c -W -Wall -ansi -g -D_POSIX_C_SOURCE -std=c99 -pedantic  -lm

scheduler.o: scheduler.c scheduler.h
	gcc -o scheduler.o scheduler.c -c -W -Wall -ansi -g -D_POSIX_C_SOURCE -std=c99 -pedantic -lm
 
clean:
	rm -rf *.o *~ broker
	
