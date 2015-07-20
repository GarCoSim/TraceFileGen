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

using namespace std;

namespace traceGen {

class ClassObject {
public:
	ClassObject(int classId, string name, int staticReference);
	virtual ~ClassObject();
	inline int getId(){ return myId;}
private:
	int myId;
	string myName;
	int mySize;
	int maxPointerSize;
	int nPrimitives;
	int countFieldType[3]; 
};
}
#endif /* CLASSOBJECT_H_ */
