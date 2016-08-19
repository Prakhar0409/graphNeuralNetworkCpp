#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ga.h"
#include "element.h"
#include <mpi.h>



GA *ga_new() {

	GA *engine = (GA*)calloc(1, sizeof(GA));
	engine->mpirank = -1;
	engine->inited = false;
	
	engine->counter = 0;
	engine->populationSize = 10;
	engine->immigrationSize = 0;

	engine->element_evaluation = ga_element_evaluate_dummy;
	engine->element_setup = ga_element_setup_dummy;
	engine->element_load = ga_element_load_dummy;
	engine->element_mix = ga_element_mix_original;
	engine->element_mutate = ga_element_mutate_original;
	engine->element_clean = ga_element_clean_dummy;
	

	engine->mutationRate = 0.01;
	engine->mutationAmount = 0.1;
	engine->keepbest = 0;
	engine->selectionstyle = GA_SELECTION_INDEX;
	engine->lambda = 2.0;
	engine->selectionstyle = 0;
	
	engine->minibatch = 1;
	engine->minibatch_rnd = false;
	
	engine->lowmem = false;
	
	//output config
	engine->output.bestonly = false;
	engine->output.binary = true;
	engine->output.fileopen = false;
	engine->output.freqMain = 1;
	engine->output.freqRst = 1;	
	engine->output.stepMain = 0;
	engine->output.stepRst = 0;
	engine->output.rstcnt = 0;
	
	
	return engine;
}






GA *ga_new_mpi() {
	
	GA *engine = ga_new();
	MPI_Comm_rank(MPI_COMM_WORLD, &engine->mpirank);
	MPI_Comm_size(MPI_COMM_WORLD, &engine->mpisize);
	
	srand(time(NULL)+engine->mpirank);
	
	printf("GA[%i]: engine allocated.\n",engine->mpirank);
	
	MPI_Barrier(MPI_COMM_WORLD);
	return engine;
}




/*! \brief Broadcast the settings of engine rank 0 to all other MPI ranks.
 * 
 * If the engines are already initialised, the population size will not change,
 * and a warning will be shown.
 * 
 * \param engine is a reference to the GA engine.
 * */
