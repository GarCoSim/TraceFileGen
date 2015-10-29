# TraceFileGen

# Obtaining TraceFileGen
TraceFileGen can be downloaded from https://github.com/GarCoSim/TraceFileGen

# Required Dependencies
  TraceFileGen has the following requirements
  * CMake is required to compile the sources. CMake can be obtained from https://cmake.org/
  * g++ (GCC) 4.4.7 or newer is required to compile the sources, visit https://gcc.gnu.org/

# Overview of the Directories Structure
  The following table lists the names of all directories and a short description.
  <table>
  <tbody>
    <tr>
      <td>
      <div>Name</div>
      </td>
      <td>
      Description.
      </td>
      </tr>
    <tr>
      <td>
      <div>build</div>
      </td>
      <td>
      build folder which is created by the ./build.sh script 
      </td>
      </tr>
    <tr>
      <td>
      <div>ObjectClasses</div>
      </td>
      <td>
      C++ sources
      </td>
      </tr>
    <tr>
      <td>
      <div>Utils</div>
      </td>
      <td>
       C++ sources 
      </td>
      </tr>
  </tbody>
  </table>
  
# Installation (Initial Builing)
  After downloading followed by extracting or cloning, do the followings
  * $ cd TraceFileGen
  * $ ./build.sh

# Compiling (After Initial Build)
  After intial building, do the followings
  * $ cmake ..
  * $ make
  
# Running (targetGen - the executatble in the build folder)
Usage: At least two argumets must be provided to run targetGen
* The name of trace file to be generated ( such as traceFile.trace)
* The name of class list file to be generated (such traceFile.cls)

Example: $./targetGen traceFile.trace traceFile.cls

<u>Options:</u>
<table class="" cellspacing=0 border=1>
                <tbody>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr>Arguments</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>Short</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>Description</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --iteration x </nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-i x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x number as the number of iterations (default: 100)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --thread x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-t x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x number as the number of threads (default: 10)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --class x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-c x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x number as the number of classes (default: 300)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --pointers x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-p x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x number as the maximum number of pointer fields in objects/static pointer fields in classes (default: 10)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --primitives x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-pm x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x number as the maximum number of primitive fields in objects/static primitive fields in classes (default: 6)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --allocation x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-a x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x percent as the allocation (default: 2)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --storeaccess x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-s x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x percent as the store (default: 10)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --readaccess x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-r x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x percent as the read (default: 88)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --deleteroot x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-d x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x percent as the rootdelete (default: 10)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --static x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-sf x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x percent as the static field access (default: 30)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --prifaccess x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-pfa x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x percent as the primitive field access (default: 70)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --classaccess x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-ca x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x number as the maximum used a class to create objects (default: 300)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --escape x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-e x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x probability as the an object to be escaped (default: 20)</nobr>
                        </td>
                    </tr>
                    <tr style="height:20px;">
                        <td style="min-width:50px">
                            <nobr> --esctopartner x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>-etp x</nobr>
                        </td>
                        <td style="min-width:50px">
                            <nobr>x probability as an object to be escaped to partner thread (default: 90)</nobr>
                        </td>
                    </tr>
                </tbody>
            </table>

