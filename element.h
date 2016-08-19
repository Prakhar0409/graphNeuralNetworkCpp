#include "ga.h"

#ifndef GAELEMENT
#define GAELEMENT

#define true 1
#define false 0


//GA population element
struct Element {
	
	unsigned long ID;
	//unsigned long *counter; //point to the engine one
	
	
	double *dna;
	int dnasize;
	
	double fitness;
	
	/// \brief Pointer for custom element info.
	void *custom;
	
};



void ga_element_create(GA *engine, Element *element, int randomise);
void ga_element_setup_dummy(Element *element, int randomise);

void ga_element_load(GA *engine, Element *element, double *dna, int size);
void ga_element_load_dummy(Element *element);

void ga_element_clean(GA *engine, Element *element);
void ga_element_clean_dummy(Element *element);

void ga_element_mutate_original(Element *element, int amount);

void ga_element_evaluate_dummy(GA *engine, Element *element);

void ga_element_mix_original(GA *engine, Element *dad, Element *mom, Element *son);
void ga_element_mix(GA *engine, Element *dad, Element *mom, Element *son);



#endif

