#include <Application.h>

#include <Core/Common.h>
#include <Core/FileSystem.h>

#include <Detail/InputSystem.h>
#include <Graphics/Window.h>
#include <Detail/RenderSystem.h>

#include <Detail/PhysicsSystem.h>

#include <Components/Camera.h>
#include <Components/KinematicBody.h>
#include <Components/ParticleSystem.h>
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>

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
	globals::window = new Window(info.name, info.dim, constants::defaultWindowScalingFactor, info.flags);
#endif

	globals::renderSystem = new detail::RenderSystem();
	globals::inputSystem = new detail::InputSystem();
	globals::physicsSystem = new detail::PhysicsSystem();
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

			update(m_deltaTime / 16.f);

			globals::physicsSystem->update();

			for (auto [animator] : etcs::world().query<SpriteAnimator>()) {
				animator.update(m_deltaTime);
			}

			m_accumulator -= m_deltaTime;
		}

		for (auto [camera, camTransform, camComponent] : etcs::world().query<const etcs::Entity, etcs::Transform, Camera>()) {
			camComponent.update(camTransform);
			auto camTransformMat = camComponent.transformMat(camera, camTransform);

			for (const auto& [sprite, transform, spriteComponent] : etcs::world().query<const etcs::Entity, const etcs::Transform, const Sprite>())
				if (detail::RenderSystem::CallData data { }; spriteComponent.drawCall(data, sprite, transform, camTransformMat, camComponent)) // @todo renderpasses are not respected, needs an extra check
					esengine::globals::renderSystem->insertCall(data, camComponent.passName());
		}

		globals::renderSystem->drawAll();
	}
}

} // namespace esengine
