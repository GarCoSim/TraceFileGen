/*
 * Object.h
 *
 *  Created on: Jul 30, 2013
 *      Author: kons
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "../defines.h"
#include <vector>
#include <iostream>
using std::vector;
namespace traceGen {

class Object {
public:
	Object(int id, int payloadSize, int maxPointers, int dateOfCreation);
	//constructor overloading with a new parameter classID
	//Object(int id, int payloadSize, int maxPointers, int dateOfCreation, int classId);
	Object(int id, int maxPointers, int dateOfCreation, int classId, int primField);
	virtual ~Object();

	int 	getID();
	int		getClassId();
	int 	getPayloadSize();
	int	 	getPointerCount();
	int 	getPointersMax();
	Object* getReferenceTo(int pointerNumber);
	int 	setPointer(int pointerNumber, Object* target);
	void	setThreadID(int tid);
	int		getThreadID();
	int 	getFieldOffset(int index,  int fieldType);
	int 	getFieldSize(int index);
	int 	getNumOfPrimField();
	int 	primitiveType(int primIndex);
	int 	visited;
	int 	creationDate;
	int     vecPrim[3];
private:
	/* thread id */
	int myTid;

	/* object it */
	int 	myId;

	/*The actual object I am storing information about
	 * (not interesting for our purpose, it only has a size) */
	int  	myPayoadSize;

	/*How many objects am I pointing at? How many am I allowed to point at?*/
	int 	myPointersMax;
	int 	myPointersCurrent;

	/*the list of objects I am pointing at*/
	vector<Object*> pointers;

	/* class id */
	int nPrimitives;



	int myClassId;

};

} /* namespace gcKons */
#endif /* OBJECT_H_ */
