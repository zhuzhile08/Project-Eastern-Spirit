/*************************
 * @file Common.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Common header with forward declarations and other stuff
 * 
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <SDL3/SDL.h>

namespace esengine {

class FileSystem;

class Background;
class Font;
class Texutre;
class Window;

class RenderSystem;
class InputSystem;

class Animator;
class Camera;
class Dispenser;
class Health;
class Hitbox;
class Hurtbox;
class Path;
class Sprite2D;
class Sprtie3D;
class Textbox;
class Trail;
class Valuable;

class Texture;

namespace globals {

extern FileSystem* fileSystem;
extern Window* window;
extern RenderSystem* renderSystem;
extern InputSystem* inputSystem;

inline constexpr std::uint32_t spriteUnit = 16;
inline constexpr std::size_t depthSortingFactor = 500

} // namespace globals

} // namespace esengine
