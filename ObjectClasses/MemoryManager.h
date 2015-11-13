/**
 * \class MemoryManager
 *
 * MemoryManager.h
 *
 * \brief 
 *
 * This class is used to create objects, class objects and to maintain threads and objects graph 
 * 
 * \author Mazder
 * 
 * \version 3.0  
 * 
 * \date  2015/03/04
 * 
 * Origin:
 *
 * \author Kons
 *
 * Created on: Jul 30, 2013
 *
 * Contact: cas, IBM
 */


#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include "Object.h"
#include "ClassObject.h"
#include <vector>

using std::vector;
namespace traceGen {

/*! A class that is used 
 * to hold and manipulate the connectivity 
 * of objects themselves and threads	
 */
class MemoryManager {
public:
	/// Constructor
	MemoryManager();

	/// Destructor
	virtual ~MemoryManager();

	/** This method allocates an object for given object size,
	  *  add object reference to the rootset of the thread,
	  *  and add object reference to the object list
	  *  return object id 
	  */
	int allocateObjectToRootset(int size, int threadNumber, int rootSetNumber, int maxPointers,int creationDate);

	/** This method allocates an object for given object size
	  *  and add object reference to the object list
	  */
	int allocateObject(int size, int maxPointers, Object* creatorObject, int pointerIndex, int creationDate);
	
	/** This method allocates an object, object size is determined 
	  *  dynamically, add object reference to the rootset of the thread,
	  *  and add object reference to the object list
	  *  return object reference  
	  */
	Object* allocateObjectNew(int threadNumber, int maxPointers,int creationDate, int classID, int primField);
	
	/** The method writes reference of targetObject into the pointer 
	  *  slot of startObject
	  */
	int setPointer(Object* startObject, int pointerIndex, Object* targetObject);
	
	/** This method writes reference of an object for a given 
	  * slot of rootset of a thread
	  */
	int setRootPointer(int threadNumber, int rootSetNumber, Object* newObject);
	
	/** This overloading method writes reference of an object  
	  *  at top of the rootset of the thread
	  */
	int setRootPointer(int threadNumber, Object* newObject);
	
	/** This method deletes reference from  
	  *  the object
	  */
	int deletePointer(Object* startObject, int pointerNumber);
	
	/** The method adds reference of an object at 
	  *  top of the thread root set if object reference 
	  *  is not in the root set, else return true
	  */
	bool addObjectRefToRootset(Object* obj, int threadNumber);
	
	/** This method deletes reference for the 
	  *  rootset of the thread for a given slot
	  */
	void deleteRoot(int thread, int root);
	
	/** This method returns an object reference  
	  *  from rootset of the thread for a give slot
	  */
	Object* getRoot(int threadNumber, int rootSlotNumber);

	/** This method returns the reference of the object for 
	  *  given object id
	  */
	Object* getObjectByID(int id);

	/** This method returns true if the object  
	  *  reference is in the root set of the thread
	  */
	int isRoot(int thread, Object* obj);

	/** This method returns the slot of the rootset 
	  *  of the thread for given object reference
	  */
	int getRootsetNumberByReference(int thread, Object* obj);

	/** This method is used to write the state of object 
	  *  graph into a file in .dot format 
	  */
	void visualizeState();

	/** This method marks all objects 
	  *  as not visited
	  */
	void markObjects();

	/** This method returns rootset size 
	  *  of the thread
	  */
	int getRootsetSize(int threadNumber);
	
	/** This method adds reference of an object 
	  *  at the top of the rootset
	  */
	void addObjectToRootset(Object* newObject, int threadNumber);
	
	/** This method deletes an object reference from  
	  *  the top of the rootset
	  */
	void deleteEndFromRootset(int threadNumber);
	
	/** This method deletes an object reference from the  
	  *  rootset for a given slot number
	  */
	void deleteFromRootset(int threadNumber, int rootSlotNumber);
	
	/** This method deletes an object reference from  
	  *  the top of the rootset
	  */
	void deleteFromRootset(int threadNumber);
	
	/** This method returns true if the object reference 
	  *  is in the rootset of the thread
	  */
	bool isObjectInRoot(int thread, Object* obj);
	
	/** This method returns an object id
	  *  by selecting an object randomly form 
	  *  object lists 
	  */
	int getARandomObjectID();

	/** This method returns an object id  
	  * by traversing the object graph 
	  * from the rootset of the thread
	  */
	int getARandomObjectID(int thread);

	/** This method creates n class objects 
	  * and marks their weights 
	  */
	void buildClassTable(int nClass);
	
	/** This method is used to print 
	  * all class information 
	  */
	void printClassTable(char *classfilename);
	
	/** This method returns a class object reference 
	 *  for given class id
	 */
	inline ClassObject* getClassObject(int clsIndex){return classList[clsIndex];}

private:

	/** This method is used to resize
	  * objectLists when it was static
	  */
	int getListSlot();

	/// id for next object allocation
	int nextId;
	
	/// Two dimensional vector for threads and rootsets [numThread][rootsetSize]
	vector<vector<Object*> > rootset;

	/// Collection of all objects (dynamic)
	vector<Object*> objectList;

	/// Collection of all classes (dynamic)
	vector<ClassObject*>classList;
};

} /* namespace gcKons */
#endif /* MEMORYMANAGER_H_ */
