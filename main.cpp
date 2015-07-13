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
			if (!strcmp(option, "--iteration") || !strcmp(shortOption, "-i")) {
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
			else if (!strcmp(option, "--objectref") || !strcmp(shortOption, "-or")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--classref") || !strcmp(shortOption, "-cr")) {
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
						"  --iteration x, -i x       uses x number as the number of iterations (default: 1000000)\n" \
						"  --thread x, -t x      	 uses x number as the number of threads (default: 10)\n" \
						"  --clazz x, -c x      	 uses x number as the number of classes (default: 300)\n" \
						"  --rootset x, -rs x      	 uses x number as the size of maximum rootset (default: 50)\n" \
						"  --payload x, -p x      	 uses x bytes as the maximum size of an object(default: 100)\n" \
						"  --longevity x, -l x       uses x number as the maximum of operations before an object dead (default: 10000)\n" \
						"  --escape x, -e x          uses x probability as the an object to be escaped (default: 20)\n" \
						"  --allocation x, -a x      uses x percent as the allocation (default: 2)\n" \
						"  --store x, -s x      	 uses x percent as the store (default: 10)\n" \
						"  --read x, -r x      		 uses x percent as the read (default: 88)\n" \
						"  --objectref x, -or x      uses x percent as the obj->obj reference (default: 90)\n" \
						"  --classref x, -cr x       uses x percent as the cls->obj reference (default: 10)\n" \
						);
		exit(1);
	}

		fprintf(stderr, "TraceFileGenerator v%.2f\n\n", VERSION);

		gDetLog = fopen("detailed.log","w+");

		char *filename  = argv[1];
		int iteration = setArgs(argc, argv, "--iteration",  "-i");
		int thread      = setArgs(argc, argv, "--thread",  "-t");
		int clazz       = setArgs(argc, argv, "--class",  "-c");
		int rootset     = setArgs(argc, argv, "--rootset",  "-rs");
		int payload     = setArgs(argc, argv, "--payload",  "-p");
		int longevity  = setArgs(argc, argv, "--longevity",  "-l");
		int escape      = setArgs(argc, argv, "--escape",  "-e");
		int allocation  = setArgs(argc, argv, "--allocation",  "-a");
		int read  		= setArgs(argc, argv, "--read",  "-r");
		int store  	    = setArgs(argc, argv, "--store",  "-s");
		int objectref  	= setArgs(argc, argv, "--objectref",  "-or");
		int clazzref  	= setArgs(argc, argv, "--classref",  "-cr");

		iteration = iteration <0 ? 1000000 : iteration;
		thread   = thread <0 ? 10 : thread;
		clazz 	 = clazz <0 ?  300 : clazz;
		rootset  = rootset <0 ? 50 : rootset;
		payload  = payload <0 ? 100 : payload;
		longevity= longevity <0 ? 10000 : longevity;
		escape   = escape <0 ? 20 : escape;
		allocation = allocation <0 ? 2 : allocation;
		read = read < 0 ? 80 : read;
		store = store <0 ? 10 : store;
		objectref = objectref <0 ? 90 : objectref;
		clazzref = clazzref <0 ? 10 : clazzref;

		fprintf(gDetLog, "TraceFileGenerator v-%.2f\n", VERSION);
		fprintf(gDetLog, "Number of iteration: %d\n", iteration);
		fprintf(gDetLog, "Number of threads: %d\n", thread);
		fprintf(gDetLog, "Number of classes: %d\n", clazz);
		fprintf(gDetLog, "Maximum rootset size: %d\n", rootset);
		fprintf(gDetLog, "Maximum payload size: %d\n", payload);
		fprintf(gDetLog, "Object's longevity: %d\n", longevity);
		fprintf(gDetLog, "Probability of escaped object: %d\n", escape);
		fprintf(gDetLog, "Ratio of allocation: %d\n", allocation);
		fprintf(gDetLog, "Ration of read access: %d\n", read);
		fprintf(gDetLog, "Ratio of store access: %d\n", store);
		fprintf(gDetLog, "Ratio of object->object reference in store access: %d\n", objectref);
		fprintf(gDetLog, "Ratio of class->object reference in store access: %d\n", clazzref);
		
		fclose(gDetLog);
		
	return 0;
}
