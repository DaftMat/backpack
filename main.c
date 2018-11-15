/**	Mathis MARGOT - 21606171
 *	Project title : Backpack problem
 *	File : main.c
 *
 *	Decription : UNIVERSITY PARALLELISM EXERCISE
 *	-> Ahmdal's law study : what acceleration do we get with
 *		1 thread
 *		2 threads
 *		4 threads
 *		8 threads
 *	This main file will use the main of backpack.c as many times the user want it to run, and will print result in a proper grid.
 *
 *	My desktop :
 *		CPU		: IntelCore i7-7700K (4.2GHz, quad core | 8 threads)
 *		GPU		: GIGABYTE GeForce GTX 1070 (NVIDIA)
 *		MB 		: GIGABYTE Z270-Gaming K3
 *		RAM		: DDR4 16GB 2400MHtz
 *
 *	Compile :
 *	$ gcc -o exeName main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 8

int main(int argc, char *argv[]){
	if (argc != 3){
		fprintf(stderr,"Usage : %s file_name.txt num_executions\n", argv[0]);
		exit(9);
	}
	int numExec;
	sscanf(argv[2], "%d", &numExec);

	char *gcc[] = {"gcc","-o","backpack","-fopenmp","backpack.c", NULL};
	if (execv("/bin/gcc",gcc) == -1){
		perror("Compilation error");
		exit(99);
	}

	int numThread = 1;
	char *backpack[] = {"backpack",argv[1], "1", NULL};
	while (numThread < MAX_THREADS){
		for (int i = 0 ; i < numExec ; ++i){
			if (execv("./backpack",backpack) == -1){
				perror("Execution error");
				exit(98);
			}
		}
		numThread *= 2;
		backpack[3][0] = (char)numThread + '0';
	}
	return 0;
}