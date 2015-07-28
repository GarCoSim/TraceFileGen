/*
 * Object.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: kons
 */

#include "Object.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

#include <time.h>


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

	nPrimitives = 0;
	vecPrim[0] = 0; // for char 8
	vecPrim[1]= 0;  // for int 32
	vecPrim[2] =0;  // for long 64
}

Object::Object(int id, int maxPointers, int dateOfCreation, int classId, int primField) {
	//prepare data structure
	myId = id;
	//myPayoadSize = payloadSize;
	myPointersCurrent = 0;
	myPointersMax = maxPointers;
	pointers.resize(maxPointers);
	visited = 0;
	creationDate = dateOfCreation;
	myClassId = classId;
	nPrimitives = primField;
	vecPrim[0] = (rand()% nPrimitives); // for char 8
	vecPrim[1]= (rand()% nPrimitives) ;  // for int 32
	while(1){

		if ( (vecPrim[0]+vecPrim[1])< nPrimitives ){
			break;
		}
		vecPrim[0] = (rand()% nPrimitives); // for char 8
		vecPrim[1]= (rand()% nPrimitives) ;  // for int 32
	}
	vecPrim[2] = nPrimitives - (vecPrim[0]+vecPrim[1]);  // for long 64
	myPayoadSize = 16 + vecPrim[0]*1 + vecPrim[1]*4 + vecPrim[2]*8 + myPointersMax*8;
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
	if (!pointers[pointerNumber]){
		//printf("errorhere\n");
		fflush(stdout);
		return NULL;
	}
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

int Object::getFieldOffset(int index, int fieldType){
	
	if(fieldType == 3){
		// return offset of the reference field
		return 16 + vecPrim[0]*1 + vecPrim[1]*4 + vecPrim[2]*8 + 8*index;
	}
	else if(fieldType == 0){
		// return offset of char filed
		return 16+index*1;
	}
	else if(fieldType == 1){
		// return offest of int field
		return 16+ vecPrim[0]*1 + index*4;
	}
	else if(fieldType == 2){
		//return offset of long field
		return 16 + vecPrim[0]*1 + vecPrim[1]*4 + index*8;
	}
	else{
		// offset of the object header
		return 0;
	}
}
int Object::getFieldSize(int fieldType){
	if (fieldType == 0){
		return 1;
	}
	else if(fieldType == 1){
		return 4;
	}
	else if(fieldType == 2){
		return 8;
	}
	else if(fieldType == 3){
		return 8;
	}
	else{
		// field size never be 0;
		return 0;
	}
}
int Object::getNumOfPrimField(){
	return nPrimitives;
}

int Object::primitiveType(int primIndex){
	if(vecPrim[0] != 0){
		if ( (int)(primIndex / vecPrim[0]) == 0){
			return 0;
		}
	}
	if(vecPrim[1] != 0){
		if ((int)(primIndex / (vecPrim[0]+ vecPrim[1]) ) == 0){
			return 1;
		}
	}
	if(vecPrim[1] != 0){
		if ((int)(primIndex / (vecPrim[0]+ vecPrim[1] + vecPrim[2]) ) == 0){
			return 2;
		}
	}
}




Object::~Object() {
}

} /* namespace gcKons */
