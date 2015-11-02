/**
 * \class Object
 * 
 * Object.h
 * 
 * \brief 
 *
 * This class is for manipulating an object.
 * 
 * \author Mazder $Author: kons $
 * 
 * \version $Revision: 3.2 $  
 * 
 * \date $Date: 2015/10/30 $
 * 
 * Contact: cas, IBM
 * 
 * Created on: Jul 30, 2013
 * 
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "../defines.h"
#include <vector>
#include <iostream>
using std::vector;
namespace traceGen {

/*! A class that is used to hold
 * information about an object to be allocated 
 */
class Object {
public:
	/// constructor 
	Object(int id, int payloadSize, int maxPointers, int dateOfCreation);

	///constructor overloading
	Object(int id, int maxPointers, int dateOfCreation, int classId, int primField);
	
	virtual ~Object();
	
	/// This methods returns the object's id
	int getID();
	
	/// This method returns the class id for which the object is created
	int	getClassId();
	
	/// This method returns the size in bytes of memory allocated by the object 
	int getPayloadSize();

	/// This method returns the number of objects that the object is pointing to   
	int	getPointerCount();
	
	/// This method returns the maximum number of objects that the object can point to 
	int getPointersMax();
	
	/** This method returns the reference of 
	  * an object that the object is pointing 
	  * to through the given pointer number 
	  * (0, 1, 2, ..., n-1). 
	  */
	Object* getReferenceTo(int pointerNumber);
	
	/** This method writes the reference of 
	  * the target object into the poniter slot of the object 
	  */
	int setPointer(int pointerNumber, Object* target);

	/// This method sets the thread (id) that created the object
	void setThreadID(int tid);

	/// This method returns the thread (id) that created the object
	int	getThreadID();

	/** This method returns the offset of a field 
	  * for given index and type of the field
	  */
	int getFieldOffset(int index,  int fieldType);

	/** This method returns the field size
	  * for the given type of a field
	  */ 
	int getFieldSize(int fieldType);
	
	/** This method returns the number of primitive
	  * fields into the object
	  */ 	
	int getNumOfPrimField();
	
	/** This method returns type of primitive
	  * for a given index of the primitives filed
	  */
	int primitiveType(int primIndex);
	
	/// A variable to mark the object
	int visited;

	/** A variable to contain the trace line
	 * number when the object is allocated
	 */
	int creationDate;

	/** This array contains the number of  
	  * primitives fields of different primitives
	  */
	int vecPrim[3];

private:
	/// thread id
	int myTid;

	/// object id
	int myId;

	/** The size of storing information about
	  * the object for our purpose, not actual size of the object
	  */
	int myPayoadSize;

	/** How many objects, the object is allowed to point at?
	  */
	int myPointersMax;
	
	/// How many objects the object currently pointing to  
	int myPointersCurrent;
	
	/** The vector of the references of objects 
	  * that the  object is pointing to
	  */
	vector<Object*> pointers;

	/// How many primitive fields
	int nPrimitives;

	/// class id of the object 
	int myClassId;

};

} /* namespace gcKons */
#endif /* OBJECT_H_ */
