#include <Application.h>

#include <Common/Common.h>
#include <Common/FileSystem.h>

#include <InputSystem.h>
#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>

#include <Components/Camera.h>
#include <Components/Sprite.h>

#include <ETCS/ETCS.h>

namespace esengine {

Application::Application(InitInfo info) : m_deltaTime(info.deltaTime) {
	etcs::init();

	if (SDL_Init(
		SDL_INIT_VIDEO | 
		SDL_INIT_EVENTS | 
		SDL_INIT_AUDIO | 
		SDL_INIT_JOYSTICK | 
		SDL_INIT_GAMEPAD) == false
	) throw std::runtime_error(SDL_GetError());

	globals::fileSystem = new FileSystem(info.argv);

#ifdef ESENGINE_DYNAMIC_WINDOW_SIZE
	globals::window = new Window(info.name, info.dim, info.flags);
#else
	globals::window = new Window(info.name, info.dim, globals::defaultWindowScalingFactor, info.flags);
#endif

	globals::renderSystem = new RenderSystem();
	globals::inputSystem = new InputSystem();
}

Application::~Application() {
	delete globals::inputSystem;
	delete globals::renderSystem;
	delete globals::window;
	delete globals::fileSystem;

	SDL_Quit();

	etcs::quit();
}

void Application::run() {
	m_currentTime = SDL_GetTicks();

	while (!esengine::globals::inputSystem->quit()) {
		m_startTime = m_currentTime;
		m_currentTime = SDL_GetTicks();

		m_accumulator += m_currentTime - m_startTime;

		globals::inputSystem->startFrame();

		while (m_accumulator >= m_deltaTime) {
			globals::inputSystem->update();

			update();

			m_accumulator -= m_deltaTime;
		}

		for (auto [camera, camTransform, camComponent] : etcs::world().query<const etcs::Entity, etcs::Transform, Camera>()) {
			camComponent.update(camTransform);
			auto camTransformMat = camComponent.transformMat(camera, camTransform);

			for (const auto& [sprite, transform, spriteComponent] : etcs::world().query<const etcs::Entity, const etcs::Transform, const Sprite>())
				if (RenderSystem::CallData data { }; spriteComponent.drawCall(data, sprite, transform, camTransformMat, camComponent))
					esengine::globals::renderSystem->insertCall(data, camComponent.passName());
		}

		globals::renderSystem->drawAll();
	}
}

} // namespace esengine
