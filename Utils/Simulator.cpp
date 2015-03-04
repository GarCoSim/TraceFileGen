/*
 * Simulator.cpp
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */

#include "Simulator.h"
#include "../defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

namespace traceGen {

Simulator::Simulator(char* tracepath) {
	stepsToGo = 0;
	currentStep = 0;

	//create MemoryManager
	memManager = new MemoryManager();

	//init logger
	log = new Logger(tracepath);

	//init random numbers
	srand(time(NULL));
}

int Simulator::run(int simulationSteps){
	stepsToGo = simulationSteps;
	currentStep = simulationSteps;
	while(currentStep > 0){
		currentStep--;

		//print indicator showing the progress every 10000 iterations.
		if(currentStep%10000==0){
			printf("%f percent done.\n",(stepsToGo-currentStep)/(float)stepsToGo*100);
		}

		//which thread is executing?
		int thread = rand() % NUM_THREADS;

		//chech if a root has to be deleted.
		int delRoot = rand()%100;
		if(delRoot < ROOT_DELETE_PROBABILITY){
			//pick a root.
			int root = rand()%ROOTSET_SIZE;
			//only immortality can save it now.
			Object* obj = memManager->getRoot(thread,root);
			if(obj != NULL && obj->creationDate < (currentStep+ROOT_IMMORTALITY_AGE)){
				log->deleteRoot(thread,root);
				memManager->deleteRoot(thread,root);
			}else if(obj != NULL && obj->creationDate >(currentStep+ROOT_IMMORTALITY_AGE)){
				//printf("Met an immortal one.\n");
			}
		}

		int doAllocate = rand() % 100;
		//allocate?
		if(doAllocate < RATIO_ALLOC_SET){
			//allocation will be performed.
			allocateRandomObject(thread);
		} else {//reference operation
			referenceOperation(thread);
			//printf("D1 %d \n", c++);
			//fflush(stdout);
		}
	}
	//memManager->visualizeState();
	log->logEnd();
	//memManager->visualizeState();
	return 1;
}



void Simulator::allocateRandomObject(int thread){

	int size = (rand() % (MAX_PAYLOAD - MIN_PAYLOAD)) + MIN_PAYLOAD;
	int outGoingRefsMax = (rand() % MAX_POINTERS) + 1;

	memManager->setupObjects();

	Object* parent;
	Object* child;

	int root = rand() % ROOTSET_SIZE;
	int createRoot = 1;
	int rnd;
	int id;

	child = memManager->getRoot(thread, root);
	//as long as the current object is an actual one
	while(child){
		if(child->visited == 1){
			return;
		} else {
			child->visited = 1;
		}
		parent = child;
		createRoot++;
		rnd = rand() % parent->getPointersMax();
		child = parent->getReferenceTo(rnd);
	}

	 //differentiate between allocating to root and allocate somewhere else
	 if(createRoot == 1){
		 id = memManager->allocateObjectToRootset(size, thread, root, outGoingRefsMax,currentStep);
		 log->logAllocationRoot(thread, root, id, size, outGoingRefsMax);

	 }else{
		 id = memManager->allocateObject(size,outGoingRefsMax,parent, rnd,currentStep);
		 //log
		 log->logAllocation(thread, parent->getID(),rnd,id,size,outGoingRefsMax);
	 }
}
int Simulator::getPartnerThread(int thread){
	if(thread%2==0){
		return (thread+1)%NUM_THREADS;
	}else{
		return (thread-1)%NUM_THREADS;
	}
}

void Simulator::referenceOperation(int thread){

	int targetThread;

	int root = rand() % ROOTSET_SIZE;

	//most reference operations will be perfomed within the thread.
	//escaping objects are less likely.
	int shouldEscape = rand()%100;
	if(shouldEscape<ESCAPE_PROBABILITY){
		/*an escape will happen. Now the decision is whether
		 *to the partner, or to a random thread*/
		int partner = rand()%100;
		if(partner < ESPACE_TO_PARTNER){

			targetThread = getPartnerThread(thread);
		}else{
			targetThread = rand() % NUM_THREADS;
		}
	} else {
		// don't escape.
		targetThread = thread;
	}




	int rnd, rnd2;
	int sloNumber;//rand() % parent->getPointersMax();

	Object* parent = memManager->getRoot(thread, root);
	Object* child = memManager->getRoot(targetThread, root);;
	Object* temp;

	int counter = ROOTSET_SIZE;
	//printf("D1\n");
	fflush(stdout);
	//find source
	while(!parent){
		 root = rand() % ROOTSET_SIZE;
		 parent = memManager->getRoot(thread, root);
		 counter--;
		 if(counter < 0 && !parent){
			 //no objects created yet
			 return;
		 }
	}
	temp = parent;
	while(temp){
		parent = temp;
		rnd = rand() % parent->getPointersMax();
		temp = parent->getReferenceTo(rnd);

		//if I would like to stop, I stop
		rnd2 = rand() % 100;
		if(rnd2 < 30){
			break;
		}
	}
	//printf("D2\n");
	fflush(stdout);


	//pick target from random thread
	counter = ROOTSET_SIZE;

	while(!child){
		root = rand() % ROOTSET_SIZE;
		child = memManager->getRoot(targetThread, root);
		counter--;
		if(counter < 0 && !child){
			 //no objects created yet
			return;
		}
	}
	temp = child;
	while(temp){
		child = temp;
		rnd = rand() % child->getPointersMax();
		temp = child->getReferenceTo(rnd);
		rnd = rand() % 100;
		if(rnd < 30){
			break;
		}
	}
	//printf("D3\n");
	fflush(stdout);
	// create the reference picked
	if(parent != child){
		sloNumber = rand() % parent->getPointersMax();
		memManager->setPointer(parent,sloNumber,child);
		log->logRefOperation(thread,parent->getID(),sloNumber,child->getID());
	}
}

int Simulator::test(){
//	memManager->allocateObjectToRootset(12,0,0,3);
//
//	int id = memManager->allocateObjectToRootset(11,0,3,2);
//
//	int id2 =memManager->allocateObject(10,2,memManager->getObjectByID(id),0);
//
//	if(memManager->getObjectByID(id2)->getReferenceTo(0)){
//		int k;
//		k=0;
//		k++;
//	}
//	if(memManager->getObjectByID(id2)->getReferenceTo(1)){
//		int l;
//		l=0;
//		l++;
//	}
//
//
//	memManager->allocateObject(10,2,memManager->getObjectByID(id),1);
	return 0;
}

int Simulator::newRun(int simulationSteps){
	stepsToGo = simulationSteps;
	currentStep = simulationSteps;
	while(currentStep > 0){
		currentStep--;

		//print indicator showing the progress every 10000 iterations.
		if(currentStep%10000==0){
			printf("%f percent done.\n",(stepsToGo-currentStep)/(float)stepsToGo*100);
		}

		//which thread is executing?
		int thread = rand() % NUM_THREADS;
		int doAllocate = rand() % 100;
		//allocate?
		if(doAllocate < RATIO_ALLOC_SET){
			/* allocation operations:
			* 10% ('allocate' followed by 'add' to root set) and
			* 90% ('allocate' followed by 'add' to root set and if possible ( 'reference' to the other object followed by 'delete' from root set)
			*/
			allocationRandomObjectAARD(thread);
		}else{
			/* Reference Operations
			 * 80
			 * */
		}


	}
	return 1;
}

void Simulator::allocationRandomObjectAARD(int thread){
	// according to Gerhard
	/* allocation operations:
	* 10% ('allocate' followed by 'add' to root set) and
	* 90% ('allocate' followed by 'add' to root set and if possible ( 'reference' to the other object followed by 'delete' from root set)
	*/

	int size = (rand() % (MAX_PAYLOAD - MIN_PAYLOAD)) + MIN_PAYLOAD;
	int outGoingRefsMax = (rand() % MAX_POINTERS) + 1;
	int rootsetSize = memManager->getRootsetSize(thread);
	Object* newObject;

	/* class id is generated randomly, however, it will be changed later on
	 */
	int classID = rand()%100;
	newObject = memManager->allocateObject(size, thread, outGoingRefsMax, currentStep, classID);
	memManager->addObjectToRootset(newObject, thread);
	log->logAllocation(thread, newObject->getID(), size, outGoingRefsMax, classID);
	log->addToRoot(thread, newObject->getID());

	int doReference = rand()%100;

	if(doReference>10){
		// to reference operation, rootset must contain object(s)
		if(rootsetSize){

			int rnd, rootSlotNumber;
			rootSlotNumber = rand() % rootsetSize;
			memManager->setupObjects();
			int createRoot = 1; // that means there exists at the root set;

			Object* child;
			Object* parent;

			child = memManager->getRoot(thread, rootSlotNumber);
			// how here find an object randomly from the tree
			while(child){
				if(child->visited == 1){
					// this means that I might fall into a loop
					return;
				}else{
					child->visited = 1;
				}
				parent = child;
				createRoot++;
				rnd = rand() % parent->getPointersMax();
				child = parent->getReferenceTo(rnd);
			}

			// do reference operation; just set pointer parent to child
			memManager->setPointer(parent,rnd, newObject);
			log->logRefOperation(thread,parent->getID(),rnd, newObject->getID());

			// delete the pointer of the newObject from the root set;
			// newObject was actually added to the end of the rooset, so it can be deleted from the last
			memManager->deleteEndFromRootset(thread);
			log->deletefromRoot(thread, newObject->getID());
		}
	}

}

Simulator::~Simulator() {
}

} /* namespace gcKons */
