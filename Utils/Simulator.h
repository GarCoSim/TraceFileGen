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
	void allocationRandomObjectAARD(int thread);
	void addReferenceToRootset(int thread); // '+'
	void deleteReferenceFromRootset(int thread); // '-'
	void setReferenceToObject(int thread); // 'w'
	void storeObjectFiledWithPrimitive(int thread);
	void readObjectFiledWithPrimitive(int thread);
	void readReferenceFromObject(int thread);
	void setReferenceToClass(int thread); // 'c'
	void storeClassFiledWithPrimitive(int thread);
	void readClassFiledWithPrimitive(int thread);
	void readReferenceFromClass(int thread);
	Logger* log;
};


} /* namespace gcKons */
#endif /* SIMULATOR_H_ */
