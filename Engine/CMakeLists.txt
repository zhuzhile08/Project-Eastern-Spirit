cmake_minimum_required(VERSION 3.24.0)

project(Engine VERSION 0.1.0)

include_directories(
PUBLIC
	# local include directory
	${CMAKE_CURRENT_SOURCE_DIR}/include/

	# graphics and windowing libraries
	Vulkan::Headers

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

	"src/Common/Filesystem.cpp"

	"src/Components/Background.cpp"
	"src/Components/Camera.cpp"
	"src/Components/Sprite2D.cpp"
	"src/Components/Sprite3D.cpp"
	"src/Components/Animator.cpp"
	"src/Components/Dispenser.cpp"
	"src/Components/Health.cpp"
	"src/Components/Hitbox.cpp"
	"src/Components/Hurtbox.cpp"
	"src/Components/Path.cpp"
	"src/Components/TextBox.cpp"
	"src/Components/Trail.cpp"

	"src/Graphics/Font.cpp"
	"src/Graphics/RenderSystem.cpp"
	"src/Graphics/Texture.cpp"
	"src/Graphics/Window.cpp"

	"src/Application.cpp"
	"src/InputSystem.cpp"
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
		# OpenAL::OpenAL

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
