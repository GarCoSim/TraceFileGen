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
	int run(int simulationSteps);
	/* runTracetFileGenerator() is a modified method of run() */
	void initializeClassTable(char* classfilename);
	void initializeClassTable(int nClass);
	int runTraceFileGenerator(int simulationSteps);
	int test();
private:
	int currentStep;

	int stepsToGo;
	MemoryManager* memManager;
	void allocateRandomObject(int thread);
	void allocationRandomObjectAARD(int thread);
	void referenceOperation(int thread);
	int getPartnerThread(int thread);

	
	/*possible primitives access operations */

	void storeObjectFiledWithPrimitive(int thread);
	void readObjectFiledWithPrimitive(int thread);
	void readReferenceFromObject(int thread);

	

	/* possible reference operations */
	void setReferenceToObject(int thread); // 'w'

	void addReferenceToRootset(int thread); // '+'
	void deleteReferenceFromRootset(int thread); // '-'
	void setReferenceToClass(int thread); // 'c'
	void readObject(int thread); // 'r'

	Logger* log;
};


} /* namespace gcKons */
#endif /* SIMULATOR_H_ */
