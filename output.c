#include "ga.h"
#include "element.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "time.h"
#include <math.h>
#include <mpi.h>


void ga_output_write(GA *engine) {
	
	//only the main rank prints the output
	if(engine->mpirank == 0) {
	
		int dnasize = engine->population[0].dnasize;
		
		//write the restart file - binary
		if(engine->output.stepRst == engine->output.freqRst) {
			//printf("GA[%i]: writing restart...\n", engine->mpirank);
			char fn[256];
			sprintf(fn, "%s.%i.rst", engine->output.nameMain, engine->output.rstcnt);
			FILE *rst = NULL;
			rst = fopen(fn, "w");
			if(rst == NULL) {
				printf("GA[%i]: FILE OPEN FAILED!\n", engine->mpirank);
				perror("fopen");
			}
			//printf("GA[%i]: restart file open... %s\n", engine->mpirank,fn);
			fwrite(&engine->populationSize, sizeof(int), 1, rst); //population size
			//printf("GA[%i]: restart header written1.\n", engine->mpirank);
			fwrite(&dnasize, sizeof(int), 1, rst); //dna size
			//printf("GA[%i]: restart header written2.\n", engine->mpirank);
			for(int i=0; i<engine->populationSize; i++) {
				//printf("GA[%i]: restart writing element %i...\n", engine->mpirank, i);
				fwrite(engine->population[i].dna, sizeof(double), dnasize, rst); //dna
				fwrite(&engine->population[i].fitness, sizeof(double), 1, rst); //fitness
			}
			//printf("GA[%i]: restart info written.\n", engine->mpirank);
			fclose(rst);
			engine->output.stepRst = 0;
			engine->output.rstcnt = ((engine->output.rstcnt+1) % 2);
			//printf("GA[%i]: restart done.\n", engine->mpirank);
		}
		
		//print the full output
		if(engine->output.stepMain == engine->output.freqMain) {
			
			//printf("GA[%i]: writing full output...\n", engine->mpirank);
			
			if(engine->output.binary == true)
				ga_output_write_bin(engine);
			else
				ga_output_write_txt(engine);
			
			engine->output.stepMain = 0; //reset the counter
			
			//printf("GA[%i]: writing full output done.\n", engine->mpirank);
		}
		
		
		//write best and worst fitness
		fprintf(engine->output.fileFits, "Generation best\t%lf\tworst\t%lf\n",engine->population[0].fitness, engine->population[engine->populationSize-1].fitness);
		fflush(engine->output.fileFits);
		
		engine->output.stepMain++;
		engine->output.stepRst++;
	}
	
	//printf("GA[%i]: write done.... waiting...\n", engine->mpirank);
	MPI_Barrier(MPI_COMM_WORLD);
}

void ga_output_write_txt(GA *engine) {
	
	FILE *fout = engine->output.fileMain;
	
	int dnasize = engine->population[0].dnasize;
	int stop = (engine->output.bestonly==true)? 1 : engine->populationSize;
	fprintf(fout,"generation: %i\n",stop);
	fprintf(fout,"dnasize: %i\n",dnasize);
	//printf("ASDASDASDASDASD\n");
	for(int i=0; i<stop; i++) {
		fprintf(fout, "DNA:\n");
		for(int j=0; j<dnasize; j++)
			fprintf(fout, "%lf ",engine->population[i].dna[j]);
		
		fprintf(fout, "\nfitness: %lf\n", engine->population[i].fitness); //fitness
	}
	
}

void ga_output_write_bin(GA *engine) {
	
	FILE *fout = engine->output.fileMain;
	
	int dnasize = engine->population[0].dnasize;
	int stop = (engine->output.bestonly==true)? 1 : engine->populationSize;
	fwrite(&stop, sizeof(int), 1, fout); // # of printed elements
	fwrite(&dnasize, sizeof(int), 1, fout); //dna size
	
	for(int i=0; i<stop; i++) {
		fwrite(engine->population[i].dna, sizeof(double)*dnasize, 1, fout); //dna
		fwrite(&engine->population[i].fitness, sizeof(double), 1, fout); //fitness
	}
	
}



