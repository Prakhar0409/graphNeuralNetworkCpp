#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef GAENGINE
#define GAENGINE

#define true 1
#define false 0

#define GA_SELECTION_INDEX 0
#define GA_SELECTION_FITNESS 1


//GA population element
typedef struct Element Element;
typedef struct GA GA;
typedef struct GAoutput GAoutput;


struct GAoutput {

  FILE *fileMain, *fileFits;
  char nameMain[128];
  char nameFits[128];
  int freqMain;
  int freqRst;
  
  int binary;
  int bestonly;

  int fileopen;
  
  int stepMain;
  int stepRst;
  
  int rstcnt;

};


//main GA engine
struct GA {

	int mpirank, mpisize;

	// *** POPULATION PARAMETERS ***
	unsigned long counter;
	
	int populationSize;
	Element *population;
	int alloc_pop;
	
	int immigrationSize;
	
	// *****************************
	
	// *** MIXING PARAMETERS *******
	
	/*! \brief Mutation occurence (per population) [0-1]. */
	double mutationRate;
	
	/*! \brief Maximum amount of DNA changed during a mutation event [0-1]. */
	double mutationAmount;
	
	/*! \brief Number of champions propagated to the next generation [0-populationSize]. */
	int keepbest;
	
	
	int selectionstyle;
	
	/*! \brief Exponential distribution factor for selection. */
	double lambda;
	
	
	double *cdf;
	
	// *****************************
	
	/*! \brief Pointer to the element custom setup function. */
	void (*element_setup)(Element *element, int randomise);

	/*! \brief Pointer to the element custom setup function. */
	void (*element_load)(Element *element);

	/*! \brief Pointer to the element custom mixing function. */
	void (*element_mix)(GA *engine, Element *mother, Element *father, Element *son);

	/*! \brief Pointer to the element custom mutation function. */
	void (*element_mutate)(Element *element, int amount);

	/*! \brief Pointer to the element custom cleaning function. */
	void (*element_clean)(Element *element);
	
	/*! \brief Pointer to the element evaluation function. */
	void (*element_evaluation)(GA *engine, Element *element);



	/*! \brief Pointer to the data for evaluation. */
	void *data;
	void *trainSet,*validationSet,*testSet;

	/*! \brief Generic string container, possibly for the name of a datafile. */
	char datafile[128];
	
	int nrecords;
	unsigned long *recordoffset;
	int minibatch;
	int minibatch_rnd;

	int restartLoads;

	GAoutput output;


	int lowmem;

	//int step;
	int inited;
};


extern GA* ga_new();
extern GA* ga_new_mpi();

int ga_compare_fitness (const void *aa, const void *bb);


void ga_clean(GA *engine);

void ga_init_cdf(GA *engine);
extern void ga_init_mpi(GA *engine);


int ga_input_getkeyline(FILE *fin, const char *key, char *string);
int ga_input_getvalue_int(FILE *fin, const char *key, int *value);
int ga_input_getvalue_dbl(FILE *fin, const char *key, double *value);
int ga_input_getvalue_chr(FILE *fin, const char *key, char *value);
extern void ga_readsettings(GA *engine, const char *filename);
extern int ga_readrestart_mpi(GA *engine, const char *rstfile);

extern void ga_settings_mpi(GA *engine);



void ga_elementlist_clean(GA *engine, Element *list, int size);
void ga_output_write(GA *engine);
void ga_output_write_txt(GA *engine);
void ga_output_write_bin(GA *engine);

extern void ga_setoutput_mpi(GA *engine, const char *filename);
extern void ga_output_close(GA *engine);

int ga_select(GA *engine, int except);
extern void ga_evaluate_mpi(GA *engine);
extern void ga_step_mpi(GA *engine);





#endif


