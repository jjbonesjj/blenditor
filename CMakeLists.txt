cmake_minimum_required (VERSION 3.6)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

macro(set_out_dir dir)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${dir})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${dir})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${dir})

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${dir})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${dir})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${dir})

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${dir})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${dir})
endmacro()

set(CONVEXIFY_VERSION_MAJOR 0)
set(CONVEXIFY_VERSION_MINOR 0)
set(CONVEXIFY_VERSION_PATCH 1)

project(Convexify)

set(CGAL_DIR "./external/cgal/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./external/cgal/cmake/modules")
find_package(CGAL REQUIRED MODULE
            COMPONENTS CORE)

include( ${CGAL_USE_FILE} )

include_directories (BEFORE "./external/include")

# add source
file(GLOB SOURCES "source/convexify/*.cpp" "source/convexify/*.h")

# add includes
include_directories(SYSTEM "source/convexify/extern/python/include/")

# external libraries
add_library(Python STATIC IMPORTED GLOBAL)
set_property(TARGET Python PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/source/convexify/extern/python/python35.lib")
add_library(Python_d STATIC IMPORTED GLOBAL)
set_property(TARGET Python_d PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/source/convexify/extern/python/python35_d.lib")

# configure output location
set(EXE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/built)
set(DLL_OUTPUT_DIR ${CMAKE_BINARY_DIR}/source/addon/intern)



#common objs
add_library(convexify OBJECT ${SOURCES})

# declare binary outputs
set_out_dir(${DLL_OUTPUT_DIR})
add_library(convexifyDLL SHARED $<TARGET_OBJECTS:convexify>)
set_out_dir(${EXE_OUTPUT_DIR})
add_executable(convexifyEXE $<TARGET_OBJECTS:convexify>)

# configure compiler/linker

set (Convexify_ARCHITECTURE "x64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set_target_properties(  convexifyEXE PROPERTIES
                        OUTPUT_NAME convexify

                        # the exe MUST have the .pyd suffix for python to load it. 
                        SUFFIX ".pyd"

                        VS_STARTUP_PROJECT convexifyEXE

                        CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DLL_OUTPUT_DIR}
                        CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DLL_OUTPUT_DIR}
                        CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DLL_OUTPUT_DIR}
                    )

set_target_properties(  convexifyDLL PROPERTIES
                        OUTPUT_NAME convexify

                        # the dll MUST have the .pyd suffix for python to load it. 
                        SUFFIX ".pyd"
                        
                        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DLL_OUTPUT_DIR}
                        CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${DLL_OUTPUT_DIR}
                        CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${DLL_OUTPUT_DIR}

                        CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DLL_OUTPUT_DIR}
                        CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${DLL_OUTPUT_DIR}
                        CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${DLL_OUTPUT_DIR}

                        CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DLL_OUTPUT_DIR}
                        CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DLL_OUTPUT_DIR}
                        CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DLL_OUTPUT_DIR}
                    )

#link libraries to output binaries
target_link_libraries(convexifyEXE Python)
#target_link_libraries(convexifyEXE Python_d)
target_link_libraries(convexifyEXE ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

target_link_libraries(convexifyDLL Python)
#target_link_libraries(convexifyDLL Python_d)
target_link_libraries(convexifyDLL ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})