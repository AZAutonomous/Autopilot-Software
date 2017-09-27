# Author: James Murphy	
# NetID: jamesmurphy
# Date: 9-27-2017

# Windows does weird things sometimes. This helps control the madness.
if(MSVC)
    # Sets the compilation flags to report warnings through Level 4. We used to use "-Wall" until
    # the MSVC compiler started freaking out all the time about secure scanf, etc.
    set( CMAKE_CXX_FLAGS "-W4" )
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
else()
    # adds debug information for all other OSs
    add_definitions(-Wall -g)
    link_libraries(m)
endif()

# The following file GLOB command will search for all files within the current directory
# that match the specified expressions (*.cpp and *.h), and assign the resulting files
# to the variable SRCS. Thus, SRCS is a list a all CPP source and header files in the 
# current directory.

file( GLOB SRCS *.cpp *.h )

# Define the target application executable and the list of C source
# and header files needed for the executable.

add_executable( Get_Learned ${SRCS} )