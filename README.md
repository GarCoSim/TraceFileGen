# TraceFileGen

This work is under improvement, fine-tuning, and documentation.

#Trace File
 To be documented

#Trace File Format and Specification
 To be documented
 	

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
 ObjectClasses	| C++ header and sources
 Utils			| C++ header and sources
 Documentation	| documentation
   
# Installing (Initial Build)
  After downloading, followed by extracting or cloning, do the following:
  * $ cd TraceFileGen
  * $ ./build.sh

# Compiling (After Initial Build)
  After initial build, do the following:
  * $ cmake ..
  * $ make
  
# Running (targetGen - the executable in the build folder)
Usage: At least one argument must be provided to run targetGen
* The name of trace file to be generated (such as traceFile)

Example: $./targetGen traceFile

Options:

  Argument				| Short			 | Description
 ---------------------- | -------------- | -----------
 --operations x  	    | -o x			 | x is an integer, number of total memory management operations to be created in a trace file (default: 100) 
 --thread x			    | -t x			 | x is an integer, number of threads (default: 10) 
 --class x			    | -c x      	 | x is an integer, number of classes (default: 300) 
 --pointers x			| -p x 			 | x is an integer, maximum number of pointer fields in an object or static pointer fields in a class (default: 10) 
 --primitives x			| -pm x     	 | x is an integer, maximum number of primitive fields in an object or static primitive fields in a class (default: 6) 
 --allocation x			| -a x      	 | x is an integer, ratio of allocation in total MM operations (default: 2) 
 --storeaccess x 		| -s x     		 | x is an integer, ratio of store operations in total MM operations (default: 11) 
 --readaccess x			| -r x      	 | x is an integer, ratio of read operations in total MM operations (default: 88) 
 --deleteroot x			| -d x      	 | x is an integer, ratio of root delete operations in total MM operations (default: 10) 
 --static x				| -sf x     	 | x is an integer, ratio of static field access (store/read) from a class(default: 30) 
 --prifaccess x			| -pfa x    	 | x is an integer, ratio of primitive field access (store/read) from either a object or a class  (default: 70) 
 --escape x				| -e x      	 | x is an integer, ratio of an object being escaped (default: 12) 
 --esctopartner x		| -etp x  		 | x is an integer, ratio of an object being escaped to partner thread (default: 90)

 # Output (After after running)
	traceGen will generate three files
	* *.trace --- containing a sequence of memory management operations
	* *.cls   --- containing a lists of classes
	* *.log   --- containing statistical data, where * is the name of the trace file specified on the command line (e.g., "traceFile")
	