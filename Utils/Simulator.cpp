/*
 * Simulator.cpp
 *
 *  Created on: 2013-08-14
 *      Author: kons
 */
#include <iostream>
#include <sstream>
#include "Simulator.h"
#include "../defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

using namespace std;


FILE* classFilePointer;

extern int NUM_THREADS ;
extern int RATIO_ALLOC_SET ;
extern int readaccess ;
extern int storeaccess ;
extern int clazz;

extern int ROOT_DELETE_PROBABILITY;

extern int clazz ;

typedef struct Tabe{
	int id;
	int nField;
	int nStatic;
	int acc;
	char className[20];
}classTable;

classTable *traceClassTable;

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


void Simulator::initializeClassTable(int nClass){
	memManager-> buildClassTable(nClass);
}

void Simulator::initializeClassTable(char *classfilename){

	
	traceClassTable = new classTable[clazz];
	
	classFilePointer = fopen(classfilename,"w+");

	for(int i=0; i<clazz; i++){
		
		traceClassTable[i].id = i+1;
		int f = rand()%100;
		int s = rand()%10;
		while(s > f){
			f = rand()%100;
			s = rand()%10;
		}
		traceClassTable[i].nField = f;
		traceClassTable[i].nStatic = s;
		traceClassTable[i].acc = 0;
		
		stringstream ss;
		ss << i;
		string str = "kdm"+ss.str();
		strcpy(traceClassTable[i].className, str.c_str());
		fprintf(classFilePointer, "C%d I%d #%d %s\n", traceClassTable[i].id, traceClassTable[i].nField, traceClassTable[i].nStatic, traceClassTable[i].className);

	}
}

