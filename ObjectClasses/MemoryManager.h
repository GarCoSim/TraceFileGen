/*
 * MemoryManager.h
 *
 *  Created on: Jul 30, 2013
 *      Author: kons
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include "Object.h"
#include <vector>

using std::vector;
namespace traceGen {

class MemoryManager {
public:
	MemoryManager();
	virtual ~MemoryManager();
	int allocateObjectToRootset(int size, int threadNumber, int rootSetNumber, int maxPointers,int creationDate);
	int allocateObject(int size, int maxPointers, Object* creatorObject, int pointerIndex, int creationDate);
	int setPointer(Object* startObject, int pointerIndex, Object* targetObject);
	int setRootPointer(int threadNumber, int rootSetNumber, Object* newObject);
	int deletePointer(Object* startObject, int pointerNumber);
	void deleteRoot(int thread, int root);
	Object* getRoot(int threadNumber, int rootSlotNumber);
	Object* getObjectByID(int id);
	int isRoot(int thread, Object* obj);
	int getRootsetNumberByReference(int thread, Object* obj);
	void visualizeState();
	void setupObjects();
	void requestPromotion(Object* object);
	void requestDeletion(Object* object);
private:
	int getListSlot();
	int nextId;
	//two dimensional rootset [threadNum][rootsetSize]
	vector<vector<Object*> > rootset;
	//collection of all objects (dynamic)
	vector<Object*> objectList;
};

} /* namespace gcKons */
#endif /* MEMORYMANAGER_H_ */
