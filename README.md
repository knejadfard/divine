# Description
Divine - the tiny file splitter and joiner written in C++.

# Future Road
Originally written in 2009, the code is not compliant to the best C++11 practices. The following items are considered for the next revision:
* Refactoring the code based on object oriented design.
* Migrating from Pthreads library to C++11 threading facilities.
* Developing a GUI front-end using Qt.

# How to build
The build system used for this project is CMake. To build the project in GNU/Linux, simply do the following:
  > mkdir build
  
  > cd build
  
  > cmake ../
  
By executing the above commands, the build process happens in the ./build directory. The resulting executable is a test program.

# Using as a library
Divine is written as a small header-only library. To use it as a library, simply including the header does the job. Don't forget to link with the Pthreads library (use the default CMakeLists.txt as an example of how you can link the library). In the next revision, this dependency is going to be eliminated by using C++11 threads.
