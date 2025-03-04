cmake_minimum_required(VERSION 3.24.0)

project(Engine VERSION 0.1.0)

include_directories(
PUBLIC
	# local include directory
	${CMAKE_CURRENT_SOURCE_DIR}/include/

	# math and physics libraries
	${LIBRARY_PATH}/glm/

	# audio libraries
	# ${LIBRARY_PATH}/openal-soft/include/

	# utility libraries
	${LIBRARY_PATH}/stb/
)

set (ENGINE_SOURCE_FILES 
	"src/Audio/Music.cpp"
	"src/Audio/SFX.cpp"

	"src/Core/Filesystem.cpp"
	"src/Core/Math.cpp"

	"src/Components/Camera.cpp"
	"src/Components/Colliders.cpp"
	"src/Components/ParticleSystem.cpp"
	"src/Components/Path.cpp"
	"src/Components/Sprite.cpp"
	"src/Components/SpriteAnimator.cpp"
	"src/Components/TextBox.cpp"
	"src/Components/TextBoxAnimator.cpp"
	"src/Components/TileMap.cpp"
	"src/Components/Trail.cpp"

	"src/Detail/RenderSystem.cpp"
	"src/Detail/PhysicsSystem.cpp"
	"src/Detail/InputSystem.cpp"

	"src/Graphics/AnimatorBuilder.cpp"
	"src/Graphics/Font.cpp"
	"src/Graphics/Texture.cpp"
	"src/Graphics/TileSet.cpp"
	"src/Graphics/Window.cpp"

	"src/Application.cpp"
	"src/Stage.cpp"
)

set (ENGINE_PRECOMPILED_HEADERS
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


if(BUILD_STATIC)
	add_library(Engine-static STATIC "${ENGINE_SOURCE_FILES}")
	add_library(Engine::Engine ALIAS Engine-static)
	add_library(Engine::Engine-static ALIAS Engine-static)

	if (WIN32) 
		target_compile_options(Engine-static PRIVATE /WX)
	else () 
		target_compile_options(Engine-static PRIVATE -Wall -Wextra -Wpedantic)
	endif ()

	target_link_libraries(Engine-static
	LINK_PRIVATE
		# graphics and windowing libraries
		SDL3::Headers
		SDL3::SDL3-static

		# audio libraries
		Miniaudio::Headers

		# utility libraries
		LyraStandardLibrary::Headers
		ETCS::Headers
		ETCS::ETCS-static
	)

	target_precompile_headers(Engine-static PRIVATE "${ENGINE_PRECOMPILED_HEADERS}")
else()
	add_library(Engine-shared STATIC "${ENGINE_SOURCE_FILES}")
	add_library(Engine::Engine ALIAS Engine-shared)
	add_library(Engine::Engine-shared ALIAS Engine-shared)

	if (WIN32) 
		target_compile_options(Engine-shared PRIVATE /WX)
	else () 
		target_compile_options(Engine-shared PRIVATE -Wall -Wextra -Wpedantic)
	endif ()

	target_link_libraries(Engine-shared
	LINK_PRIVATE
		# graphics and windowing libraries
		SDL3::Headers
		SDL3::SDL3-shared

		# audio libraries
		# OpenAL::OpenAL

		# utility libraries
		LyraStandardLibrary::Headers
		ETCS::Headers
		ETCS::ETCS-shared
	)

	target_precompile_headers(Engine-shared PRIVATE "${ENGINE_PRECOMPILED_HEADERS}")
endif()


add_library(Engine_Headers INTERFACE)
add_library(Engine::Headers ALIAS Engine_Headers)
target_include_directories(Engine_Headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
