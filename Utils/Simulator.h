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
	int test();
private:
	int currentStep;
	int stepsToGo;
	MemoryManager* memManager;
	void allocateRandomObject(int thread);
	void referenceOperation(int thread);
	int getPartnerThread(int thread);
	Logger* log;
};

} /* namespace gcKons */
#endif /* SIMULATOR_H_ */
