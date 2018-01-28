cmake_minimum_required (VERSION 2.6)
project(Convexify)

set (CONVEXIFY_VERSION_MAJOR 0)
set (CONVEXIFY_VERSION_MINOR 0)
set (CONVEXIFY_VERSION_PATCH 1)


add subdirectory(source/convexify)


add_executable(convexify.exe convexify.cxx)
add_library(convexify.pyd convexify.cxx)
