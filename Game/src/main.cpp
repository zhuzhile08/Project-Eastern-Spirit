#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL.h>

#include <ETCS/ETCS.h>

#include <glm/glm.hpp>
#include <stb_image.h>

#include <cstdio>
#include <random>
#include <map>

static constexpr std::size_t windowWidth = 320;
static constexpr std::size_t windowHeight = 240;
static constexpr std::size_t windowScale = 2;
static constexpr std::size_t fov = 90;

namespace {

struct SpriteRect {
	SDL_FRect rect;
};

struct DrawData {
	SDL_FRect src;
	SDL_FRect dst;
};

}

int main(int argc, char** args) {
	bool running = true;

	float fps = 1;
	float deltaTime = 0;
	std::uint64_t frameStart = 0;
	std::uint64_t frameTime = 0;

	std::random_device randDevice;
	std::default_random_engine randEngine(randDevice());

	SDL_Event e;
	SDL_Window* window = { };
	SDL_Renderer* renderer = { };

	SDL_Texture* textureAtlas = { };
	glm::ivec3 textureDim = { }; // z represents the channels

	float fovRatio = glm::tan(glm::radians(static_cast<float>(fov / 2)));

	std::map<int, lsd::Vector<DrawData>, std::greater<float>> drawData = { };


	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == SDL_FALSE) {
		std::printf("Failed to initialize SDL with error: \"%s\"", SDL_GetError());
		return 0;
	}

	etcs::init();

	std::srand(std::time(nullptr));


	if (SDL_CreateWindowAndRenderer(
		"車万朱元璋", 
		windowWidth * windowScale, windowHeight * windowScale, 
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_RESIZABLE, &window, &renderer) == SDL_FALSE) {
		std::printf("Failed to create SDL Window and/or Renderer with error: \"%s\"", SDL_GetError());
		return 0;
	}


	auto pixels = stbi_load("/Users/zhilezhugd/Programming/C++/Project-Eastern-Spirit/out/build/Debug/Game/Data/Sheet.png", &textureDim.x, &textureDim.y, &textureDim.z, STBI_rgb_alpha);
	if (pixels == nullptr) {
		std::printf("Failed to load sprite sheet!");
		return 0;
	}

	if (textureAtlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, textureDim.x, textureDim.y); textureAtlas == nullptr) {
		std::printf("Failed to create SDL Texutre with error: \"%s\"", SDL_GetError());
		return 0;
	}

	if (SDL_Rect dim { 0, 0, textureDim.x, textureDim.y }; SDL_UpdateTexture(textureAtlas, &dim, pixels, textureDim.x * textureDim.z) == SDL_FALSE) {
		std::printf("Failed to update SDL Texutre with error: \"%s\"", SDL_GetError());
		return 0;
	}

	stbi_image_free(pixels);


	std::uniform_int_distribution<int> randXGen(-50, 50);
	std::uniform_int_distribution<std::size_t> randYGen(0, 64);
	std::uniform_int_distribution<int> randZGen(-50, 50);

	for (size_t i = 2049; i > 1; i--) {
		auto e = etcs::insertEntity();

		e.insertComponent<etcs::Transform>(glm::vec3(randXGen(randEngine), randYGen(randEngine), randZGen(randEngine)));
		//e.insertComponent<etcs::Transform>(glm::vec3(1, 10, 1));
		e.insertComponent<SpriteRect>();

		if (i % 17 == 0) e.component<SpriteRect>().rect = { 0, 0, 16, 16 };
		if (i % 13 == 0) e.component<SpriteRect>().rect = { 16, 0, 16, 16 };
		if (i % 11 == 0) e.component<SpriteRect>().rect = { 32, 0, 32, 32 };
		if (i % 7 == 0)	e.component<SpriteRect>().rect = { 0, 16, 16, 16 };
		if (i % 5 == 0) e.component<SpriteRect>().rect = { 16, 16, 16, 16 };
		if (i % 3 == 0)	e.component<SpriteRect>().rect = { 0, 32, 32, 32 };
		if (i % 2 == 0) e.component<SpriteRect>().rect = { 32, 32, 32, 32 };
	}


	auto renderSystem = etcs::insertSystem<const etcs::Transform, const SpriteRect>();
	lsd::Function<void(const etcs::Transform&, const SpriteRect&)> renderFunction = 
		[&drawData, &deltaTime, fovRatio, renderer, textureAtlas](const etcs::Transform& transform, const SpriteRect& rect) {
		float fovRatY = transform.translation.y * fovRatio;

		float px = transform.translation.x / fovRatY * windowWidth + windowWidth / 2; // perspective calculations
		float py = -transform.translation.z / fovRatY * windowHeight + windowHeight / 2;
		float w = windowWidth / fovRatY;

		drawData[static_cast<int>(transform.translation.y * 10)].emplaceBack(DrawData { SDL_FRect { rect.rect }, SDL_FRect{
			(px - w / 2) * windowScale,
			(py - w / 2) * windowScale,
			w * windowScale, 
			w * windowScale
		} });
	};


	frameStart = SDL_GetTicks();

	while (running) {
		frameTime = SDL_GetTicks();

		if (frameStart < frameTime) {
			deltaTime = frameTime - frameStart;

			fps = 1000.0f / deltaTime;
			frameStart = frameTime;

			while (SDL_PollEvent(&e) == SDL_TRUE) {
				if (e.type == SDL_EVENT_QUIT) {
					running = false;

					break;
				} else if (e.type == SDL_EVENT_KEY_DOWN) {
					std::printf("FPS: %.5f\n", fps);
				}
			}

			SDL_RenderClear(renderer);

			renderSystem.each(renderFunction);

			for (auto& [_, data] : drawData) {
				for (const auto& call : data) {
					SDL_RenderTextureRotated(renderer, textureAtlas, &call.src, &call.dst, 0, nullptr, SDL_FLIP_NONE);
					data.clear();
				}
			}

			SDL_RenderPresent(renderer);
		} else SDL_Delay(1);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	SDL_Quit();

	etcs::quit();
}
