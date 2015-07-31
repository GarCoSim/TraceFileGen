/*
 * Logger.h
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdio.h>
namespace traceGen {



class Logger {
public:
	Logger(char* tracepath);
	virtual ~Logger();
	void logAllocationRoot(int thread, int rootsetIndex, int id, int size, int refCount);
	void logAllocation(int thread, int parent, int parentSlot, int id, int size, int refCount);

	/* allocation : (a T1 O63 S112 N14 C2) */
	void logAllocation(int thread, int id, int size, int refCount, int classID);

	/* reference object to object : (w T1 P64 #3 O63) */
	void logRefOperation(int thread, int parentID, int parentSlot, int childID);
	void logRefOperation(int thread, int parentID, int parentSlot, int childID, int fieldOffset, int fieldSize, int v);
	void logEnd();

	void deleteRoot(int thread,int root);

	/*delete object from root set : (- T1 O63) */
	void deletefromRoot(int thread,int id);

	/* read : (r T1 O63) */
	void logReadOperation(int thread, int id);

	/* reference class to object : (c T1 C44 O89) */
	void logRefOperationClaasToObject(int thread, int classID, int slotOffset, int objId, int fieldSize, int v);
	void logreadRefFromClaas(int thread, int classID, int slotOffset, int fieldSize, int v);
	/* add object to the root set : (+ T1 O63) */
	void addToRoot(int thread,int id);


	void logstoreObjFieldWithPrimOperation(int thread, int parentID, int fieldOffset, int fieldSize, int v );
	void logstoreClassFieldWithPrimOperation(int thread, int classID, int fieldOffset, int fieldSize, int v );
	void logreadClassFieldWithPrimOperation(int thread, int classID, int fieldOffset, int fieldSize, int v );

	void logreadObjFieldWithPrimOperation(int thread, int parentID, int fieldOffset, int fieldSize, int v );
	void logreadRefFromObjectOperation(int thread, int parentID, int parentSlot, int fieldSize, int v);
	

private:
	FILE* trace;
	FILE* dot;
};


} /* namespace gcKons */
#endif /* LOGGER_H_ */
