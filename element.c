#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ga.h"
#include "element.h"



void ga_element_create(GA *engine, Element *element, int randomise) {
	
	element->ID = ++engine->counter;
	element->fitness = 0;
	
	engine->element_setup(element, randomise);
	
}


void ga_element_setup_dummy(Element *element, int randomise) { }


/*! \!brief Copy the DNA array into the element dna and perform the custom load function.
 */
void ga_element_load(GA *engine, Element *element, double *dna, int size) {
	
	
	if(size != element->dnasize) {
		element->dna = (double*)realloc(element->dna, sizeof(double)*size);
	}
	element->dnasize = size;
	memcpy(element->dna, dna, size*sizeof(double));
	engine->element_load(element);
	
}

void ga_element_load_dummy(Element *element) { }


void ga_element_clean(GA *engine, Element *element) {
	
	engine->element_clean(element);
	
	if(element->dnasize > 0)
		free(element->dna);
	
}

void ga_element_clean_dummy(Element *element) { }



void ga_element_evaluate_dummy(GA *engine, Element *element) {
	element->fitness = (double)rand()/RAND_MAX;
}


void ga_element_mutate_original(Element *element, int amount) {
	
	double r;
	
	for(int i=0; i<amount; i++) {
		int idx = rand() % element->dnasize;
		r = (double)rand()/RAND_MAX;
		element->dna[idx] = (2*r-1);
	}
	
}


void ga_element_mix_original(GA *engine, Element *dad, Element *mom, Element *son) {

  	double r;

	//mixed dnas
	for(int i=0; i<dad->dnasize; i++) {
		r = (double)rand()/RAND_MAX;
		son->dna[i] = (r>0.5)? dad->dna[i] : mom->dna[i];
	}
	
	//mutate
	r = (double)rand()/RAND_MAX;
	if(r < engine->mutationRate) {
	
		int amount = rand() % (int)ceil(dad->dnasize*engine->mutationAmount);
		if(amount < 0) amount *= -1;
		
		engine->element_mutate(son, amount);
		
	}
	
	engine->element_load(son);
}


/*! \brief Mix two elements. The element must be eventually loaded with the custom function
 * at the end.
 * 
 * \param engine is a reference to the GA object.
 * \param dad is a reference to an element.
 * \param mom is a reference to the other element.
 * \param son is a pointer to the element storage for the result.
 * */
void ga_element_mix(GA *engine, Element *dad, Element *mom, Element *son) {
	
	//allocate the son
	ga_element_create(engine, son, false);
	
	//call the possily customised function instead
	engine->element_mix(engine, dad, mom, son);

}
