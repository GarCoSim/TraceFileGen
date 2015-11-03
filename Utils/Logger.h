/**
 * \class Logger
 *
 * Logger.h
 *
 * \brief 
 *
 * This class is used to write basic MM operations into trace files.
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
 * Created on: 2013/08/14
 *
 * Contact: cas, IBM
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdio.h>
namespace traceGen {


/*! A class that is used
 * 	to write memory management operations
 *  into a file --- called trace file
 */
class Logger {
public:
	/// Constructor
	Logger(char* tracepath);

	///Destructor
	virtual ~Logger();
	
	/** This method writes information into the trace file
	  * for an object allocation (for version 0.0)
	  */
	void logAllocationRoot(int thread, int rootsetIndex, int id, int size, int refCount);

	/** This method writes information into the trace file
	  * for an object allocation and 
	  * write reference into a parent object (for version 0.0)
	  */
	void logAllocation(int thread, int parent, int parentSlot, int id, int size, int refCount);

	/** This method writes information
	  * into the trace file for an object allocation (for version 3.0)
	  */
	void logAllocation(int thread, int id, int size, int refCount, int classID);

	/** This method writes information into the trace file 
	  * for writing reference of an object into another object 
	  */
	void logRefOperation(int thread, int parentID, int parentSlot, int childID);

	/** This overload method writes information into the trace file 
	  * for writing reference of an object into another object 
	  */
	void logRefOperation(int thread, int parentID, int parentSlot, int childID, int fieldOffset, int fieldSize, int v);

	/** This fucntion is used to view
	  * the status of object graph at the end.
	  */ 
	void logEnd();

	/** This method writes inforamtion into 
	  * the trace for deleting a reference from the thread root set
	  * for given slot (version 0.0)
	  */
	void deleteRoot(int thread,int root);

	/** This method writes inforamtion into 
	  * the trace for deleting a reference of an object 
	  * from the thread root set
	  */
	void deletefromRoot(int thread,int id);

	/** This method writes inforamtion into 
	  * the trace for reading fileds from an object (version 0.0)
	  */
	void logReadOperation(int thread, int id);

	/** This method writes detail inforamtion into 
	  * the trace for writing reference of an object 
	  * into another object  
	  */
	void logRefOperationClaasToObject(int thread, int classID, int slotOffset, int objId, int fieldSize, int v);

	/** This method writes detail inforamtion into 
	  * the trace for reading static reference of an object 
	  * from the a class  
	  */
	void logreadRefFromClaas(int thread, int classID, int slotOffset, int fieldSize, int v);
	
	/** This method writes inforamtion into 
	  * the trace for writing reference of an object 
	  * into the rootset of the thread   
	  */
	void addToRoot(int thread,int id);

	/** This method writes detail inforamtion into 
	  * the trace for writing primitive filed of an object  
	  */
	void logstoreObjFieldWithPrimOperation(int thread, int parentID, int fieldOffset, int fieldSize, int v );

	/** This method writes detail inforamtion into 
	  * the trace for writing static primitive filed of a class 
	  */
	void logstoreClassFieldWithPrimOperation(int thread, int classID, int fieldOffset, int fieldSize, int v );

	/** This method writes detail inforamtion into 
	  * the trace for reading static primitive filed of a class 
	  */
	void logreadClassFieldWithPrimOperation(int thread, int classID, int fieldOffset, int fieldSize, int v );

	/** This method writes detail inforamtion into 
	  * the trace for reading primitive filed of an object  
	  */
	void logreadObjFieldWithPrimOperation(int thread, int parentID, int fieldOffset, int fieldSize, int v );

	/** This method writes detail inforamtion into 
	  * the trace for reading reference of an object  
	  */
	void logreadRefFromObjectOperation(int thread, int parentID, int parentSlot, int fieldSize, int v);

private:
	/// Pointer trace file
	FILE* trace;
	/// pointer .dot file
	FILE* dot;
};


} /* namespace gcKons */
#endif /* LOGGER_H_ */
