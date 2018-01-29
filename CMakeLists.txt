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

set (CONVEXIFY_VERSION_MAJOR 0)
set (CONVEXIFY_VERSION_MINOR 0)
set (CONVEXIFY_VERSION_PATCH 1)

# aux_source_directory(source/convexify, convexify_source)
# include_directories(source/convexify/extern/*)

# add_library(objects OBJECT ${convexify_source})

add_executable(convexifyEXE test.cpp)
add_custom_target(  convexifyDLL ALL
                    SOURCES test.cpp)

set_target_properties(convexifyEXE PROPERTIES OUTPUT_NAME convexify.exe)
set_target_properties(convexifyDLL PROPERTIES OUTPUT_NAME convexify.pyd)

