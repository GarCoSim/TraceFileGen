/**
 * \class ClassObject
 *
 * ClassObject.h
 *
 * \brief 
 *
 * This class is used to manipulate a class object.
 * 
 * \author Mazder
 * 
 * Created on: Jul 17, 2015
 *
 * Contact: cas, IBM
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

/*! A class of class object that is 
 * used to hold the class inforamtion 
 * of an object that is allocated.
 */
class ClassObject {
public:
	/// Constructor
	ClassObject(int classId, string clsName, int staticReference);

	/// Destructor
	virtual ~ClassObject();
	
	/// This inline method returns class id
	inline int getId(){ return myId;}
	/// This inline method returns class object size
	inline int getSize(){return mySize;}

	/// This inline method returns the no. of static reference fields
	inline int getStaticRefCount(){return pointerSize;}
	
	/// This inline method increases a counter of class access
	inline void increaseAccess(){ acc++; }
	
	/// This inline method returns the no. static primitive fields
	inline int getNumPrimitives(){return nPrimitives;}

	/// This inline method returns the number of usages of a class in object creation
	inline int getAccessCount(){return acc;}

	/// This inline method return the class name
	inline string getName(){return myName;}

	/// This method returns filed offset for given field index and field type
	int getFieldOffset(int index, int fieldType);

	/// This method returns field size for given field type
	int getFieldSize(int fieldType);

	/// This method returns the type of primitives for given primitive field index 
	int primitiveType(int primIndex);

	/// This method writes static reference of an object into a class 
	void setReference(int Index, Object* obj);

	/// This method returns a static reference slot of a class
	int getReferenceSlot();

	/// This inline function sets the weight of a class
	inline void setWeight(double x){weight=x;}

	/// This inline function returns the weight of a class
	inline double getWeight(){ return weight;}
	
private:
	/// Class id
	int myId; 
	/// Class name
	string myName; //
	/// Class size
	int mySize;
	/// No. of static reference fields
	int pointerSize;
	/// No. of static primitive fields
	int nPrimitives;
	/// No of types of fields
	int countFieldType[3];
	/// An access counter 
	int acc;
	/// Container of static references
	vector<Object*> refContainer;
	/// class weight
	double weight;
};
}
#endif /* CLASSOBJECT_H_ */
