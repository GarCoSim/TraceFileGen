/*
 * MemoryManager.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: kons
 */


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "MemoryManager.h"
//#include "../Utils/configReader.h"
#include "Object.h"

using namespace std;

extern int NUM_THREADS ;
extern int ROOTSET_SIZE ; 


namespace traceGen {

MemoryManager::MemoryManager() {

	rootset.resize(NUM_THREADS);
	nextId = 1;
	/* In old version, root set size was fixed that requires the following initializations */
	/* In new version, root set size can be changed dynamically */
	if(VERSION == 0){
		int i;
		//init all rootsets
		for(i = 0; i < NUM_THREADS ; i++){
			rootset.at(i).resize(ROOTSET_SIZE);
		}
		objectList.resize(NUM_THREADS*ROOTSET_SIZE);
	}

}

int MemoryManager::setRootPointer(int threadNumber, int rootsetNumber, Object* newObject){
	if(rootsetNumber >= ROOTSET_SIZE || threadNumber >= NUM_THREADS){
		return -1;
	}
	rootset[threadNumber][rootsetNumber] = newObject;
	return 0;
}

int MemoryManager:: setRootPointer(int threadNumber, Object* newObject){
	if(threadNumber >= NUM_THREADS){
			return -1;
		}
	rootset[threadNumber].push_back(newObject);
	return 0;
}

int MemoryManager::allocateObjectToRootset(int size, int threadNumber,
		int rootSetNumber, int maxPointers, int creationDate){
	//check if you can create more objects and get a slot in object list
	int listSlot = getListSlot();
	if(listSlot<0){
		printf("listError\n");
		fflush(stdout);
		return 0;
	}
	//create new Object
	//printf("creating root with id: %d\n", nextId);
	int id = nextId;
	nextId++;
	Object* newObject = new Object(id,size,maxPointers, creationDate);
	objectList[listSlot] = newObject;
	//set the rootset pointer as desired
	setRootPointer(threadNumber, rootSetNumber, newObject);
	return id;
}

int MemoryManager::allocateObject(int size, int maxPointers,
		Object* creatorObject, int pointerIndex, int creationDate){
	//check if you can create more objects and get a slot in object list
	int listSlot = getListSlot();
	if(listSlot<0){
		printf("listError\n");
		fflush(stdout);
		return 0;
	}
	//create new Object
	//printf("creating new Object with id: %d\n\n", nextId);
	int id = nextId;
	nextId++;
	Object* newObject = new Object(id,size,maxPointers, creationDate);
	objectList[listSlot] = newObject;
	//set the rootset pointer as desired
	setPointer(creatorObject, pointerIndex, newObject);
	return id;
}

Object* MemoryManager::allocateObject(int size, int threadNumber, int maxPointers,int creationDate, int classID){

	int objectID = nextId;
	nextId++;
	Object* newObject = new Object(objectID, size, maxPointers, creationDate, classID);
	//new
	newObject->setThreadID(threadNumber);
	return newObject;
}

int MemoryManager::setPointer(Object* startObject, int pointerIndex, Object* targetObject){
	return startObject->setPointer(pointerIndex,targetObject);
}

int MemoryManager::deletePointer(Object* startObject, int pointerNumber){
	return startObject->setPointer(pointerNumber, NULL);
}

Object* MemoryManager::getObjectByID(int id){
	int i;
	Object* temp;
	for(i=0;(unsigned int)i<objectList.size();i++){
		temp = objectList[i];
		if(temp){
			int currentId = temp->getID();
			if(id == currentId){
				return temp;
			}
		}
	}
	return NULL;
}

Object* MemoryManager::getRoot(int threadNumber, int rootSlotNumber){
	return rootset[threadNumber][rootSlotNumber];
}

void MemoryManager::deleteRoot(int thread, int root){
	rootset[thread][root] = NULL;
}

void MemoryManager::deleteEndFromRootset(int threadNumber){
	int deleteIndex = rootset[threadNumber].size()-1;
	rootset[threadNumber].erase(rootset[threadNumber].begin()+deleteIndex);
}
void MemoryManager::deleteFromRootset(int threadNumber, int rootSlotNumber){
	rootset[threadNumber].erase(rootset[threadNumber].begin()+rootSlotNumber);
}

int MemoryManager::getListSlot(){
	unsigned int i;
	for(i = 0 ; i < objectList.size() ; i++){
		if(!objectList.at(i)){
			return i;
		}
		if(i+1 == objectList.size()){
			objectList.resize(objectList.size()*2);
		}
	}
	printf("listSlotERror\n");
	fflush(stdout);
	return -1;
}

int MemoryManager::isRoot(int thread, Object* obj){
	int i;
	for(i = 0 ; i < ROOTSET_SIZE ; i++){
		if(rootset[thread].at(i) && obj == rootset[thread].at(i)){
			return 1;
		}
	}
	return 0;
}

int MemoryManager::getRootsetNumberByReference(int thread, Object* obj){
	int i;
	for(i = 0 ; i < ROOTSET_SIZE ; i++){
		if(rootset[thread].at(i) && obj == rootset[thread].at(i)){
			return i;
		}
	}
	return -1;
}

void MemoryManager::visualizeState(){
	FILE* file = fopen("state.dot", "w");
		fprintf(file, "digraph G { \n");
		int i;
		int j;
		int k;
		//create threads and init pointers
		for(i = 0 ; i < NUM_THREADS ; i++){
			fprintf(file, "T%d;\n", i);
			for(j = 0 ; j < ROOTSET_SIZE ; j++){
				if(rootset.at(i).at(j)){
					fprintf(file, "T%d -> %d;\n", i, rootset.at(i).at(j)->getID());
				}
			}
		}

		for(k = 0 ; (unsigned int)k < objectList.size() ; k++){
			//Object* parent = objectList.at(k);
			if(objectList.at(k)){
				for(i = 0; i < objectList.at(k)->getPointersMax() ;i++){
					Object* child = objectList.at(k)->getReferenceTo(i);
					if(child){
						fprintf(file, "%d -> %d;\n",objectList.at(k)->getID(), child->getID());
					}
				}
			}
		}
		fprintf(file, "}\n");
		fclose(file);

}

void MemoryManager::setupObjects(){
	int i;
	Object* temp;
	for(i = 0 ; (unsigned int)i < objectList.size() ; i++){
		temp = objectList[i];
		if(temp){
			temp->visited = 0;
		}
	}
}

int MemoryManager::getRootsetSize(int threadNumber){
	if(threadNumber<(int)rootset.size()){
			return rootset[threadNumber].size();
		}else{
			return 0;
		}
}

void MemoryManager::addObjectToRootset(Object* newObject, int threadNumber){
	// add new object to the object list
	objectList.push_back(newObject);
	//add object to the root set
	setRootPointer(threadNumber, newObject);
}

bool MemoryManager::isObjectInRoot(int thread, Object* obj){
	int i;
	for(i = 0 ; i <(int) rootset[thread].size();  i++){
		if(rootset[thread].at(i) && obj == rootset[thread].at(i)){
			return true;
		}
	}
	return false;
}

int MemoryManager::getARandomObjectID(){
	int rnd;
	rnd = rand()% ((int)objectList.size());
	return objectList[rnd]->getID();
}

int MemoryManager::getARandomObjectID(int thread){
	int rnd;
	rnd = rand()% ((int)objectList.size());
	int oid, otd;
	oid = objectList[rnd]->getID();
	otd = objectList[rnd]->getThreadID();

    /* randomly select object but never try more than the number of objects created*/
	int i=0;
	while(i < (int)objectList.size()){
		if(thread != otd){
			break;
		}
		else{
			rnd = rand()% ((int)objectList.size());
			oid = objectList[rnd]->getID();
			otd = objectList[rnd]->getThreadID();
			//cout<<"Here"<<endl;
		}
		i++;
	}
	//cout<<"Here"<<endl;
	return oid;
}

MemoryManager::~MemoryManager() {
}

} /* namespace gcKons */
