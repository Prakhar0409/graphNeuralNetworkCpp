#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ga.h"
#include "element.h"
#include <mpi.h>
#include "params.h"

int trainSetSize,validationSetSize,testSetSize;

extern double eval(void* d,int dataSize,double* params,int paramSize,int printOut);	 //evaluates and returns a cost 
extern void reader(void** data,int* trainSetSize,void** validationSet,int* validationSetSize,void** testSet,int* testSetSize,int stateDim);	//reads trainset molecules & puts them into data 
void printTestCase(void* d,int dataSize);
extern void foo();	// TEST DEBUG FUNCTION


void element_setup(Element *element, int randomise) {
	element->dnasize = 95;//NUMPARAMSF + NUMPARAMSO;
	element->dna = (double*) calloc(element->dnasize, sizeof(double));
	if(randomise == 1) {
		for(int i=0; i<element->dnasize; i++)
			element->dna[i] = 2*((double)rand()/RAND_MAX)-1;	
	}
}

void element_load(Element *element) {
	// this is used to put the dna parameters into the custom object of the elemnent (which was neural net in an example shown to you) -- Not used in this program
	
}
void element_clean(Element *element) {
		// clean the memory after the element dies. dna is just an array of doubles and the custom void pointer also needs to be cleaned if used.	
}

void element_evaluate(GA *engine, Element *element) {
	void *data = engine->data;	//pointer to start of the engine data for eval
	int paramLen = element->dnasize;
	double* params = element->dna;
	// (double*) malloc(paramLen * sizeof(double));
	// int i=0;
	// for(i=0;i<paramLen;i++){
		// params[i] = element->dna[i];
	// }
	
	// printf("HEHRHEHRHEHRHEHRHHEHHER\n");
	element->fitness = 0;	
	double fitness=-1;
	fitness = eval(data,trainSetSize,params,paramLen,0);	
	// printf("BLABCLALCBLC\n");
	printf("fitness => %lf\n",fitness);
	// free(params);
	element->fitness = fitness;
}

int main(int argc, char **argv) {
	int mpierr = MPI_Init(&argc, &argv);

	int mpiID, mpiNP;
	mpierr = MPI_Comm_rank(MPI_COMM_WORLD, &mpiID);	
	mpierr = MPI_Comm_size(MPI_COMM_WORLD, &mpiNP);	
	
	printf("Rank: %d and size: %d\n",mpiID,mpiNP);
	GA *engine = ga_new_mpi();		
	engine->element_setup = element_setup;	
	engine->element_load = element_load;	
	engine->element_evaluation = element_evaluate;
	
	//defaults if not in ga.in
	engine->populationSize = 1024;	
	engine->immigrationSize = 10;	//crossover rate
	engine->mutationRate = 0.1;
	engine->mutationAmount=0.5;
	engine->output.binary = 0;
	engine->output.bestonly = 1;
	engine->keepbest = 1;

	//read defined settings
	ga_readsettings(engine, "ga.in");
	
	// data for evaluation - 
	void* data = NULL;	//void pointer
	void *trainSet,*validationSet,*testSet;
	reader(&data,&trainSetSize,&validationSet,&validationSetSize,&testSet,&testSetSize,4);	//passed address to pointer pointer
	engine->data = data;
	if(data == NULL){
		printf("No data read. Probable error in reader.\n");
		return 0;
	}
	
	ga_settings_mpi(engine);	// This is already called by ga_readsettings 
					// Also no need to share data since u r already reading it for every node

	ga_setoutput_mpi(engine, "st.out");	//setting output file on ga[0];
	
	ga_init_mpi(engine);		//random population generated on every process but trasferred from proc 0 to all
					// so prints initialising ga[]; random population generated on ga[]; ga engine initialized

//	foo();
	
	//training!
	// printf("TRAINING BEGINS\n");
	ga_evaluate_mpi(engine);
	// printf("BHJWBDHGVXGHBS XHGVSXBJH\n");
	for(int gen=0; gen<6; gen++) {
		if(mpiID == 0){
			printf("ITERATION(GENERATION) NUMBER: %d: ",gen);
		}
		ga_step_mpi(engine);
		ga_evaluate_mpi(engine);
	}


	if(mpiID==0){	
		printf("\n**********************TRAINGING ENDS***************************\n\n");
		
		//test
		qsort(engine->population, engine->populationSize, sizeof(Element), ga_compare_fitness);
		//now population[0] has the best fitness dna therefore the best parameters
		printf("Best Fitness : %lf\n",engine->population[0].fitness);
		double fitness = eval(data,trainSetSize,engine->population[0].dna,engine->population[0].dnasize,1);	
		printf("Fitness on training data : %lf\n",fitness);
	}


	ga_clean(engine);
	mpierr = MPI_Finalize();
	return 0;
}

