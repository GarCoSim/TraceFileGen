/*
 * Simulator.h
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "../ObjectClasses/MemoryManager.h"
#include "../Utils/Logger.h"



namespace traceGen {

class Simulator {
public:
	Simulator(char* tracepath);
	virtual ~Simulator();
	/* for version 0 */
	int run(int simulationSteps);
	int test();
	/* for version 3 */
	void initializeClassTable(char* classfilename );
	int runTraceFileGenerator(int simulationSteps);

private:
	int currentStep;
	int stepsToGo;
	MemoryManager* memManager;
	void allocateRandomObject(int thread);
	void referenceOperation(int thread);
	int getPartnerThread(int thread);
	void readObject(int thread); // 'r'
	
	/*possible operations for version 3*/
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