int Simulator::runTraceFileGenerator(int simulationSteps){


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
		
		int i = RATIO_ALLOC_SET;
		int j = i + storeaccess ;
		int k = j + readaccess;
		int l = k + ROOT_DELETE_PROBABILITY;
		int x = rand()%100;
		//printf("i=%d j=%d k=%d l=%d x=%d\n", i,j,k,l,x);
		if( (0 <= x)&&(x <= i) ){
			printf( "Select Allocation....\n");
			allocationRandomObjectAARD(thread);
		}
		
		else if( ( i < x) && (x <= j) ){
			// store operations
			printf( "Select Store....\n");
			int doStore = rand() % 100;
			int storeInObject = rand()%100;
			if(doStore <  PRIMITIVE_PROBABILITY ){
				if(storeInObject > STATIC_FIELD_ACCESS){
					storeObjectFiledWithPrimitive(thread);
				}
				else{
					//storeClassFiledWithPrimitive(thread);
				}
			}
			else{
				if(storeInObject > STATIC_FIELD_ACCESS){

					setReferenceToObject(thread);
				}
				else{
					//setReferenceToClass(thread);
				}
			}
		}
		else if( ( j< x) && ( x <= k) ){
			// read operations
			printf( "Select Read....\n");
			int doRead = rand() % 100;
			int readFromObject = rand()%100;
			if(doRead< PRIMITIVE_PROBABILITY ){
				if(readFromObject > STATIC_FIELD_ACCESS){
					readObjectFiledWithPrimitive(thread);
				}
				else{
					//readClassFiledWithPrimitive(thread);
				}
			}
			else{
				if(readFromObject > STATIC_FIELD_ACCESS){
					readReferenceFromObject(thread);
				}
				else{
					//readReferenceFromClass(thread);
				}
			}
		}
		else if( ( k< x) && ( x <= l) ){
			printf( "Select Add/DeleteRootset....\n");
			int addDel = rand() % 100;
			
			if(addDel>50){
				//add the pointer of an existing object to the root set of either the same thread or other thread
				//
				addReferenceToRootset(thread);
			}
			else{
				//delete the pointer of an existing object from the root set
				//
				deleteReferenceFromRootset(thread);
			}
		}
		

	/*
		if(doAllocate < allocation){
			// allocation operations:
			// 10% ('allocate' followed by 'add' to root set) and
			// 90% ('allocate' followed by 'add' to root set and if possible ( 'reference' to the other object followed by 'delete' from root set)
			//
			allocationRandomObjectAARD(thread);
		}else{
			// Reference Operations:
			// add reference to the root set
			// set reference to object
			// set reference to class
			// delete reference from the root set
			// read object
			// 
			int doReference = rand() % 100;
			if(doReference < REFERENCE){
				doReference = rand() % 100;
				if(doReference<REFERENCE){
					doReference = rand() % 100;
					if(doReference < REFERENCE){
						//set the pointer of an existing object to another existing object of either the same thread or other thread
						setReferenceToObject(thread);
						//set reference to class
						setReferenceToClass(thread);
					}
					else{
						//set the pointer of an existing object to another existing object of either the same thread or other thread
						setReferenceToObject(thread);
					}
				}
				else{
					doReference = rand() % 100;
					if(doReference>ROOT_DELETE_PROBABILITY){
						//add the pointer of an existing object to the root set of either the same thread or other thread
						addReferenceToRootset(thread);
					}
					else{
						//delete the pointer of an existing object from the root set
						deleteReferenceFromRootset(thread);
					}
				}
			}
			else{
				//read object operation
				readObject(thread);
			}

		}
	*/

	}
	delete [] traceClassTable;

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

	int primitiveField = (rand() % MAX_PRIMITIVES) +1;

	int rootsetSize = memManager->getRootsetSize(thread);
	Object* newObject;

	/* class id is generated randomly, however, it will be changed later on
	 */

	//int classID = rand()%100;
	//3.0
	int classID;
	int clsIndex = rand()%clazz;
	// select class which has been acceessed (hit) less
	while(1){
		if( traceClassTable[clsIndex].acc < (int)(MAXACCESS/2) ){
			classID = clsIndex+1;
			traceClassTable[clsIndex].acc = traceClassTable[clsIndex].acc +1;
			break;
		}
		clsIndex = rand()%clazz;
	}

	//newObject = memManager->allocateObject(size, thread, outGoingRefsMax, currentStep, classID);
	newObject = memManager->allocateObject(size, thread, outGoingRefsMax, currentStep, classID, primitiveField); // 3.0
	memManager->addObjectToRootset(newObject, thread);


	log->logAllocation(thread, newObject->getID(), newObject->getPayloadSize(), outGoingRefsMax, classID);
	log->addToRoot(thread, newObject->getID());

	

	int doReference = rand()%100;

	if(doReference < RATIO_ALLOCATION){
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
			//int fieldIndex = rand()%outGoingRefsMax;
			
			//log->logRefOperation(thread,parent->getID(),rnd, newObject->getID());
			log->logRefOperation(thread,parent->getID(),rnd, newObject->getID(), parent->getFieldOffset(rnd, REFTYPE), (int)8, (int)rand()%2 );

			// delete the pointer of the newObject from the root set;
			// newObject was actually added to the end of the rooset, so it can be deleted from the last
			memManager->deleteEndFromRootset(thread);
			
			log->deletefromRoot(thread, newObject->getID());
		}
	}

}

void Simulator::addReferenceToRootset(int thread){

	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;
	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}

	Object* object = memManager->getRoot(thread, rootSlotNumber);
	Object* temp;
	int rnd;
	temp = object;
	while(temp){
		object = temp;
		rnd = rand() % object->getPointersMax();
		temp = object->getReferenceTo(rnd);
		rnd = rand() % 100;
		if(rnd < 30){
			break;
		}
	}

	// escaping object are less likely;
	int shouldEscape = rand() % 100;
	if(shouldEscape < ESCAPE_PROBABILITY){
		/*an escape will happen. Now the decision is whether
		*to the partner, or to a random thread*/
		int partner = rand()%100;
		if(partner < ESPACE_TO_PARTNER){
			targetThread = getPartnerThread(thread);
		}else{
			targetThread = rand() % NUM_THREADS;
		}
	}else{
		// don't escape.
		targetThread = thread;
	}
	if(memManager->getRootsetSize(targetThread) && memManager->isObjectInRoot(targetThread, object)){
		// object already exists in root set;
		return;
	}
	
	memManager->addObjectToRootset(object, targetThread);

	log->addToRoot(targetThread, object->getID());
}

