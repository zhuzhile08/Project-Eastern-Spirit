#define SDL_MAIN_HANDLED

#include <Common/Common.h>
#include <InputSystem.h>
#include <Application.h>

#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/Texture.h>

#include <Components/Camera.h>
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>

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

using namespace std::chrono_literals;

class Application : public esengine::Application {
public:
	Application(esengine::InitInfo info) : 
		esengine::Application(info), 
		m_spriteSheet("IMG/PlayerSheet.png"), 
		m_camera(etcs::world().insertEntity()), 
		m_camTransform(m_camera.insertComponent<etcs::Transform>(glm::vec3(0.0f, 0.0f, 0.0f))) {
		m_camera.insertComponent<esengine::Camera>();

		auto e = etcs::world().insertEntity();

		e.insertComponent<etcs::Transform>(glm::vec3(16, 16, 0));
		auto s = e.insertComponent<esengine::Sprite>(SDL_FRect { 0, 0, 32, 32 }, &m_spriteSheet);
		e.insertComponent<esengine::SpriteAnimator>(s, esengine::SpriteAnimator::Animation{"default", {{0, 150ms}, {1, 150ms}, {2, 150ms}, {3, 150ms}}, true});
	}

private:
	void update() {
		auto& transfrom = m_camTransform.get();

		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::w).held) {
			transfrom.translation().z += 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::s).held) {
			transfrom.translation().z -= 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::a).held) {
			transfrom.translation().x -= 0.1;
		}
		if (esengine::globals::inputSystem->keyboard(esengine::KeyType::d).held) {
			transfrom.translation().x += 0.1;
		}
	}

	esengine::Texture m_spriteSheet;

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
