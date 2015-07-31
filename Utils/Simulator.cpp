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

extern int NUM_THREADS ;
extern int RATIO_ALLOC_SET ;
extern int RATIO_READ_ACCESS ;
extern int RATIO_STORE_ACCESS ;
extern int NUM_CLAZZ;
extern int ROOT_DELETE_PROBABILITY;
extern int MAX_POINTERS;
extern int MAX_PRIMITIVES;
extern int STATIC_FIELD_ACCESS;
extern int PRIMITIVE_FIELD_ACCESS;
extern int MAXCACCESS;
extern int ESCAPE_PROBABILITY;
extern int ESPACE_TO_PARTNER;





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
		 id = memManager->allocateObject(size, outGoingRefsMax, parent, rnd, currentStep);
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


void Simulator::initializeClassTable(char* classfilename){
	memManager-> buildClassTable(NUM_CLAZZ);
	memManager->printClassTable(classfilename);
}

int Simulator::runTraceFileGenerator(int simulationSteps){

	stepsToGo = simulationSteps;
	currentStep = simulationSteps;
	bool operationdone = false;
	while(currentStep > 0){
		//if(operationdone){
		//	currentStep--;
		//}
		//print indicator showing the progress every 10000 iterations.
		if(currentStep%10000==0){
			printf("%f percent done.\n",(stepsToGo-currentStep)/(float)stepsToGo*100);
		}
		//which thread is executing?
		int thread = rand() % NUM_THREADS;
			
		//calculation of selecting a operation
		int i = RATIO_ALLOC_SET;
		int j = i + RATIO_STORE_ACCESS ;
		int k = j + RATIO_READ_ACCESS;
		int l = k + ROOT_DELETE_PROBABILITY;
		int x = rand()%100;
		//printf("i=%d j=%d k=%d l=%d x=%d\n", i,j,k,l,x);
		if( (0 <= x)&&(x <= i) ){
			//printf( "Select Allocation....\n");
			operationdone = allocationRandomObjectAARD(thread);
		}
		else if( ( i < x) && (x <= j) ){
			//printf( "Select Store....\n");
			int doStore = rand() % 100;
			int storeInObject = rand()%100;
			if(doStore <  PRIMITIVE_FIELD_ACCESS ){
				if(storeInObject > STATIC_FIELD_ACCESS){
					operationdone = storeObjectFiledWithPrimitive(thread);
				}
				else{
					operationdone = storeClassFiledWithPrimitive(thread);
				}
			}
			else{
				if(storeInObject > STATIC_FIELD_ACCESS){

					operationdone = setReferenceToObject(thread);
				}
				else{
					operationdone = setReferenceToClass(thread);
				}
			}
		}
		else if( ( j< x) && ( x <= k) ){
			//printf( "Select Read....\n");
			int doRead = rand() % 100;
			int readFromObject = rand()%100;
			if(doRead< PRIMITIVE_FIELD_ACCESS ){
				if(readFromObject > STATIC_FIELD_ACCESS){
					operationdone = readObjectFiledWithPrimitive(thread);
				}
				else{
					operationdone = readClassFiledWithPrimitive(thread);
				}
			}
			else{
				if(readFromObject > STATIC_FIELD_ACCESS){
					operationdone = readReferenceFromObject(thread);
				}
				else{
					operationdone = readReferenceFromClass(thread);
				}
			}
		}
		else if( ( k< x) && ( x <= l) ){
			//printf( "Select Add/DeleteRootset....\n");
			int addDel = rand() % 100;
			if(addDel>30){
				//add the pointer of an existing object to the root set of either the same thread or other thread
				operationdone = addReferenceToRootset(thread);
			}
			else{
				//delete the pointer of an existing object from the root set
				operationdone = deleteReferenceFromRootset(thread);
			}
		}
		else{
			;// do nothing
		}
	}
	log->logEnd();
	return 1;
}


