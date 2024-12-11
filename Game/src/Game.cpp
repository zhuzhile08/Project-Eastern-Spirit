#include <Game.h>

#include <Controllers/Player.h>

#include <Detail/InputSystem.h>

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
		player.insertComponent<esengine::KinematicBody>(glm::vec2(6.0f, 9.0f), glm::vec2(-3, 15), 0, 0);
		player.insertComponent<PlayerController>();

		auto sprite = player.insertComponent<esengine::Sprite>(SDL_FRect { 0, 0, 32, 32 }, &m_spriteSheet);

		esengine::AnimatorBuilder animBuilder;
		animBuilder.addAnimation({ "IdleDown", { { 0 } }, true });
		animBuilder.addAnimation({ "IdleLeft", { { 1 } }, true });
		animBuilder.addAnimation({ "IdleUp", { { 2 } }, true });
		animBuilder.addAnimation({ "IdleRight", { { 3 } }, true });

		animBuilder.addAnimation({ "WalkDown", { { 32 }, { 33 }, { 34 }, { 35 } }, true });
		animBuilder.addAnimation({ "WalkLeft", { { 36 }, { 37 }, { 38 }, { 39 } }, true });
		animBuilder.addAnimation({ "WalkUp", { { 40 }, { 41 }, { 42 }, { 43 } }, true });
		animBuilder.addAnimation({ "WalkRight", { { 44 }, { 45 }, { 46 }, { 47 } }, true });

		player.insertComponent<esengine::SpriteAnimator>(animBuilder.buildSpriteAnimator(sprite));
	}

	{
		auto wall = etcs::world().insertEntity();
		wall.insertComponent<etcs::Transform>(glm::vec3(64, 64, 0));
		wall.insertComponent<esengine::StaticBody>(glm::vec2(32.0f, 32.0f), glm::vec2(0, 0), 0, 0);
		wall.insertComponent<esengine::Sprite>(SDL_FRect { 0, 0, 32, 32 }, &m_spriteSheet);
	}
}

void Game::update(float deltaTime) {
	for (auto [playerController, kinematicBody, animator] : etcs::world().query<PlayerController, esengine::KinematicBody, esengine::SpriteAnimator>()) {
		playerController.update(deltaTime, kinematicBody, animator);
	}
}
