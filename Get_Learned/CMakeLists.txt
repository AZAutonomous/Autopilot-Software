# Author: James Murphy
# NetID: jamesmurphy
# Date: 9-27-2017

# A simple example CMake Setup for Compiling C++ Programs. This setup utilizes a single
# subdirectory (src) in which all CPP source (.cpp) and header (.h) files within that 
# directory are included within the project.

# CMake projects are forward compatible, but may not be backwards compatible. This
# commands defines the version of CMake used for the current setup.

cmake_minimum_required(VERSION 3.1)

# Define the project name. 

project (Get_Learned_project)

# Add a subdirectory to the project. The CMakeLists.txt file in that subdirectory
# will be used for further setting up the project.

add_subdirectory( src )