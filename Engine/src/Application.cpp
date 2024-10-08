#include <Application.h>

#include <Common/Common.h>
#include <Common/FileSystem.h>

#include <InputSystem.h>
#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>

#include <Components/Camera.h>
#include <Components/Floor.h>
#include <Components/Sprite3D.h>

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
	globals::window = new Window(info.name, info.dim, SDL_WINDOW_RESIZABLE | info.flags);
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

		for (auto [camTransform, camera] : etcs::world().query<etcs::Transform, Camera>()) {
			camera.update();

			/*
			for (auto [transform, floor] : etcs::world().query<etcs::Transform, Floor>()) {
				floor.update(transform.translation(), camTransform, camera);

				esengine::globals::renderSystem->insertCall(
					transform.translation().z, // if you for some reason have multiple floors, they will be sorted by z instead of y
					floor.drawCall(camera)
				);
			}
			*/

			for (const auto& [transform, sprite] : etcs::world().query<const etcs::Transform, const Sprite3D>())
				if (RenderSystem::CallData data { }; sprite.drawCall(data, transform.translation(), camTransform.localTransform(), camera))
					esengine::globals::renderSystem->insertCall(data);
		}

		globals::renderSystem->drawAll();
	}
}

} // namespace esengine
