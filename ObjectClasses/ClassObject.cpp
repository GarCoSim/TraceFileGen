/*
 * ClassObject.cpp
 *
 *  Created on: Jul 17, 2015
 *      Author: cas
 */
#include <iostream>
#include <sstream>
#include "../defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassObject.h"
#include <time.h>

using namespace std;

extern int MAX_PRIMITIVES;	

namespace traceGen {

ClassObject::ClassObject(int classId, string clsName, int staticReference) {
	myId = classId;
	myName = clsName;
	pointerSize = staticReference;
	refContainer.resize(pointerSize);
	for(int i=0; i<(int)refContainer.size(); i++){
		refContainer.at(i) = NULL;
	}


	nPrimitives = (rand() % MAX_PRIMITIVES) +1;
	
	countFieldType[0] = (rand()% nPrimitives); // for char 8
	countFieldType[1]= (rand()% nPrimitives) ;  // for int 32
	
	while(1){

		if ( (countFieldType[0]+countFieldType[1])< nPrimitives ){
			break;
		}
		countFieldType[0] = (rand()% nPrimitives); // for char 8
		countFieldType[1]= (rand()% nPrimitives) ;  // for int 32
	}

	countFieldType[2] = nPrimitives - (countFieldType[0]+countFieldType[1]);  // for long 64
	mySize = 16 + countFieldType[0]*1 + countFieldType[1]*4 + countFieldType[2]*8 + pointerSize*8;
	acc = 0;
}
int ClassObject::getFieldOffset(int index, int fieldType){
	if(fieldType == 3){
		// return offset of the reference field
		return 16 + countFieldType[0]*1 + countFieldType[1]*4 + countFieldType[2]*8 + 8*index;
	}
	else if(fieldType == 0){
		// return offset of char filed
		return 16+index*1;
	}
	else if(fieldType == 1){
		// return offest of int field
		return 16+ countFieldType[0]*1 + index*4;
	}
	else if(fieldType == 2){
		//return offset of long field
		return 16 + countFieldType[0]*1 + countFieldType[1]*4 + index*8;
	}
	else{
		// offset of the object header
		return 0;
	}
}

int ClassObject::getFieldSize(int fieldType){
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
int ClassObject::primitiveType(int primIndex){
	if(countFieldType[0] != 0){
		if ( (int)(primIndex / countFieldType[0]) == 0){
			return 0;
		}
	}
	if(countFieldType[1] != 0){
		if ((int)(primIndex / (countFieldType[0]+ countFieldType[1]) ) == 0){
			return 1;
		}
	}
	if(countFieldType[1] != 0){
		if ((int)(primIndex / (countFieldType[0]+ countFieldType[1] + countFieldType[2]) ) == 0){
			return 2;
		}
	}

}

void ClassObject::setReference(int Index, Object* obj){
	refContainer[Index] = obj;
}
int ClassObject::getReferenceSlot(){
	int k = rand()%(int)refContainer.size();
	while(refContainer[k] != NULL){
		k = rand()%(int)refContainer.size();
		int rnd = rand() % 100;
		if(rnd < 30){
			break;
		}
	}
	if(refContainer[k] != NULL){
		return k;
	}
	else{
		return -1;
	}
}


ClassObject::~ClassObject() {
	// TODO Auto-generated destructor stub
}

}