void ga_settings_mpi(GA *engine) {
	
	printf("GA[%i]: sharing settings...\n", engine->mpirank);
	
	//engine properties
	int iprops[7] = {engine->populationSize, engine->immigrationSize, engine->selectionstyle, 
		engine->keepbest, engine->minibatch, engine->minibatch_rnd, engine->restartLoads};
	MPI_Bcast(iprops, 7, MPI_INT, 0, MPI_COMM_WORLD);
	if(engine->inited == false) {
		engine->populationSize = iprops[0];
	} else {
		printf("WARNING: ga_settings_mpi received a population size, but the engine is already initialised.\n");
	}
	engine->immigrationSize = iprops[1];
	engine->selectionstyle = iprops[2];
	engine->keepbest = iprops[3];
	engine->minibatch = iprops[4];
	engine->minibatch_rnd = iprops[5];
	engine->restartLoads = iprops[6];
	
	double dprops[3] = {engine->mutationRate, engine->mutationAmount, engine->lambda};
	MPI_Bcast(dprops, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	engine->mutationRate = dprops[0];
	engine->mutationAmount = dprops[1];
	engine->lambda = dprops[2];
	
	
	printf("GA[%i]: sharing settings done.\n", engine->mpirank);
	MPI_Barrier(MPI_COMM_WORLD);
}




/*! \brief Set the GA output filename and open the buffer.
 * 
 * This sets also the name of the restart file to filename.rst.
 * Only MPIrank 0 process will actually do this. If an output file was already open,
 * it will be closed and the new one created.
 * 
 * \param engine is a reference to the GA engine.
 * \param filename is a string with the new filename.
 * */
void ga_setoutput_mpi(GA *engine, const char *filename) {
	
	if(engine->mpirank <= 0) {
		
		printf("GA[%i]: setting output files...\n", engine->mpirank);
		
		if(engine->output.fileopen == true) {
			printf("GA: closing output files...\n");
			ga_output_close(engine);
		}
		
		sprintf(engine->output.nameMain, "%s",filename);
		sprintf(engine->output.nameFits, "%s.fit",filename);
		
		if(engine->output.binary == true) {
			engine->output.fileMain = fopen(engine->output.nameMain, "wb");
		} else {
			engine->output.fileMain = fopen(engine->output.nameMain, "w");
		}
		
		engine->output.fileFits = fopen(engine->output.nameFits, "w");
		engine->output.fileopen = true;
		printf("GA[%i]: setting output files done.\n", engine->mpirank); 
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
}

/*! \brief Close the output file streams.
 * 
 * This is not an MPI based function, so it will run regardless of the mpirank of the engine.
 * 
 * \param engine is a reference to the GA engine.
 * */
void ga_output_close(GA *engine) {
	
	engine->output.fileopen = false;
	
	if(engine->mpirank <= 0) {
		
		if(engine->output.fileMain != NULL)
			fclose(engine->output.fileMain);
			
		if(engine->output.fileFits != NULL)
			fclose(engine->output.fileFits);
		
		engine->output.fileopen = false;
		
	} else {
		
		engine->output.fileopen = false;
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
}



int ga_compare_fitness (const void *aa, const void *bb) {

  Element *a = (Element*)aa;
  Element *b = (Element*)bb;

  if(b->fitness > a->fitness) return 1;
  else if (b->fitness < a->fitness) return -1;
  else return 0;
}


/*! \brief Evaluates the proper chunk of elements and each engine rank.
 * 
 * After evaluation all ranks gather the fitnesses.
 * 
 * \param engine is a reference to the GA engine.
 * */
void ga_evaluate_mpi(GA *engine) {
	
	//each rank evaluates only a subset of the elements
	
	int chunksize = floor((double)engine->populationSize/engine->mpisize);
	int pstart = engine->mpirank*chunksize;
	int pend = pstart + chunksize;
	if(engine->mpirank == engine->mpisize-1) pend = engine->populationSize; //good enough!
	//pend = min(pend, engine->populationSize);

	printf("GA[%i] Evaluation: from %i to %i -- #%i\n",engine->mpirank,pstart,pend, pend-pstart);

	//setup gatherv parameters
	double fits[engine->populationSize];
	int recvcounts[engine->mpisize];
	int displs[engine->mpisize];
	
	for(int i=0;i<engine->mpisize;i++) {
		int ps = i*chunksize; 
		int pe = ps + chunksize; 
		if(i==engine->mpisize-1) pe = engine->populationSize; 
		recvcounts[i] = pe-ps; 
		displs[i] = ps; 
	}
	
	//evaluation loop
	for (int i=pstart; i<pend; i++) {
		//use the function pointed
		engine->element_evaluation(engine, &engine->population[i]);
		fits[i] = engine->population[i].fitness;
		//printf("mpi %i, elem %i, %lf\n",engine->mpirank, i, fits[i]);
	}
	printf("Hehreherherhe\n");
	//share fitnesses
	MPI_Allgatherv(&fits[pstart], (pend-pstart), MPI_DOUBLE, fits, recvcounts, displs, MPI_DOUBLE, MPI_COMM_WORLD);
	
	//printf();
	for (int i=0; i<engine->populationSize; i++) {
		engine->population[i].fitness = fits[i];
		//printf("%lf ",fits[i]);
	}
	
	printf("GA[%i]: evaluation completed.\n", engine->mpirank);
	MPI_Barrier(MPI_COMM_WORLD);
}


//returns the index of am element selected according to the breeding distribution.
int ga_select(GA *engine, int except) {
	
  int picked = except;
  while (picked == except) {
		
    double r = (double)rand()/RAND_MAX; //normal distro
    for (int i = 0; i < engine->populationSize; i++) {
      if (engine->cdf[i] >= r) {
	picked = i;
	break;
      }
    }
  }
	
  return picked;
}



void ga_step_mpi(GA *engine) {

	// each rank sorts the population... itz ok if the order is not exactly the same
	qsort(engine->population, engine->populationSize, sizeof(Element), ga_compare_fitness);
	//for(int i=0; i<engine->populationSize; i++)
	//  printf("fits[rank %i]: ID[%i] fit %lf \n", engine->mpirank, engine->population[i].ID, engine->population[i].fitness);

	printf("fits[rank %i]:  best=%lf  worst=%lf\n", engine->mpirank, 
		engine->population[0].fitness, engine->population[engine->populationSize-1].fitness);

	//printf("GA[%i]: calling write...\n", engine->mpirank);
	//write the output if needed
	ga_output_write(engine);
	//printf("GA[%i]: write done.\n", engine->mpirank);

	//do the mixing -- the first keepbest champions are not mixed!
	int chunksize = floor((double)engine->populationSize/engine->mpisize);
	int pstart = engine->mpirank*chunksize;
	int pend = pstart + chunksize;
	if(engine->mpirank == engine->mpisize-1) pend = engine->populationSize; //good enough!

	//WARNING: only ok for fixed DNA runs
	int dnasize = engine->population[0].dnasize;
	double *alldna = (double*)malloc(dnasize*engine->populationSize*sizeof(double)); //for all elements

	Element *parents = engine->population; //save the ref to the sorted population
	//make a new population
	engine->population = (Element*)calloc(engine->populationSize, sizeof(Element));
  
	//printf("GA[%i]: starting chunk %i %i.\n", engine->mpirank, pstart, pend);
	//process the chunk
	for(int i=pstart; i<pend; i++) {
		
		if( i<engine->keepbest ) {
			// copy this champion dna
			ga_element_create(engine, &engine->population[i], false); //create an empty element
			ga_element_load(engine, &engine->population[i], parents[i].dna, dnasize);
			
		} else {
		
			if(i >= engine->populationSize-engine->immigrationSize) { //make a new random immigrant
				ga_element_create(engine, &engine->population[i], true);
			} else { //do mixing
				//TODO: fitness based selection mode
				int p1 = ga_select(engine, -1);
				int p2 = ga_select(engine, p1);
				ga_element_mix(engine, &parents[p1], &parents[p2], &engine->population[i]);
			}
			
		}
		//serialise the element DNA
		memcpy(&alldna[i*dnasize], engine->population[i].dna, sizeof(double)*dnasize);
	}
	
	//clear the old population
	ga_elementlist_clean(engine, parents, engine->populationSize);

	//give the dna chunks to everybody
	//setup gatherv parameters
	int recvcounts[engine->mpisize];
	int displs[engine->mpisize];

	for(int i=0;i<engine->mpisize;i++) {
		int ps = i*chunksize;
		int pe = ps + chunksize;
		if(i==engine->mpisize-1) pe = engine->populationSize;
		recvcounts[i] = (pe-ps)*dnasize;
		displs[i] = ps*dnasize;
	}
  
	MPI_Allgatherv(&alldna[pstart*dnasize], (pend-pstart)*dnasize, MPI_DOUBLE, alldna, recvcounts, displs, MPI_DOUBLE, MPI_COMM_WORLD);
	//printf("mixing recevied @ %i  %i\n",engine->mpirank, ierr==MPI_SUCCESS);

	for(int i=0; i<engine->populationSize; i++) {
		if(i>=pstart && i<pend) continue; //do not reload the rank own elements
		// initialise an element
		ga_element_create(engine, &engine->population[i], false); //no randomisation needed
		ga_element_load(engine, &engine->population[i], &alldna[i*dnasize], dnasize);
	}
	//reset the global counter to avoid overflow
	engine->counter = 0;
	free(alldna);
	MPI_Barrier(MPI_COMM_WORLD);
}




void ga_elementlist_clean(GA *engine, Element *list, int size) {
	
	for(int i=0; i<size; i++) {
		ga_element_clean(engine, &list[i]); 
	}
	free(list);
}


void ga_clean(GA *engine) {
	
	//clean the population
	if(engine->inited == true) {
		ga_elementlist_clean(engine, engine->population, engine->populationSize);
		free(engine->cdf);
	}
	free(engine);
}




