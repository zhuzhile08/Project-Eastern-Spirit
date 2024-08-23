cmake_minimum_required(VERSION 3.24.0)

project(Game VERSION 0.5.0)

# includes and stuff
include_directories(
PUBLIC
	# library local include directory
	${ENGINE_INCLUDE_DIR}

	# graphics and windowing libraries
	${LIBRARY_PATH}/sdl/include/

	# math and physics libraries
	${LIBRARY_PATH}/glm/

	# audio libraries
	# ${LIBRARY_PATH}/openal-soft/include/

	# utility libraries
	${LIBRARY_PATH}/stb/
	${LIBRARY_PATH}/lsd/
)

set (GAME_SOURCE_FILES 
	"src/main.cpp"
)

# building the game
add_executable(Game
	"${GAME_SOURCE_FILES}"
)


# link some libraries
target_link_libraries(Game
LINK_PUBLIC
	# graphics and windowing libraries
	SDL3::SDL3-static

	# audio libraries
	# OpenAL::OpenAL
)

target_precompile_headers(Game
PUBLIC
	<typeindex>
	<type_traits>
	<algorithm>
	<chrono>
	<cmath>
	<cstdio>
	<cstdint>
	<cstdlib>
	<deque>
	<filesystem>
	<fstream>
	<functional>
	<iostream>
	<initializer_list>
	<map>
	<set>
	<stddef.h>
	<stdexcept>
	<stdint.h>
	<type_traits>
	<utility>
	<wchar.h>
)
