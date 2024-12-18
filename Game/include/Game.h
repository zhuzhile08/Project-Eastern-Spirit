/*************************
 * @file Game.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Game class. Handles main loop
 * 
 * @date 2024-11-25
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <Application.h>

#include <Graphics/Texture.h>
#include <Graphics/Font.h>

#include <ETCS/ETCS.h>

class Game : public esengine::Application {
public:
	Game(esengine::InitInfo info);

private:
	void update(float deltaTime);

	esengine::Texture m_spriteSheet;
	esengine::Texture m_wall;

	esengine::Font m_font;
};
