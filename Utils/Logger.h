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
	void logRefOperation(int thread, int parentID, int parentSlot, int childID);
	void logEnd();
	void deleteRoot(int thread,int root);
private:
	FILE* trace;
	FILE* dot;
};

} /* namespace gcKons */
#endif /* LOGGER_H_ */
