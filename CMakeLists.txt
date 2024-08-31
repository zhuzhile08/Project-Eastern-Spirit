# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.24.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.3" CACHE STRING "Minimum OS X deployment version")

project (ProjectEasternSprit)


# project options
option(BUILD_STATIC "Build the backend libraries statically" ON)
if (BUILD_STATIC)
	option(BUILD_SHARED_LIBS OFF)
else ()
	option(BUILD_SHARED_LIBS ON)
endif()


# 3rd party library path
set(LIBRARY_PATH ${CMAKE_SOURCE_DIR}/Deps)


# set the local include directory for the engine
set(ENGINE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Engine/include/)

# copy data to the binary folder
file(COPY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_BINARY_DIR}/Game/Data)


# set global options
set(CMAKE_CXX_STANDARD 23)

# Include sub-projects.
add_subdirectory ("Deps")
add_subdirectory ("Engine")
add_subdirectory ("Game")
