# TraceFileGen

# Obtaining TraceFileGen
TraceFileGen can be downloaded from https://github.com/GarCoSim/TraceFileGen

# Required Dependencies
  TraceFileGen has the following requirements
  * CMake is required to compile the sources. CMake can be obtained from https://cmake.org/
  * g++ (GCC) 4.4.7 or newer is required to compile the sources, visit https://gcc.gnu.org/

# Overview of the Directories Structure
  The following table lists the names of all directories and a short description.
  
 Name    		| Description
 -------------- | -----------
 build   		| build folder which is created by the ./build.sh script
 ObjectClasses	| C++ sources
 Utils			| C++ sources
   
# Installation (Initial Building)
  After downloading followed by extracting or cloning, do the followings
  * $ cd TraceFileGen
  * $ ./build.sh

# Compiling (After Initial Build)
  After intial building, do the followings
  * $ cmake ..
  * $ make
  
# Running (targetGen - the executable in the build folder)
Usage: At least one argument must be provided to run targetGen
* The name of trace file to be generated ( such as traceFile)

Example: $./targetGen traceFile

Options:

  Arguments			| Short		| Description
 ------------------	| ----------| -----------
 --iteration x  	| -i x		| x number as the number of threads (default: 10)
 --thread x			| -t x		| x number as the number of threads (default: 10)\n" \
 --class x			| -c x      | x number as the number of classes (default: 300)\n" \
 --pointers x		| -p x 		| x number as the maximum number of pointer fields in objects/static pointer fields in classes (default: 10)\n" \
 --primitives x		| -pm x     | x number as the maximum number of primitive fields in objects/static primitive fields in classes (default: 6)\n" \
 --allocation x		| -a x      | x percent as the allocation (default: 2)\n" \
 --storeaccess x 	| -s x     	| x percent as the store (default: 11)\n" \
 --readaccess x		| -r x      | x percent as the read (default: 88)\n" \
 --deleteroot x		| -d x      | x percent as the rootdelete (default: 10)\n" \
 --static x			| -sf x     | x percent as the static field access (default: 30)\n" \
 --prifaccess x		| -pfa x    | x percent as the primitive field access (default: 70)\n" \
 --classaccess x	| -ca x    	| x number as the maximum used a class to create objects (default: 300)\n" \
 --escape x			| -e x      | x probability as the an object to be escaped (default: 12)\n" \
 --esctopartner x	| -etp x  	| x probability as an object to be escaped to partner thread (default: 90)\n" \
 -- maxWeight x		| -maxW x   | x maximum weight of a class (default: 25)\n" \
 -- maxFrequency x	| -maxF x   | x maximum frequency of a class in object allocation (default: 25)\n" \
 --constantValue x	| -cvalue x | x constant for a where {y = ab^x} (default: 10)\n" \
 --slop x			| -slp x  	| x tangent of a line value between 0 t0 1 (default: b=.5)\n" \
  
# Output (After after running)
	It will generate three files
	* *.trace --- containing a sequence of memory management operations 
	* *.cls   --- containing a lists of classes
	* *.log   --- containing statistical data
	