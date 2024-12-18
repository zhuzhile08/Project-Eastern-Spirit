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
#include <Components/TextBox.h>
#include <Components/TextBoxAnimator.h>

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

		while (m_accumulator >= m_deltaTime) { // update loop
			globals::inputSystem->update();

			update(m_deltaTime / 16.f);

			globals::physicsSystem->update();

			for (auto [animator] : etcs::world().query<SpriteAnimator>())
				animator.update(m_deltaTime);

			for (auto [animator] : etcs::world().query<TextBoxAnimator>())
				animator.update(m_deltaTime);

			m_accumulator -= m_deltaTime;
		}

		for (auto [camEntity, camTransform, camComponent] : etcs::world().query<const etcs::Entity, etcs::Transform, Camera>()) { // render loop
			camComponent.update(camTransform);
			auto camTransformMat = camComponent.transformMat(camEntity, camTransform);

			for (const auto& [spriteEntity, transform, sprite] : etcs::world().query<const etcs::Entity, const etcs::Transform, const Sprite>())
				if (detail::RenderSystem::CallData data { }; sprite.drawCall(data, spriteEntity, transform, camTransformMat, camComponent))
					esengine::globals::renderSystem->insertCall(data, camComponent.passName());

			for (const auto& [textBoxEntity, transform, textBox] : etcs::world().query<const etcs::Entity, const etcs::Transform, const TextBox>())
				textBox.draw(textBoxEntity, transform, camTransformMat, camComponent);
		}

		globals::renderSystem->drawAll();
	}
}

} // namespace esengine