bool Simulator::allocationRandomObjectAARD(int thread){
	
	int outGoingRefsMax = (rand() % MAX_POINTERS) + 1;
	int primitiveField = (rand() % MAX_PRIMITIVES) +1;
	int rootsetSize = memManager->getRootsetSize(thread);
	Object* newObject;

	//3.0
	// select an entry from class table;
	int clsIndex = rand()%NUM_CLAZZ;
	ClassObject* clsObj = memManager->getClassObject(clsIndex);
	// select class which has been acceessed (hit) less
	int cnt = 0;
	while(clsObj->getAccessCount()>MAXCACCESS){
		cnt++;
		clsIndex = rand()%NUM_CLAZZ;
		clsObj = memManager->getClassObject(clsIndex);
		if(cnt == 30){
			break;
		}
	}

	clsObj->increaseAccess();
	// allocate an object
	newObject = memManager->allocateObjectNew(thread, outGoingRefsMax, currentStep, clsObj->getId(), primitiveField); // 3.0
	memManager->addObjectToRootset(newObject, thread);

	log->logAllocation(thread, newObject->getID(), newObject->getPayloadSize(), outGoingRefsMax, clsObj->getId());
	currentStep--;
	log->addToRoot(thread, newObject->getID());
	currentStep--;

	// according to Gerhard
	/* allocation operations:
	* 10% ('allocate' followed by 'add' to root set) and
	* 90% ('allocate' followed by 'add' to root set and if possible ( 'reference' to the other object followed by 'delete' from root set)
	*/

	int doReference = rand()%100;
	if(doReference < RATIO_ALLOCATION){
		// to reference operation, rootset must contain object(s)
		if(rootsetSize){
			int rnd, rootSlotNumber;
			rootSlotNumber = rand() % rootsetSize;
			// Actually it is possible to have a circle in graph 
			//memManager->setupObjects();

			//int createRoot = 1; // that means there exists at the root set;

			Object* child;
			Object* parent;

			child = memManager->getRoot(thread, rootSlotNumber);
			parent = child;
			// how here find an object randomly from the tree
			while(child){
				
				//if(child->visited == 1){
				//	// this means that I might fall into a loop
				//	return true;
				//}else{
				//	child->visited = 1;
				//}
				parent = child;
				//createRoot++;
				//printf("I am here in loop %d\n", createRoot);
				rnd = rand() % parent->getPointersMax();
				child = parent->getReferenceTo(rnd);
				cnt = rand() % 100;
				if(cnt < 30){
					break;
				}
			}
			if(parent != NULL){
				// do reference operation; just set pointer parent to child
				memManager->setPointer(parent,rnd, newObject);
				//int fieldIndex = rand()%outGoingRefsMax;
				
				//log->logRefOperation(thread,parent->getID(),rnd, newObject->getID());
				log->logRefOperation(thread,parent->getID(),rnd, newObject->getID(), parent->getFieldOffset(rnd, REFTYPE), (int)8, (int)rand()%2 );
				currentStep--;

				// delete the pointer of the newObject from the root set;
				// newObject was actually added to the end of the rooset, so it can be deleted from the last
				memManager->deleteEndFromRootset(thread);
				log->deletefromRoot(thread, newObject->getID());
				currentStep--;
			}
		}
	}

	return true;
}

bool Simulator::addReferenceToRootset(int thread){

	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;
	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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
		return false;
	}
	
	if( !memManager->addExObjectToRootset(object, targetThread)){
		log->addToRoot(targetThread, object->getID());
		currentStep--;
	}
	return true;
}

bool Simulator::storeObjectFiledWithPrimitive(int thread){

	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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
	int primType = parent->primitiveType(primIndex); //rand()%3; 

	log->logstoreObjFieldWithPrimOperation(thread, parent->getID(), parent->getFieldOffset(primIndex, primType), parent->getFieldSize(primType), (int)rand()%2 );
	currentStep--;
	return true;
}



bool Simulator::readObjectFiledWithPrimitive(int thread){

	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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
	//int primType = rand()%3; // wrong
	int primType = parent->primitiveType(primIndex);
	log->logreadObjFieldWithPrimOperation(thread, parent->getID(), parent->getFieldOffset(primIndex, primType), parent->getFieldSize(primType), (int)rand()%2 );
	currentStep--;
	return true;
}


bool Simulator::setReferenceToObject(int thread){
	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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
		return false;
	}
	
	/* Actually there might have cycles */
	//memManager->setupObjects();

	Object* child = memManager->getRoot(targetThread, slotNumber);
	temp = child;
	while(temp){
		child = temp;
		/*
		if(child->visited == 1){
			// this means that I might fall into a loop
			return false;
		}
		else{
			child->visited = 1;
		}*/
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
		currentStep--;
		return true;
	}
	else{
		return false;
	}
}


