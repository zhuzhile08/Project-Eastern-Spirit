#define SDL_MAIN_HANDLED

#include <Game.h>

int main(int argc, char** argv) {
	Game game({
		argv,
		"Project Eastern Spirit",
	});

	game.run();
}
