all:
	g++ ObjectClasses/MemoryManager.cpp ObjectClasses/ClassObject.cpp ObjectClasses/Object.cpp Utils/Logger.cpp Utils/Simulator.cpp main.cpp -g  -o traceGen
