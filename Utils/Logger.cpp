/*
 * Logger.cpp
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */

#include "Logger.h"
#include <stdlib.h>
#include <string.h>

extern int na;
extern int ar;
extern int dr;
extern int sRo;
extern int sRc;
extern int sPo;
extern int sPc;
extern int rRo;
extern int rRc;
extern int rPo;
extern int rPc;


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
	dr++;
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
	na++;
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
	sRo++;
}


void Logger::logreadRefFromObjectOperation(int thread, int parentID, int parentSlot, int fieldSize, int v){
	fprintf(trace,"r T%d O%d I%d S%d V%d\n",
			thread,
			parentID,
			parentSlot,
			fieldSize,
			v);
	rRo++;
}



void Logger::logstoreObjFieldWithPrimOperation(int thread, int parentID, int fieldOffset, int fieldSize, int v ){
	fprintf(trace,"s T%d P%d F%d S%d V%d\n",
			thread,
			parentID,
			fieldOffset,
			fieldSize,
			v);
	sPo++;
}


void Logger::logstoreClassFieldWithPrimOperation(int thread, int classID, int fieldOffset, int fieldSize, int v ){
	fprintf(trace,"s T%d C%d F%d S%d V%d\n",
			thread,
			classID,
			fieldOffset,
			fieldSize,
			v);
	sPc++;
}

void Logger::logreadClassFieldWithPrimOperation(int thread, int classID, int fieldOffset, int fieldSize, int v ){
	fprintf(trace,"r T%d C%d F%d S%d V%d\n",
			thread,
			classID,
			fieldOffset,
			fieldSize,
			v);
	rPc++;
}



void Logger::logreadObjFieldWithPrimOperation(int thread, int parentID, int fieldOffset, int fieldSize, int v ){
	fprintf(trace,"r T%d O%d F%d S%d V%d\n",
			thread,
			parentID,
			fieldOffset,
			fieldSize,
			v);
	rPo++;
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

void Logger::logRefOperationClaasToObject(int thread, int classID, int slotOffset, int objId, int fieldSize, int v){
//	printf("c T%d C%d O%d\n",
//			thread,
//			classID,
//			id);
//	//fflush(stdout);
	fprintf(trace, "c T%d C%d F%d O%d S%d V%d\n",
			thread,
			classID,
			slotOffset,
			objId,
			fieldSize,
			v);
	sRc++;
}
void Logger::logreadRefFromClaas(int thread, int classID, int slotOffset, int fieldSize, int v){
//	printf("c T%d C%d O%d\n",
//			thread,
//			classID,
//			id);
//	//fflush(stdout);
	fprintf(trace, "r T%d C%d F%d S%d V%d\n",
			thread,
			classID,
			slotOffset,
			fieldSize,
			v);
	rRc++;
}

void Logger::addToRoot(int thread,int id){
//	printf("+ T%d O%d\n",
//			thread,
//			id);
//	//fflush(stdout);
	fprintf(trace, "+ T%d O%d\n",
			thread,
			id);
	ar++;
}
Logger::~Logger() {
}

} /* namespace gcKons */
