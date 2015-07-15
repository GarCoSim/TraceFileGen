/*
 * Logger.cpp
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */

#include "Logger.h"
#include <stdlib.h>
#include <string.h>

namespace traceGen {

Logger::Logger(char* tracepath) {
	//open File to log.
	trace = fopen(tracepath, "w");

	// dot file is not used, set null as default value
	dot = '\0';
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
//	printf("- T%d O%d\n",
//			thread,
//			root);
//	//fflush(stdout);
	fprintf(trace, "- T%d R%d\n",
			thread,
			root);
}

void Logger::deletefromRoot(int thread,int id){
//	printf("- T%d O%d\n",
//			thread,
//			id);
//	//fflush(stdout);
	fprintf(trace, "- T%d O%d\n",thread,id);
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

void Logger::logAllocation(int thread, int id, int size, int refCount, int classID){
	//	printf("a T%d O%d S%d N%d C%d\n",
	//			thread,
	//			id,
	//			size,
	//			refCount,
	//			classID);
		//fflush(stdout);
		fprintf(trace,"a T%d O%d S%d N%d C%d\n",
				thread,
				id,
				size,
				refCount,
				classID);
	//	fprintf(dot,"%d -> %d;\n", parent, id);
}

void Logger::logRefOperation(int thread, int parentID, int parentSlot, int childID){

//	printf("r T%d P%d #%d O%d\n",
//			thread,
//			parentID,
//			parentSlot,
//			childID);
	//fflush(stdout);
	fprintf(trace,"w T%d P%d #%d O%d\n",
			thread,
			parentID,
			parentSlot,
			childID);
	//fprintf(dot,"%d -> %d;\n", parentID, childID);
}

void Logger::logRefOperation(int thread, int parentID, int parentSlot, int childID, int fieldOffset, int fieldSize, int v){
	fprintf(trace,"w T%d P%d #%d O%d F%d S%d V%d\n",
			thread,
			parentID,
			parentSlot,
			childID,
			fieldOffset,
			fieldSize,
			v);

}



void Logger::logEnd(){
//	fprintf(dot,"}\n");
	//system("dot -Tpng gcKons.dot -o gcKons.png");
}


void Logger::logReadOperation(int thread, int id){
//	printf("r T%d O%d\n",
//			thread,
//			id);
//	//fflush(stdout);
	fprintf(trace, "r T%d O%d\n",
			thread,
			id);
}

void Logger::logRefOperationClaasToObject(int thread, int classID, int id){
//	printf("c T%d C%d O%d\n",
//			thread,
//			classID,
//			id);
//	//fflush(stdout);
	fprintf(trace, "c T%d C%d O%d\n",
			thread,
			classID,
			id);
}
void Logger::addToRoot(int thread,int id){
//	printf("+ T%d O%d\n",
//			thread,
//			id);
//	//fflush(stdout);
	fprintf(trace, "+ T%d O%d\n",
			thread,
			id);
}
Logger::~Logger() {
}

} /* namespace gcKons */
