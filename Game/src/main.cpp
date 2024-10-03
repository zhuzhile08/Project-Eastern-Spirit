#define SDL_MAIN_HANDLED

#include <Common/Common.h>
#include <InputSystem.h>
#include <Application.h>

#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/Texture.h>

#include <Components/Sprite3D.h>
#include <Components/Camera.h>

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
	Application(esengine::InitInfo info) : esengine::Application(info), m_textureAtlas("Sheet.png"), m_camera(etcs::world().insertEntity()), m_camTransform(m_camera.insertComponent<etcs::Transform>()) {
		m_camera.insertComponent<esengine::Camera>();

		std::random_device randDevice;
		std::default_random_engine randEngine(randDevice());

		std::uniform_real_distribution<float> randXGen(-20, 20);
		std::uniform_real_distribution<float> randYGen(0, 64);
		std::uniform_real_distribution<float> randZGen(-20, 20);

		for (size_t i = 2049; i > 1; i--) {
			auto e = etcs::world().insertEntity();

			e.insertComponent<etcs::Transform>(glm::vec3(randXGen(randEngine), randYGen(randEngine), randZGen(randEngine)));

			if (i % 17 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 0, 0, 16, 16 }, &m_textureAtlas);
			else if (i % 13 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 16, 0, 16, 16 }, &m_textureAtlas);
			else if (i % 11 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 32, 0, 32, 32 }, &m_textureAtlas);
			else if (i % 7 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 0, 16, 16, 16 }, &m_textureAtlas);
			else if (i % 5 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 16, 16, 16, 16 }, &m_textureAtlas);
			else if (i % 3 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 0, 32, 32, 32 }, &m_textureAtlas);
			else if (i % 2 == 0) e.insertComponent<esengine::Sprite3D>(SDL_FRect { 32, 32, 32, 32 }, &m_textureAtlas);
		}
	}

private:
	void update() {
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
			m_camTransform.get().translation().z += 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::s).held) {
			m_camTransform.get().translation().z -= 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::a).held) {
			m_camTransform.get().translation().x += 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::d).held) {
			m_camTransform.get().translation().x -= 0.1;
		}
	}

	esengine::Texture m_textureAtlas;

	etcs::Entity m_camera;
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
