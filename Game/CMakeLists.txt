cmake_minimum_required(VERSION 3.24.0)

project(Game VERSION 0.5.0)

# includes and stuff
include_directories(
PUBLIC
	# library local include directory
	${ENGINE_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include/

	# audio libraries
	# ${LIBRARY_PATH}/openal-soft/include/

	# utility libraries
	${LIBRARY_PATH}/stb/
)

set (GAME_SOURCE_FILES 
	"src/Controllers/Player.cpp"

	"src/Game.cpp"
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
	Engine::Headers
	Engine::Engine
	SDL3::Headers
	SDL3::SDL3-static

	# audio libraries
	# OpenAL::OpenAL

	# math libraries
	glm::glm

	# utility libraries
	LyraStandardLibrary::Headers
	ETCS::Headers
	ETCS::ETCS-static
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
