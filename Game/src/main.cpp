#define SDL_MAIN_HANDLED

#include <Common/Common.h>
#include <Init.h>
#include <InputSystem.h>

#include <Graphics/Texture.h>

#include <ETCS/ETCS.h>

#include <glm/glm.hpp>
#include <stb_image.h>

#include <cstdio>
#include <random>
#include <map>

static constexpr std::size_t fov = 100;
static constexpr double deltaTime = 0.01;
static const float fovRatio = glm::tan(glm::radians(static_cast<float>(fov / 2)));

namespace {

struct SpriteRect {
	SDL_FRect rect;
};

struct DrawData {
	SDL_FRect src;
	SDL_FRect dst;
};

}

int main(int argc, char** argv) {
	bool running = true;

	double fps = 0;
	double accumulator = 0;
	std::uint64_t startTime = 0;
	std::uint64_t currentTime = 0;

	std::random_device randDevice;
	std::default_random_engine randEngine(randDevice());


	esengine::init({
		argv,
		"車万朱元璋"
	});


	esengine::Texture textureAtlas("Sheet.png");


	auto camera = etcs::insertEntity();
	camera.insertComponent<etcs::Transform>();

	auto camTransform = camera.component<etcs::Transform>();


	std::uniform_int_distribution<int> randXGen(-20, 20);
	std::uniform_int_distribution<std::size_t> randYGen(0, 64);
	std::uniform_int_distribution<int> randZGen(-20, 20);

	for (size_t i = 2049; i > 1; i--) {
		auto e = etcs::insertEntity();

		e.insertComponent<etcs::Transform>(glm::vec3(randXGen(randEngine), randYGen(randEngine), randZGen(randEngine)));
		//e.insertComponent<etcs::Transform>(glm::vec3(1, 10, 1));
		auto rect = e.insertComponent<SpriteRect>();

		if (i % 17 == 0) rect.get() = { 0, 0, 16, 16 };
		if (i % 13 == 0) rect.get() = { 16, 0, 16, 16 };
		if (i % 11 == 0) rect.get() = { 32, 0, 32, 32 };
		if (i % 7 == 0)	rect.get() = { 0, 16, 16, 16 };
		if (i % 5 == 0) rect.get() = { 16, 16, 16, 16 };
		if (i % 3 == 0)	rect.get() = { 0, 32, 32, 32 };
		if (i % 2 == 0) rect.get() = { 32, 32, 32, 32 };
	}


	auto renderSystem = etcs::insertSystem<const etcs::Transform, const SpriteRect>();

	lsd::Function<void(const etcs::Transform&, const SpriteRect&)> renderFunction = 
		[&camTransform, textureAtlas](const etcs::Transform& transform, const SpriteRect& rect) {
			if (transform.translation().y > 0) {
				auto translation = camTransform.get().localTransform() * glm::vec4(transform.translation(), 1.0f);
				
				float fovRatY = translation.y * fovRatio;

				float px = translation.x / fovRatY * windowDim.x + windowDim.x / 2; // perspective calculations
				float py = -translation.z / fovRatY * windowDim.y + windowDim.y / 2;
				float w = windowDim.x / fovRatY;

				drawData[static_cast<int>(translation.y * 10)].emplaceBack(DrawData { SDL_FRect { rect.rect }, SDL_FRect{
					(px - w / 2),
					(py - w / 2),
					w,
					w,
				} });
			}
	};


	


	startTime = SDL_GetTicks();

	while (!esengine::globals::inputSystem->quit()) {
		currentTime = SDL_GetTicks();
		accumulator += (currentTime - startTime) / 1000.0f;
		startTime = currentTime;

		while (accumulator >= deltaTime) {
			if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
				camTransform.get().translation().z -= 0.1;
			}
			if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
				camTransform.get().translation().z += 0.1;
			}
			if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
				camTransform.get().translation().x += 0.1;
			}
			if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
				camTransform.get().translation().x -= 0.1;
			}

			accumulator -= deltaTime;

			renderSystem.each(renderFunction);
		}


		SDL_RenderClear(renderer);

		for (auto& [_, data] : drawData) {
			for (const auto& call : data) {
				SDL_RenderTextureRotated(renderer, textureAtlas, &call.src, &call.dst, 0, nullptr, SDL_FLIP_NONE);
				data.clear();
			}
		}

		SDL_RenderPresent(renderer);
	}

	esengine::quit();
}
