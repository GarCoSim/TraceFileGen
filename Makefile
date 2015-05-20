all:
	g++ ObjectClasses/MemoryManager.cpp ObjectClasses/Object.cpp Utils/Logger.cpp Utils/Simulator.cpp main.cpp -g  -o traceGen
