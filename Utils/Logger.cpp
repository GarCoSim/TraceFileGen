/*
 * Logger.cpp
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */

#include "Logger.h"
#include <stdlib.h>

namespace traceGen {

Logger::Logger(char* tracepath) {
	//open File to log.
	trace = fopen(tracepath, "w");
	//dot = fopen("gcKons.dot", "w");
	//fprintf(dot,"digraph G {\n");
}

void Logger::logAllocationRoot(int thread, int rootsetIndex, int id, int size, int refCount){
//	printf("a T%d R%d O%d S%d N%d\n",
//			thread,
//			rootsetIndex,
//			id,
//			size,
//			refCount);
//	//fflush(stdout);
	fprintf(trace,"a T%d R%d O%d S%d N%d\n",
			thread,
			rootsetIndex,
			id,
			size,
			refCount);
	//fprintf(dot,"T%d -> %d;\n",thread, id);
}

void Logger::deleteRoot(int thread,int root){
	fprintf(trace, "- T%d R%d\n",thread,root);
}

void Logger::logAllocation(int thread, int parent, int parentSlot, int id, int size, int refCount){
//	printf("a T%d P%d #%d O%d S%d N%d\n",
//			thread,
//			parent,
//			parentSlot,
//			id,
//			size,
//			refCount);
	//fflush(stdout);
	fprintf(trace,"a T%d P%d #%d O%d S%d N%d\n",
			thread,
			parent,
			parentSlot,
			id,
			size,
			refCount);
//	fprintf(dot,"%d -> %d;\n", parent, id);
}

void Logger::logRefOperation(int thread, int parentID, int parentSlot, int childID){

//	printf("r T%d P%d #%d O%d\n",
//			thread,
//			parentID,
//			parentSlot,
//			childID);
	//fflush(stdout);
	fprintf(trace,"r T%d P%d #%d O%d\n",
			thread,
			parentID,
			parentSlot,
			childID);
	//fprintf(dot,"%d -> %d;\n", parentID, childID);
}

void Logger::logEnd(){
//	fprintf(dot,"}\n");
	//system("dot -Tpng gcKons.dot -o gcKons.png");
}

Logger::~Logger() {
}

} /* namespace gcKons */
