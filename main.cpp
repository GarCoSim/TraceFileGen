#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "ObjectClasses/MemoryManager.h"
#include "Utils/Simulator.h"


using namespace traceGen;
using namespace std;


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
		printf("I am old version running \n");
		sim->run(iterations);
	}else{
		printf("I am new version running \n");
		sim->runTraceFileGenerator(iterations);
	}
	//sim->test();
	return 0;
}
