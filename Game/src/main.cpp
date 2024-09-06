#define SDL_MAIN_HANDLED

#include <Common/Common.h>
#include <InputSystem.h>
#include <Application.h>

#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>
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

class Application : public esengine::Application {
public:
	Application(esengine::InitInfo info) : esengine::Application(info), m_textureAtlas("Sheet.png") {
		auto camera = etcs::insertEntity();
		camera.insertComponent<etcs::Transform>();

		m_camTransform = camera.component<etcs::Transform>();


		std::random_device randDevice;
		std::default_random_engine randEngine(randDevice());

		std::uniform_int_distribution<int> randXGen(-20, 20);
		std::uniform_int_distribution<std::size_t> randYGen(0, 64);
		std::uniform_int_distribution<int> randZGen(-20, 20);

		for (size_t i = 2049; i > 1; i--) {
			auto e = etcs::insertEntity();

			e.insertComponent<etcs::Transform>(glm::vec3(randXGen(randEngine), randYGen(randEngine), randZGen(randEngine)));
			//e.insertComponent<etcs::Transform>(glm::vec3(1, 10, 1));
			auto rect = e.insertComponent<esengine::SpriteRect>();

			if (i % 17 == 0) rect.get() = { 0, 0, 16, 16 };
			if (i % 13 == 0) rect.get() = { 16, 0, 16, 16 };
			if (i % 11 == 0) rect.get() = { 32, 0, 32, 32 };
			if (i % 7 == 0)	rect.get() = { 0, 16, 16, 16 };
			if (i % 5 == 0) rect.get() = { 16, 16, 16, 16 };
			if (i % 3 == 0)	rect.get() = { 0, 32, 32, 32 };
			if (i % 2 == 0) rect.get() = { 32, 32, 32, 32 };
		}


		m_projector = etcs::insertSystem<const etcs::Transform, const esengine::SpriteRect>();

		m_projectionFunction = [this](const etcs::Transform& transform, const esengine::SpriteRect& rect) {
			if (transform.translation().y > 0) {
				auto translation = m_camTransform.get().localTransform() * glm::vec4(transform.translation(), 1.0f);
				
				float fovRatY = translation.y * fovRatio;

				float px = translation.x / fovRatY * esengine::globals::window->size().x + esengine::globals::window->size().x / 2; // perspective calculations
				float py = -translation.z / fovRatY * esengine::globals::window->size().y + esengine::globals::window->size().y / 2;
				float w = esengine::globals::window->size().x / fovRatY;

				esengine::globals::renderSystem->insertCall(translation.y, esengine::RenderSystem::CallData { 0.0f, rect.rect, SDL_FRect {
						(px - w / 2),
						(py - w / 2),
						w,
						w,
					}, &m_textureAtlas
				});
			}
		};
	}

private:
	void update() {
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
			m_camTransform.get().translation().z -= 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
			m_camTransform.get().translation().z += 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
			m_camTransform.get().translation().x += 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
			m_camTransform.get().translation().x -= 0.1;
		}
	}

	esengine::Texture m_textureAtlas;

	etcs::ComponentView<etcs::Transform> m_camTransform;
};

}

int main(int argc, char** argv) {
	Application app({
		argv,
		"車万朱元璋",
	});

	app.run();
}
