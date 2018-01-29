macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

cmake_minimum_required (VERSION 2.6)
project(Convexify)

set(CONVEXIFY_VERSION_MAJOR 0)
set(CONVEXIFY_VERSION_MINOR 0)
set(CONVEXIFY_VERSION_PATCH 1)

# add source
file(GLOB SOURCES "source/convexify/*.cpp")

# add includes
include_directories("source/convexify/extern/python/include/")

# external libraries
add_library(Python STATIC IMPORTED GLOBAL)
set_property(TARGET Python PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/source/convexify/extern/python/python37.lib")
add_library(Python_d STATIC IMPORTED GLOBAL)
set_property(TARGET Python_d PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/source/convexify/extern/python/python37_d.lib")

# configure output location
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/built)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

# declare binary outputs
add_library(convexifyDLL SHARED ${SOURCES})
add_executable(convexifyEXE ${SOURCES})

# configure compiler/linker

set (Convexify_ARCHITECTURE "x64")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

set_target_properties(  convexifyEXE PROPERTIES
                        OUTPUT_NAME convexify
                        SUFFIX ".exe")

set_target_properties(  convexifyDLL PROPERTIES
                        OUTPUT_NAME convexify
                        SUFFIX ".pyd")

#link libraries to output binaries
target_link_libraries(convexifyEXE Python)
target_link_libraries(convexifyEXE Python_d)

target_link_libraries(convexifyDLL Python)
target_link_libraries(convexifyDLL Python_d)
