/**
 * \class Simulator
 * 
 * Simulator.h
 * 
 * \brief 
 *
 * This class is used to iterate for all MM operations by selecting threads and memory management operations  
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

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "../ObjectClasses/MemoryManager.h"
#include "../Utils/Logger.h"



namespace traceGen {
/*! A class that is used 
 * to select threads and memory management operations
 * for generating trace files  
 */
class Simulator {
public:
	/// Constructor
	Simulator(char* tracepath);

	/// Destructor
	virtual ~Simulator();

	/** This method is used to generate trace files
	  * for version 0.0
	  */ 
	int run(int simulationSteps);
	
	/** This method is used to create all classes 
	  * with attributes   
	  */
	void initializeClassTable(char* classfilename );
	/** This method is used to generate trace files
		for version 3.0
	  */ 
	int runTraceFileGenerator(int simulationSteps);

private:
	int currentStep;
	int stepsToGo;
	MemoryManager* memManager;
	void allocateRandomObject(int thread);
	void referenceOperation(int thread);
	int getPartnerThread(int thread);
	void readObject(int thread); // 'r'
	
	/*possible operations for version 3.0*/
	bool allocationRandomObjectAARD(int thread);
	bool addReferenceToRootset(int thread); // '+'
	bool deleteReferenceFromRootset(int thread); // '-'
	bool setReferenceToObject(int thread); // 'w'
	bool storeObjectFiledWithPrimitive(int thread);
	bool readObjectFiledWithPrimitive(int thread);
	bool readReferenceFromObject(int thread);
	bool setReferenceToClass(int thread); // 'c'
	bool storeClassFiledWithPrimitive(int thread);
	bool readClassFiledWithPrimitive(int thread);
	bool readReferenceFromClass(int thread);
	Logger* log;
};


} /* namespace gcKons */
#endif /* SIMULATOR_H_ */
