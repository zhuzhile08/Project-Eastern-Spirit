cmake_minimum_required(VERSION 3.24.0)
set(CMAKE_CXX_STANDARD 23)

project(Miniaudio VERSION 0.8.0)


# add headers
add_library(Miniaudio INTERFACE)
add_library(Miniaudio_Headers INTERFACE)
add_library(Miniaudio::Headers ALIAS Miniaudio_Headers)

target_include_directories(Miniaudio_Headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


# tests
option(LSD_BUILD_TESTS "Build tests for Miniaudio" OFF)
if (LSD_BUILD_TESTS)
	add_subdirectory("Tests")
endif()
