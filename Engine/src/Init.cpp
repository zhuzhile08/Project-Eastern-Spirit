#include <Init.h>

#include <Common/FileSystem.h>

#include <InputSystem.h>
#include <Graphics/Window.h>
#include <Graphics/RenderSystem.h>

#include <ETCS/ETCS.h>

namespace esengine {

namespace globals {

extern FileSystem* fileSystem;
extern Window* window;
extern RenderSystem* renderSystem;
extern InputSystem* inputSystem;

}

void init(InitInfo info) {
	etcs::init();

	if (SDL_Init(
		SDL_INIT_VIDEO | 
		SDL_INIT_EVENTS | 
		SDL_INIT_AUDIO | 
		SDL_INIT_JOYSTICK | 
		SDL_INIT_GAMEPAD) == SDL_FALSE
	) throw std::runtime_error(SDL_GetError());

	globals::fileSystem = new FileSystem(info.argv);
	globals::window = new Window(info.name, info.dim, SDL_WINDOW_RESIZABLE | info.flags);
	globals::renderSystem = new RenderSystem();
	globals::inputSystem = new InputSystem();
}

void quit() {
	delete globals::inputSystem;
	delete globals::renderSystem;
	delete globals::window;
	delete globals::fileSystem;

	SDL_Quit();

	etcs::quit();
}

} // namespace esengine