void Simulator::storeObjectFiledWithPrimitive(int thread){

	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}
	Object* temp;
	Object* parent = memManager->getRoot(thread, rootSlotNumber);
	int rnd, rnd2;

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
	int primIndex = rand() % parent->getNumOfPrimField();
	int primType = rand()%3;

	log->logstoreObjFieldWithPrimOperation(thread, parent->getID(), parent->getFieldOffset(primIndex, primType), parent->getFieldSize(primType), (int)rand()%2 );

}



void Simulator::readObjectFiledWithPrimitive(int thread){

	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}
	Object* temp;
	Object* parent = memManager->getRoot(thread, rootSlotNumber);
	int rnd, rnd2;

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
	int primIndex = rand() % parent->getNumOfPrimField();
	int primType = rand()%3;

	log->logreadObjFieldWithPrimOperation(thread, parent->getID(), parent->getFieldOffset(primIndex, primType), parent->getFieldSize(primType), (int)rand()%2 );

}


void Simulator::setReferenceToObject(int thread){
	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}

	// most reference operations will be performed within the thread.
	// escaping object are less likely;
	int shouldEscape = rand() % 100;
	if(shouldEscape < ESCAPE_PROBABILITY){
		/*an escape will happen. Now the decision is whether
		*to the partner, or to a random thread*/
		int partner = rand()%100;
		if(partner < ESPACE_TO_PARTNER){
			targetThread = getPartnerThread(thread);
		}else{
			targetThread = rand() % NUM_THREADS;
		}
	}else{
		// don't escape.
		targetThread = thread;
	}

	Object* temp;
	Object* parent = memManager->getRoot(thread, rootSlotNumber);
	int rnd, rnd2;

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

	int childRootSetSize = memManager->getRootsetSize(targetThread);
	int slotNumber;
	if(childRootSetSize){
		slotNumber = rand() % (childRootSetSize);
	}else{
		//printf("No object created for the targetThread\n");
		return;
	}
	Object* child = memManager->getRoot(targetThread, slotNumber);
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

	// create the reference picked
	if(parent != child){
		slotNumber = rand() % parent->getPointersMax();
		memManager->setPointer(parent,slotNumber,child);	
		//log->logRefOperation(thread,parent->getID(),slotNumber,child->getID());
		
		log->logRefOperation(thread,parent->getID(),slotNumber,child->getID(),  parent->getFieldOffset(slotNumber, REFTYPE), (int)8, (int)rand()%2 );
	}
}


void Simulator::readReferenceFromObject(int thread){
	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}

	// most reference operations will be performed within the thread.
	// escaping object are less likely;
	int shouldEscape = rand() % 100;
	if(shouldEscape < ESCAPE_PROBABILITY){
		/*an escape will happen. Now the decision is whether
		*to the partner, or to a random thread*/
		int partner = rand()%100;
		if(partner < ESPACE_TO_PARTNER){
			targetThread = getPartnerThread(thread);
		}else{
			targetThread = rand() % NUM_THREADS;
		}
	}else{
		// don't escape.
		targetThread = thread;
	}

	Object* temp;
	Object* parent = memManager->getRoot(thread, rootSlotNumber);
	int rnd, rnd2;

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

	int childRootSetSize = memManager->getRootsetSize(targetThread);
	int slotNumber;
	if(childRootSetSize){
		slotNumber = rand() % (childRootSetSize);
	}else{
		//printf("No object created for the targetThread\n");
		return;
	}
	Object* child = memManager->getRoot(targetThread, slotNumber);
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

	// create the reference picked
	if(parent != child){
		slotNumber = rand() % parent->getPointersMax();
		memManager->setPointer(parent,slotNumber,child);	
		//log->logRefOperation(thread,parent->getID(),slotNumber,child->getID());

		log->logreadRefFromObjectOperation(thread,parent->getID(),slotNumber, (int)8, (int)rand()%2 );
	}
}



