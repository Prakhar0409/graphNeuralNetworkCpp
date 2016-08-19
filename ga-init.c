#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "time.h"
#include "ga.h"
#include "element.h"
#include <mpi.h>




//make the CDF for selection, called by init
void ga_init_cdf(GA *engine) {
  
  //printf("GA[%i]: cdf 0\n", engine->mpirank);
  
  engine->cdf = (double*)malloc(sizeof(double)*engine->populationSize);
  double *pdf = (double*)malloc(sizeof(double)*engine->populationSize);
  double *cdf = engine->cdf;
  //printf("GA[%i]: cdf 1\n", engine->mpirank);
  double area = 0;
  
  for(int i=0; i<engine->populationSize; i++) {
    pdf[i] = exp(-engine->lambda * (double)i / engine->populationSize);
    area += pdf[i];
  }
  //printf("GA[%i]: cdf 2\n", engine->mpirank);
  for(int i=0; i<engine->populationSize; i++) {
    pdf[i] /= area;
    cdf[i] = pdf[i];
  }
  //printf("GA[%i]: cdf 3\n", engine->mpirank);
  for(int i=1; i<engine->populationSize; i++)
    cdf[i] += cdf[i-1];

	//printf("GA[%i]: cdf 4\n", engine->mpirank);

	free(pdf);
	//printf("GA[%i]: cdf computed.\n", engine->mpirank);
}


void ga_init_mpi(GA *engine) {

	printf("GA[%i]: initialising...\n", engine->mpirank);

	if( engine->inited == true ) {
		printf("GA[%i]: system already initialised!\n", engine->mpirank);
		return;
	}

	

	ga_init_cdf(engine);
	//everybody allocates population array
	engine->population = (Element*)calloc(engine->populationSize, sizeof(Element));

	int dnasize;

	//everybody creates a random population - but the rank 0 wins! :)
	for(int i=0; i<engine->populationSize; i++) {
		ga_element_create(engine, &engine->population[i], true);
	} // end of OMP for
	//this way everyone sets up elements properly
	printf("GA engine [%i] random population done.\n", engine->mpirank);


	//global dna collector
	dnasize = engine->population[0].dnasize; 
	double *alldna = (double*)malloc(dnasize*engine->populationSize*sizeof(double));


	if(engine->mpirank == 0) {
		//main rank 0 collects the dna of everyone
		for(int i=0; i<engine->populationSize; i++)
			memcpy(&alldna[i*dnasize], engine->population[i].dna, sizeof(double)*dnasize);
	}

	//send it to everyone with Broadcast - rank 0 is the sender
	MPI_Bcast(alldna, dnasize*engine->populationSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	//now the other ranks load the dna 
	if(engine->mpirank != 0) {
		for(int i=0; i<engine->populationSize; i++) {
			ga_element_load(engine, &engine->population[i], &alldna[i*dnasize], dnasize);
		}
	}
	
	free(alldna);
	engine->inited = true;

	printf("GA engine [%i] initialised.\n", engine->mpirank);
	MPI_Barrier(MPI_COMM_WORLD);
}


