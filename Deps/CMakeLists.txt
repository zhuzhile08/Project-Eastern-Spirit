cmake_minimum_required (VERSION 3.24.0)

project(BuildLibraries)

set(ETCS_ENABLE_COMPONENTS_EXT ON CACHE BOOL "Use the already implemented components as an extension" FORCE)

# add all libraries that need to be built
# add_subdirectory("openal-soft")
if (BUILD_STATIC)
	set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
	set(SDL_STATIC ON CACHE BOOL "Build a static version of the library" FORCE)
else ()
	set(SDL_SHARED ON CACHE BOOL "Build a shared version of the library" FORCE)
	set(SDL_STATIC OFF CACHE BOOL "Build a static version of the library" FORCE)
endif ()

add_subdirectory("lsd")
add_subdirectory("glm")
add_subdirectory("sdl")
add_subdirectory("etcs")
add_subdirectory("miniaudio")
