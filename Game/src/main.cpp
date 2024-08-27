#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>

#include <ETCS/ETCS.h>

#include <glm/glm.hpp>
#include <stb_image.h>

#include <cstdio>

static constexpr std::size_t windowWidth = 320;
static constexpr std::size_t windowHeight = 240;
static constexpr std::size_t windowScale = 2;

namespace {

}

int main(int argc, char** args) {
	bool running = true;

	SDL_Event e;
	SDL_Window* window = { };
	SDL_Renderer* renderer = { };

	SDL_Texture* textureAtlas = { };
	SDL_FRect textureRect = { };
	glm::ivec3 textureDim = { }; // z represents the channels

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == SDL_FALSE) {
		std::printf("Failed to initialize SDL with error: \"%s\"", SDL_GetError());
		return 0;
	}

	etcs::init();


	if (SDL_CreateWindowAndRenderer(
		"車万朱元璋", 
		windowWidth * windowScale, windowHeight * windowScale, 
		SDL_WINDOW_RESIZABLE, &window, &renderer) == SDL_FALSE) {
		std::printf("Failed to create SDL Window and/or Renderer with error: \"%s\"", SDL_GetError());
		return 0;
	}

	auto pixels = stbi_load("/Users/zhilezhugd/Programming/C++/Project-Eastern-Spirit/out/build/Debug/Game/Data/Sheet.png", &textureDim.x, &textureDim.y, &textureDim.z, STBI_rgb_alpha);
	if (pixels == nullptr) {
		std::printf("Failed to load sprite sheet!");
		return 0;
	}

	std::printf("%i\n", textureDim.z);

	if (textureAtlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, textureDim.x, textureDim.y); textureAtlas == nullptr) {
		std::printf("Failed to create SDL Texutre with error: \"%s\"", SDL_GetError());
		return 0;
	}

	textureRect = SDL_FRect { 0, 0, static_cast<float>(textureDim.x), static_cast<float>(textureDim.y) };


	if (SDL_Rect dim { 0, 0, textureDim.x, textureDim.y }; SDL_UpdateTexture(textureAtlas, &dim, pixels, textureDim.x * textureDim.z) == SDL_FALSE) {
		std::printf("Failed to update SDL Texutre with error: \"%s\"", SDL_GetError());
		return 0;
	}

	stbi_image_free(pixels);





	while (running) {
		while (SDL_PollEvent(&e) == SDL_TRUE) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;

				break;
			}
		}

		SDL_RenderClear(renderer);

		SDL_RenderTexture(renderer, textureAtlas, &textureRect, &textureRect);

		SDL_RenderPresent(renderer);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();

	etcs::quit();
}
