#include <Game.h>

#include <Controllers/Player.h>

#include <Detail/InputSystem.h>

#include <Components/Camera.h>
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <Components/Colliders.h>
#include <Components/TextBox.h>
#include <Components/TextBoxAnimator.h>

#include <Graphics/AnimatorBuilder.h>
#include <Graphics/Window.h>

#include <glm/glm.hpp>

Game::Game(esengine::InitInfo info) : 
	esengine::Application(info), 
	m_spriteSheet("img/sprites/PlayerSheet.png"), 
	m_wall("img/sprites/Wall.png"), 
	m_font("img/fonts/Regular.esfj") 
{	
	m_spriteSheet.disableFiltering();
	m_font.texture().disableFiltering();
	m_wall.disableFiltering();

	{ // Camera
		auto camera = etcs::world().insertEntity();
		camera.insertComponent<etcs::Transform>(glm::vec3(0, 0, 0));
		camera.insertComponent<esengine::Camera>();
	}

	{ // Textbox
		auto text = etcs::world().insertEntity();
		text.insertComponent<etcs::Transform>(glm::vec3(0.0, 0.0, 0.0));

		auto box = text.insertComponent<esengine::TextBox>("Hello World!", &m_font, glm::vec2(100, 100), glm::vec2(0, 0), glm::vec2(-1, 0), esengine::TextBox::Alignment::center);
		text.insertComponent<esengine::TextBoxAnimator>(box);
	}

	{ // Player
		auto player = etcs::world().insertEntity();
		player.insertComponent<etcs::Transform>(glm::vec3(0, 0, 0));
		player.insertComponent<esengine::KinematicBody>(glm::vec2(6, 9), glm::vec2(13, 21));
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
		wall.insertComponent<etcs::Transform>(glm::vec3(48, 48, 1));
		wall.insertComponent<esengine::StaticBody>(glm::vec2(32, 32), glm::vec2(0, 0));
		wall.insertComponent<esengine::Sprite>(SDL_FRect { 0, 0, 32, 32 }, &m_wall);
	}
}

void Game::update(float deltaTime) {
	for (auto [playerController, kinematicBody, animator] : etcs::world().query<PlayerController, esengine::KinematicBody, esengine::SpriteAnimator>()) {
		playerController.update(deltaTime, kinematicBody, animator);
	}
}
