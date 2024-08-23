cmake_minimum_required (VERSION 3.24.0)

project(BuildLibraries)

# add all libraries that need to be built
# add_subdirectory("openal-soft")
if (BUILD_STATIC)
	option(SDL_SHARED "Build a shared version of the library" OFF)
	option(SDL_STATIC "Build a static version of the library" ON)
else ()
	option(SDL_SHARED "Build a shared version of the library" ON)
	option(SDL_STATIC "Build a static version of the library" OFF)
endif ()

add_subdirectory("sdl")

# GLM and VMA aren't build here because they only build the tests, 
# which is not necceceary for the library to function 
