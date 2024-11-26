#include <Game.h>

#include <Controllers/Player.h>

#include <InputSystem.h>

#include <Components/SpriteAnimator.h>

#include <Graphics/AnimatorBuilder.h>
#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>

#include <glm/glm.hpp>

using namespace std::chrono_literals;

Game::Game(esengine::InitInfo info) : esengine::Application(info), m_spriteSheet("IMG/PlayerSheet.png") {
	{ // camera
		auto camera = etcs::world().insertEntity();
		camera.insertComponent<etcs::Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
		camera.insertComponent<esengine::Camera>();
	}

	{ // player
		auto player = etcs::world().insertEntity();
		player.insertComponent<etcs::Transform>(glm::vec3(16, 16, 0));
		player.insertComponent<esengine::KinematicBody>();
		player.insertComponent<PlayerController>();

		auto sprite = player.insertComponent<esengine::Sprite>(SDL_FRect { 0, 0, 32, 32 }, &m_spriteSheet);

		esengine::AnimatorBuilder animBuilder;
		animBuilder.addAutoPlay({"default", {{0, 150ms}, {1, 150ms}, {2, 150ms}, {3, 150ms}}, true});

		player.insertComponent<esengine::SpriteAnimator>(animBuilder.buildSpriteAnimator(sprite));
	}
}

void Game::update() {
	for (auto [playerController, kinematicBody, animator] : etcs::world().query<const PlayerController, esengine::KinematicBody, esengine::SpriteAnimator>()) {
		playerController.update(kinematicBody, animator);
	}
}
