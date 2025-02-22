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
#include <glm/glm.hpp>

namespace esengine {

// Convenience aliases

using ms_time_t = std::uint64_t;
using es_color_t = glm::vec3;


// Structure forward declarations

struct Animation;

class FileSystem;

class Camera;
class Health;
class KinematicBody;
class ParticleSystem;
class Path;
class Sprite;
class SpriteAnimator;
class StaticBody;
class TextBox;
class TextBoxAnimator;
class TileMap;
class Valuable;

namespace detail {

class RenderSystem;
struct Frame;
struct AnimationPlayData;

class InputSystem;
class PhysicsSystem;

} // namespace detail

class AnimatorBuilder;
class Font;
class Texture;
class TileSet;
class Window;

namespace globals {

extern FileSystem* fileSystem;
extern Window* window;
extern detail::RenderSystem* renderSystem;
extern detail::InputSystem* inputSystem;
extern detail::PhysicsSystem* physicsSystem;

} // namespace globals


namespace constants {

inline constexpr std::uint32_t graphicsCellUnit = 16;
inline constexpr std::size_t depthSortingFactor = 10;

#ifndef ESENGINE_DYNAMIC_WINDOW_SIZE
inline constexpr std::size_t defaultWindowScalingFactor = 3;
#endif

inline constexpr ms_time_t defaultAnimationFrameTime = 125;
inline constexpr ms_time_t defaultTextSpeed = 100;

inline constexpr SDL_Rect nullRect = { std::numeric_limits<int>::min(), 0, 0, 0 };

} // namespace constants

} // namespace esengine


#define ESENGINE_DEFAULT_CONSTRUCTORS(className)\
className() = default;\
className(const className&) = default;\
className(className&&) = default;\
className& operator=(const className&) = default;\
className& operator=(className&&) = default;
