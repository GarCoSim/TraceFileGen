/*
 * Object.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: kons
 */

#include "Object.h"
#include "stdio.h"

namespace traceGen {

Object::Object(int id, int payloadSize, int maxPointers, int dateOfCreation) {

	//prepare data structure
	myId = id;
	myPayoadSize = payloadSize;
	myPointersCurrent = 0;
	myPointersMax = maxPointers;
	pointers.resize(maxPointers);
	visited = 0;
	creationDate = dateOfCreation;

	// default classId is set to 0
	myClassId = 0;
	/* A thread id must be positive integer */
	myTid = -1;
}

Object::Object(int id, int payloadSize, int maxPointers, int dateOfCreation, int classId) {

	//prepare data structure
	myId = id;
	myPayoadSize = payloadSize;
	myPointersCurrent = 0;
	myPointersMax = maxPointers;
	pointers.resize(maxPointers);
	visited = 0;
	creationDate = dateOfCreation;
	myClassId = classId;
}


int Object::getID(){
	return this->myId;
}

int Object::getClassId(){
	return this->myClassId;
}

int Object::getPayloadSize(){
	return myPayoadSize;
}

int Object::getPointerCount(){
	return myPointersCurrent;
}

int Object::getPointersMax(){
	return myPointersMax;
}
Object* Object::getReferenceTo(int pointerNumber){
/*	if (!pointers[pointerNumber]){
		printf("errorhere\n");
		fflush(stdout);
		return NULL;
	}*/
	return pointers[pointerNumber];
}

int Object::setPointer(int pointerNumber, Object* target){

	if(pointerNumber >= myPointersMax){
		printf("No, can't do it\n");
		fflush(stdout);
		return 0;
	}

	pointers[pointerNumber] = target;
	return 1;
}


void Object::setThreadID(int tid){
	myTid = tid;
}
int	Object::getThreadID(){
	return myTid;
}

Object::~Object() {
}

} /* namespace gcKons */