bool Simulator::readReferenceFromObject(int thread){
	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
	}

	Object* temp;
	Object* child;
	Object* parent = memManager->getRoot(thread, rootSlotNumber);
	int rnd, rnd2;

	temp = parent;
	child = parent;

	while(temp){
		parent = child;
		child = temp;

		rnd = rand() % child->getPointersMax();
		temp = child->getReferenceTo(rnd);

		//if I would like to stop, I stop
		rnd2 = rand() % 100;
		if(rnd2 < 30){
			break;
		}
	}


	int slotNumber = rand() % parent->getPointersMax();
	temp = parent->getReferenceTo(slotNumber);
	while(temp==NULL){
		slotNumber = rand() % parent->getPointersMax();
		temp = parent->getReferenceTo(slotNumber);
		//if I would like to stop, I stop
		rnd2 = rand() % 100;
		if(rnd2 < 30){
			break;
		}
	}
	if(temp){
		log->logreadRefFromObjectOperation(thread,parent->getID(), slotNumber, (int)8, (int)rand()%2 );
		currentStep--;
		return true;
	}
	else{
		return false;
	}

}

bool Simulator::deleteReferenceFromRootset(int thread){
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
	}
	Object* obj = memManager->getRoot(thread, rootSlotNumber);
	//printf("Obj id: %d\n", obj->getID());
	memManager->deleteFromRootset(thread, rootSlotNumber);
	log->deletefromRoot(thread, obj->getID());
	currentStep--;
	return true;
}

bool Simulator::setReferenceToClass(int thread){

	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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

	int pClassId = parent->getClassId();
	ClassObject* clsObj = memManager->getClassObject(pClassId-1);
	if(clsObj->getStaticRefCount() !=0 ){
		int slotNumber = rand()% clsObj->getStaticRefCount();
		clsObj->setReference(slotNumber, parent);
		log->logRefOperationClaasToObject(thread, clsObj->getId(), clsObj->getFieldOffset(slotNumber, REFTYPE), parent->getID(), (int)8, (int)rand()%2);
		currentStep--;
		return true;
	}
	else{
		return false;
	}

}

bool Simulator::readReferenceFromClass(int thread){

	int targetThread;
	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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

	int pClassId = parent->getClassId();
	
	ClassObject* clsObj = memManager->getClassObject(pClassId-1);
	if(clsObj->getStaticRefCount() !=0 ){
		int slotNumber = clsObj->getReferenceSlot();
		if(slotNumber >= 0){
			log->logreadRefFromClaas(thread, clsObj->getId(), clsObj->getFieldOffset(slotNumber, REFTYPE), (int)8, (int)rand()%2);
			currentStep--;
			return true;
		}
	}
	return false;
}


bool Simulator::storeClassFiledWithPrimitive(int thread){

	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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

	int parentClassId =  parent->getClassId();
	ClassObject* clsObj = memManager->getClassObject(parentClassId-1);

	int primIndex = rand() % clsObj->getNumPrimitives();
	int primType = clsObj->primitiveType(primIndex);
	log->logstoreClassFieldWithPrimOperation(thread, clsObj->getId(), clsObj->getFieldOffset(primIndex, primType), clsObj->getFieldSize(primType), (int)rand()%2 );
	currentStep--;
	return true;
}

bool Simulator::readClassFiledWithPrimitive(int thread){

	int rootSetSize = memManager->getRootsetSize(thread);
	int rootSlotNumber;

	if(rootSetSize){
		rootSlotNumber = rand() % (rootSetSize);
	}else{
		//printf("No object created for this thread\n");
		return false;
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

	int parentClassId =  parent->getClassId();
	ClassObject* clsObj = memManager->getClassObject(parentClassId-1);

	int primIndex = rand() % clsObj->getNumPrimitives();
	int primType = clsObj->primitiveType(primIndex);
	log->logreadClassFieldWithPrimOperation(thread, clsObj->getId(), clsObj->getFieldOffset(primIndex, primType), clsObj->getFieldSize(primType), (int)rand()%2 );
	currentStep--;
	return true;
}

Simulator::~Simulator() {
}

} /* namespace gcKons */
