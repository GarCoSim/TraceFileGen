#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "ObjectClasses/MemoryManager.h"
#include "Utils/Simulator.h"


using namespace traceGen;
using namespace std;

FILE* gDetLog;


int NUM_THREADS ;
int clazz  ;
int ROOTSET_SIZE ;      
int longevity ; 
int escape ;    
int RATIO_ALLOC_SET; 
int readaccess ;
int storeaccess ;
int ROOT_DELETE_PROBABILITY; 	   
int ratiostaticfield;	

/*
int main(int argc, char *argv[]) {
	//default trace file name (and path).
	char filename[80] = "traceFile.trc";
	int iterations = 0;
	switch(argc){
	case 3:
		strcpy(filename,argv[2]);
	case 2:
		iterations = atoi(argv[1]);
		break;
	default:
		fprintf(stderr, "Wrong number of arguments provided\n\n");
		fprintf(stderr, "Usage: \t./traceGen numTraceFileLines\n<OR>\n"
				"\t./traceGen numTraceFileLines outputFilename\n");
		return EXIT_FAILURE;
	}


	Simulator* sim = new Simulator(filename);
	if(VERSION == 0){
		//printf("I am old version running \n");
		sim->run(iterations);
	}else{
		//printf("I am new version running \n");
		sim->runTraceFileGenerator(iterations);
	}
	//sim->test();
	return 0;
}
*/


int setArgs(int argc, char *argv[], const char *option, const char *shortOption) {
	int i;
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], option) || !strcmp(argv[i], shortOption)) {
			if (!strcmp(option, "--iterations") || !strcmp(shortOption, "-i")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--thread") || !strcmp(shortOption, "-t")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--class") || !strcmp(shortOption, "-c")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--rootset") || !strcmp(shortOption, "-rs")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--payload") || !strcmp(shortOption, "-p")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--longevity") || !strcmp(shortOption, "-l")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--escape") || !strcmp(shortOption, "-e")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--allocation") || !strcmp(shortOption, "-a")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--stroe") || !strcmp(shortOption, "-s")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--read") || !strcmp(shortOption, "-r")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--static") || !strcmp(shortOption, "-s")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--delrootset") || !strcmp(shortOption, "-d")) {
				return atoi(argv[i + 1]);
			}
			else{
				fprintf(stderr, "Some options have been set by default\n");
				return -1;
			}
		}
	}

	return -1;
}

int main(int argc, char *argv[]){
	
	if(argc < 2) {
		fprintf(stderr, "Usage: TraceFileGenerator traceFile [OPTIONS]\n" \
						"Options:\n" \
						"  The name of trace file (default: traceFile.trace)\n" \
						"  --iteration x, -i x       uses x number as the number of iterations (default: 100)\n" \
						"  --thread x, -t x      	 uses x number as the number of threads (default: 10)\n" \
						"  --clazz x, -c x      	 uses x number as the number of classes (default: 300)\n" \
						"  --rootset x, -rs x      	 uses x number as the size of maximum rootset (default: 50)\n" \
						"  --payload x, -p x      	 uses x bytes as the maximum size of an object(default: 100)\n" \
						"  --longevity x, -l x       uses x number as the maximum of operations before an object dead (default: 10000)\n" \
						"  --escape x, -e x          uses x probability as the an object to be escaped (default: 20)\n" \
						"  --allocation x, -a x      uses x percent as the allocation (default: 2)\n" \
						"  --storeaccess x, -s x      	 uses x percent as the store (default: 10)\n" \
						"  --readaccess x, -r x      		 uses x percent as the read (default: 88)\n" \
						"  --deleteroot x, -r x    uses x percent as the rootdelete (default: 10)\n" \
						"  --static x, -or x      uses x percent as the ratio of static field oepration (default: 30)\n" \
						);
		exit(1);
	}

		fprintf(stderr, "TraceFileGenerator v-%d\n\n", VERSION);

		gDetLog = fopen("detailed.log","w+");

		char *filename  = argv[1];
		char *clsfilename = argv[2];

		 int iterations = setArgs(argc, argv, "--iteration",  "-i");
		 NUM_THREADS      = setArgs(argc, argv, "--thread",  "-t");
		 clazz       = setArgs(argc, argv, "--class",  "-c");
		 ROOTSET_SIZE     = setArgs(argc, argv, "--rootset",  "-rs");
		 longevity  = setArgs(argc, argv, "--longevity",  "-l");
		 escape      = setArgs(argc, argv, "--escape",  "-e");
		 RATIO_ALLOC_SET = setArgs(argc, argv, "--allocation",  "-a");
		 readaccess  		= setArgs(argc, argv, "--read",  "-r");
		 storeaccess  	    = setArgs(argc, argv, "--store",  "-s");
		 ratiostaticfield = setArgs(argc, argv, "--static",  "-s");
		 ROOT_DELETE_PROBABILITY 	= setArgs(argc, argv, "--deleteaccess",  "-d");

		iterations = iterations <0 ? 100 : iterations;
		NUM_THREADS   = NUM_THREADS <0 ? 10 : NUM_THREADS;
		clazz 	 = clazz <0 ?  300 : clazz;
		ROOTSET_SIZE  = ROOTSET_SIZE <0 ? 50 : ROOTSET_SIZE;
		longevity= longevity <0 ? 10000 : longevity;
		escape   = escape <0 ? 20 : escape;
		RATIO_ALLOC_SET = RATIO_ALLOC_SET <0 ? 2 : RATIO_ALLOC_SET;
		readaccess = readaccess < 0 ? 80 : readaccess;
		storeaccess = storeaccess <0 ? 10 : storeaccess;
		ratiostaticfield = ratiostaticfield <0 ? 30 : ratiostaticfield;
		ROOT_DELETE_PROBABILITY = ROOT_DELETE_PROBABILITY <0 ? 10 : ROOT_DELETE_PROBABILITY;

		fprintf(gDetLog, "TraceFileGenerator v-%.2f\n", VERSION);
		fprintf(gDetLog, "Number of iteration: %d\n", iterations);
		fprintf(gDetLog, "Number of threads: %d\n", NUM_THREADS);
		fprintf(gDetLog, "Number of classes: %d\n", clazz);
		fprintf(gDetLog, "Maximum rootset size: %d\n", ROOTSET_SIZE);
		fprintf(gDetLog, "Maximum payload size: %d\n", MAX_PAYLOAD);
		fprintf(gDetLog, "Maximum reference slot in object&class: %d\n",  MAX_POINTERS);
		fprintf(gDetLog, "Object's longevity: %d\n", longevity);
		fprintf(gDetLog, "Probability of escaped object: %d\n", escape);
		fprintf(gDetLog, "Ratio of allocation: %d\n", RATIO_ALLOC_SET);
		fprintf(gDetLog, "Ration of read access: %d\n", readaccess);
		fprintf(gDetLog, "Ratio of store access: %d\n", storeaccess);

		fprintf(gDetLog, "Ratio of static field access: %d\n", ratiostaticfield);
		fprintf(gDetLog, "Ratio of rootsetdeletion: %d\n", ROOT_DELETE_PROBABILITY);

		Simulator* sim = new Simulator(filename);
		if(VERSION == 0){
			//printf("I am old version running \n");
			sim->run(iterations);
		}else{
			//printf("I am new version running \n");
			sim->initializeClassTable(clsfilename);
			sim->runTraceFileGenerator(iterations);
		}



		fclose(gDetLog);
		
	return 0;
}