void Simulator::deleteReferenceFromRootset(int thread){
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}
	Object* obj = memManager->getRoot(thread, rootSlotNumber);
	memManager->deleteFromRootset(thread, rootSlotNumber);
	log->deletefromRoot(thread, obj->getID());
}

void Simulator::setReferenceToClass(int thread){
	/* Actually, it is the same operation as setReferenceToObject, (print classId of parent obj in place of parentID)*/
	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return;
	}
	// most reference operations will be performed within the thread.
	// escaping object are less likely;
	int shouldEscape = rand() % 100;
	if(shouldEscape < ESCAPE_PROBABILITY){
		/*an escape will happen. Now the decision is whether
		*to the partner, or to a random thread*/
		int partner = rand()%100;
		if(partner < ESPACE_TO_PARTNER){
			targetThread = getPartnerThread(thread);
		}else{
			targetThread = rand() % NUM_THREADS;
		}
	}else{
		// don't escape.
		targetThread = thread;
	}

	Object* temp;
	Object* parent = memManager->getRoot(thread, rootSlotNumber);
	int rnd, rnd2;

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

	int childRootSetSize = memManager->getRootsetSize(targetThread);
	int slotNumber;
	if(childRootSetSize){
		slotNumber = rand() % (childRootSetSize);
	}else{
		//printf("No object created for the targetThread\n");
		return;
	}
	Object* child = memManager->getRoot(targetThread, slotNumber);
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

	// create the reference picked
	if(parent != child){
		int cassign = rand()%100;
		if(cassign<80){
			slotNumber = rand() % parent->getPointersMax();
			memManager->setPointer(parent,slotNumber,child);
			log->logRefOperation(thread,parent->getID(),slotNumber,child->getID());
			log->logRefOperationClaasToObject(thread, parent->getClassId(), child->getID());
		}
		else{
			//slotNumber = rand() % parent->getPointersMax();
			//memManager->setPointer(parent,slotNumber,child);
			log->logRefOperationClaasToObject(thread, parent->getClassId(), child->getID());
		}
	}
}

/* Emulation of read operations in JVM
 * An object to be read may or may not be in the given thread;
 * Lets say (50% may)
 * If a thread has not allocated any object then it can read an object that is allocated by other
 *
 */

void Simulator::readObject(int thread){

	bool hasRootSet = true;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		hasRootSet = false;
		//printf("No object created for this threadNumber\n");
		//return;
	}

	if(!hasRootSet){
		// select an random object
		log->logReadOperation(thread, memManager->getARandomObjectID());
	}
	else{
		int rnd;
		rnd = rand()%100;
		if(rnd < THREADRATIO)
		{
			memManager->setupObjects();
			// find an object from the root set;
			Object* child = memManager->getRoot(thread, rootSlotNumber);
			Object* parent;

			// find an object randomly from the tree
			while(child){
				if(child->visited == 1){
					// this means that I might fall into a loop
					return;
				}else{
					child->visited = 1;
				}
				parent = child;
				rnd = rand() % parent->getPointersMax();
				child = parent->getReferenceTo(rnd);
			}

			log->logReadOperation(thread, parent->getID());
		}
		else{
			// select an random object that does not belongs to the given thread
			log->logReadOperation(thread, memManager->getARandomObjectID(thread));
		}
	}
}


Simulator::~Simulator() {
}

} /* namespace gcKons */
