#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ga.h"
#include "element.h"
#include <mpi.h>




int ga_input_getkeyline(FILE *fin, const char *key, char *string) {
	
	//rewind the file
	fseek(fin, 0, SEEK_SET);
	
	char line[256];
	char k[256], k2[256];
	
	while (fgets(line, sizeof(line), fin)) {
		
		sscanf(line, "%s %s\n", k,k2);
		if( strcmp(key,k) == 0 ) {
			
			//printf("key %s found = %s!\n",k,k2);
			strcpy(string, k2);
			
			return true;
		}
	}
	
	
	return false;
}

/*! \brief asd
 * 
 * \param fin is the file stream to parse.
 * \param key is the key string to look for.
 * \param value is a pointer to the storage for the key's value (if found).
 * \return true is the key was found, false otherwise.
 * */
int ga_input_getvalue_int(FILE *fin, const char *key, int *value) {
	
	char line[256];//, k[256];
	int v;
	
	int found = ga_input_getkeyline(fin, key, line);
	if (found == true) {
		
		sscanf(line, "%i\n", &v);
		*value = v;
		printf("GA input: setting %s to %i.\n", key, v);
		return true;
	} else {
		
		printf("GA input: key %s not found.\n", key);
		return false;
	}
}
int ga_input_getvalue_dbl(FILE *fin, const char *key, double *value) {
	
	char line[256];//, k[256];
	double v;
	
	int found = ga_input_getkeyline(fin, key, line);
	if (found == true) {
		
		sscanf(line, "%lf\n", &v);
		*value = v;
		printf("GA input: setting %s to %lf.\n", key, v);
		return true;
	} else {
		
		printf("GA input: key %s not found.\n", key);
		return false;
	}
}
int ga_input_getvalue_chr(FILE *fin, const char *key, char *value) {
	
	char line[256];//, k[256];
	char v[256];
	
	int found = ga_input_getkeyline(fin, key, line);
	if (found == true) {
		
		sscanf(line, "%s\n", v);
		strcpy(value, v);
		printf("GA input: setting %s to %s.\n", key, v);
		return true;
	} else {
		
		printf("GA input: key %s not found.\n", key);
		return false;
	}
}



void ga_readsettings(GA *engine, const char *filename) {
	
	int dorestart = 0;
	char rstfile[256];
	
	if(engine->mpirank <= 0) {
	
		printf("GA[%i]: reading settings file... %s\n", engine->mpirank, filename);
		FILE *fin = fopen(filename, "r");
			
		ga_input_getvalue_int(fin, "populationSize", &engine->populationSize);
		engine->restartLoads = engine->populationSize;
		ga_input_getvalue_int(fin, "immigrationSize", &engine->immigrationSize);
		
		ga_input_getvalue_int(fin, "keepbest", &engine->keepbest);
		ga_input_getvalue_int(fin, "selectionstyle", &engine->selectionstyle);
		ga_input_getvalue_dbl(fin, "mutationRate", &engine->mutationRate);
		ga_input_getvalue_dbl(fin, "mutationAmount", &engine->mutationAmount);
		ga_input_getvalue_dbl(fin, "lambda", &engine->lambda);
		
		ga_input_getvalue_int(fin, "output.bestonly", &engine->output.bestonly);
		ga_input_getvalue_int(fin, "output.binary", &engine->output.binary);
		ga_input_getvalue_int(fin, "output.freqMain", &engine->output.freqMain);
		ga_input_getvalue_int(fin, "output.freqRst", &engine->output.freqRst);
		
		ga_input_getvalue_int(fin, "minibatch", &engine->minibatch);
		ga_input_getvalue_int(fin, "minibatch_rnd", &engine->minibatch_rnd);

		ga_input_getvalue_int(fin, "restart", &dorestart);
		if(dorestart == 1) {
			ga_input_getvalue_chr(fin, "restartfile", rstfile);
			ga_input_getvalue_int(fin, "restartnload", &engine->restartLoads);
			printf("GA[%i]: restarting from %s\n", engine->mpirank, rstfile);
		}
		
		
		fclose(fin);
	}
	
	ga_settings_mpi(engine); //give settings to everyone
	
	MPI_Bcast(&dorestart, 1, MPI_INT, 0, MPI_COMM_WORLD); //broadcast the restart flag
	MPI_Bcast(rstfile, 256, MPI_CHAR, 0, MPI_COMM_WORLD); //broadcast the restart filename
	
	//read the restart
	if(dorestart == 1) {
		int goodrst = ga_readrestart_mpi(engine, rstfile);
		
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
}







int ga_readrestart_mpi(GA *engine, const char *rstfile) {
	
	
	FILE *rst = NULL;
	rst = fopen(rstfile, "rb");
	if(rst == NULL) {
		printf("GA[%i]: Read restart could not open the file!\n", engine->mpirank);
		perror("fopen");
		return false;
	}
	
	int psize = 0;
	fread(&psize, sizeof(int), 1, rst);
	if(psize > engine->populationSize) {
		printf("GA[%i]: WARNING! Restart population size is different... adjusting to %i.\n", engine->mpirank, psize);
		engine->populationSize = psize;
	}
	
	//init the population
	ga_init_mpi(engine);
	
	int dnasize = 0;
	fread(&dnasize, sizeof(int), 1, rst); //dna size
	if(dnasize != engine->population[0].dnasize) {
		printf("GA[%i]: ERROR! Restart dna size is different. Restart cannot be used!\n", engine->mpirank);
		printf("GA[%i]: dna size expected %i, restart %i!\n", engine->mpirank, engine->population[0].dnasize, dnasize);
		fclose(rst);
		return false;
	}
	
	//printf("GA[%i]: Read restart dnasize %i!\n", engine->mpirank, dnasize);
	
	double *tmpdna = (double*)malloc(sizeof(double)*dnasize);
	double *alldna = (double*)malloc(sizeof(double)*dnasize*psize);
	
	
	
	MPI_Barrier(MPI_COMM_WORLD);
	//each rank reads the population in the restart
	for(int i=0; i<psize; i++) {
		fread(&alldna[i*dnasize], sizeof(double), dnasize, rst); //dna
		fread(&engine->population[i].fitness, sizeof(double), 1, rst); //fitness
	}
	//if the population size is larger than in the restart, the extra elements will start at random
	fclose(rst);
	MPI_Barrier(MPI_COMM_WORLD);
	
	for(int i=0; i<engine->restartLoads; i++) {
		
		//printf("GA[%i]: reading rst for element %i\n", engine->mpirank, i);
		ga_element_load(engine, &engine->population[i], &alldna[i*dnasize], dnasize);
		//printf("GA[%i]: reading rst for element %i DONE.\n", engine->mpirank, i);
	}
	
	free(tmpdna); free(alldna);	
	
	printf("GA[%i]: Restart file loaded successfully!\n", engine->mpirank);
	MPI_Barrier(MPI_COMM_WORLD);
	return true;
}




