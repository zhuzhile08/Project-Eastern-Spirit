#define SDL_MAIN_HANDLED

#include <Game.h>

int main(int argc, char** argv) {
	Game game({
		argv,
		"Project Eastern Spirit",
		{ 16 * 28, 16 * 16 }
	});

	game.run();
}
