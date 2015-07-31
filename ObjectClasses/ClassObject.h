/*
 * ClassObject.h
 *
 *  Created on: Jul 17, 2015
 *      Author: cas
 */

#ifndef CLASSOBJECT_H_
#define CLASSOBJECT_H_

#include <iostream>
#include <sstream>
#include "../defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Object.h"

using namespace std;

namespace traceGen {

class ClassObject {
public:
	ClassObject(int classId, string clsName, int staticReference);
	virtual ~ClassObject();
	inline int getId(){ return myId;}
	inline int getSize(){return mySize;}
	inline int getStaticRefCount(){return pointerSize;}
	inline void increaseAccess(){ acc++; }
	inline int getNumPrimitives(){return nPrimitives;}
	inline int getAccessCount(){return acc;}
	inline string getName(){return myName;}
	int getFieldOffset(int index, int fieldType);
	int getFieldSize(int fieldType);
	int primitiveType(int primIndex);
	void setReference(int Index, Object* obj);
	int getReferenceSlot();
private:
	int myId;
	string myName;
	int mySize;
	int pointerSize;
	int nPrimitives;
	int countFieldType[3]; 
	int acc;
	vector<Object*> refContainer;
};
}
#endif /* CLASSOBJECT_H_ */
