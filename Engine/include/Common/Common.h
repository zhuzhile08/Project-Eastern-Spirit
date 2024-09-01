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

namespace esengine {

class FileSystem;
class Window;
class RenderSystem;
class InputSystem;

class Texture;

namespace globals {

extern FileSystem* fileSystem;
extern Window* window;
extern RenderSystem* renderSystem;
extern InputSystem* inputSystem;

} // namespace globals

} // namespace esengine
