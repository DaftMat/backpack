/**	Mathis MARGOT - 21606171
 *	Project title : Backpack problem
 *	File : backpack.c
 *
 *	Decription : UNIVERSITY PARALLELISM EXERCISE
 *	-> Ahmdal's law study : what acceleration do we get with
 *		1 thread
 *		2 threads
 *		4 threads
 *		8 threads
 *
 *	My desktop :
 *		CPU		: IntelCore i7-7700K (4.2GHz, quad core | 8 threads)
 *		GPU		: GIGABYTE GeForce GTX 1070 (NVIDIA)
 *		MB 		: GIGABYTE Z270-Gaming K3
 *		RAM		: DDR4 16GB 2400MHtz
 *
 *	Compile :
 *	$ gcc -o exeName -fopenmp backpack.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define MAX_SIZE 6404
#define MAX_WEIGHT 48

int main(int argc, char **argv){
	//study data
	double start,stop;
	double startSeq = start = omp_get_wtime();//Starts chrono for squential part
	double stopSeq, startPar, stopPar;
	double totalSeq = 0.0,totalPar = 0.0,totalTime = 0.0;
	int numThreads;

	//algo data
	int nbObj=0, cpBag=0;
	char cursor;
	int m[MAX_SIZE], u[MAX_SIZE], s[MAX_WEIGHT][MAX_SIZE], e[MAX_SIZE];
	int maxUtility = 0;
	int nbInBag = 0;

	if (argc != 3){
		fprintf(stderr, "Usage : %s file_name.txt num_threads\n", argv[0]);
		exit(8);
	}

	sscanf(argv[2],"%d",&numThreads);

	FILE *f;//open file to read data.
	if ((f = fopen(argv[1], "r")) == NULL){
		perror("File error");	//check if the file exists
		exit(9);
	}

	//Get data from file
	while (fscanf(f, "%c",&cursor) != EOF){
		if (cursor == 'c'){
			fgetc(f);
			fscanf(f, "%d", &cpBag);
			fgetc(f);
		}else if (cursor == 'o'){
			fgetc(f);
			fscanf(f, "%d", &m[nbObj]);
			fgetc(f);
			fscanf(f, "%d", &u[nbObj]);
			nbObj++;
			fgetc(f);
		}else{
			perror("Bad file content design");
			exit(88);
		}
	}
	fclose(f);

	//Getting in the parallel part
	stopSeq = omp_get_wtime();
	totalSeq += (stopSeq-startSeq);
	startPar = omp_get_wtime();
	//Algorithm finding s
	#pragma omp parallel num_threads(numThreads)
	{
		//first line (s[0][j])
		#pragma omp for
		for (int i = 0 ; i < cpBag+1 ; ++i){
			(m[0] <= i) ? (s[0][i] = u[0]) : (s[0][i] = 0);
		}

		//other lines
		for (int i = 1 ; i < nbObj ; ++i){
			#pragma omp for
			for (int j = 0 ; j < cpBag+1 ; ++j){
				if (m[i] <= j){
					int utility1 = u[i]+s[i-1][j-m[i]];
					int utility2 = s[i-1][j];
					if (utility1 > utility2){	//compare utilities and get the max
						s[i][j] = utility1;
					}else{
						s[i][j] = utility2;
					}
				}else{
					s[i][j] = s[i-1][j];
				}
			}
		}
	}
	stopPar = startSeq = omp_get_wtime();
	totalPar = (stopPar-startPar);

	maxUtility = s[nbObj-1][cpBag];

	//find list of objects to take
	int current = s[nbObj-1][cpBag],k = nbObj-2,h = cpBag;
	while (h >= 0 && k >= 0){
		if (s[k][h] != current){
			e[k+1] = 1;
			h -= m[k+1];
			if (h >= 0 && k >= 0)	current = s[k][h];
		}
		k--;
	}


	printf("Max utility for a capacity of %d => %d\n",cpBag,maxUtility);
	printf("Objects : ");
	for (int i = 0 ; i < nbObj ; ++i){
		printf(" %d",e[i]);
	}
	printf("\n");

	stopSeq = stop = omp_get_wtime();
	totalSeq += (stopSeq-startSeq);
	totalTime = (stop-start);
	printf("Study result for %d threads :\n > Sequential part\t: %lf\n > Parallel part\t: %lf\n > Total\t: %lf\n",numThreads,totalSeq,totalPar,totalTime);
	return 0;
}