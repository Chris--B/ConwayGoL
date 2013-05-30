#include "game.h"

int main() {

	Conway::Game game;

	game.setResolution(800, 600);
	game.setTitle("Not a title");
	// game.loadSettings("settings.txt");

	game.addCell(4, 10);
	game.addCell(5, 10);
	game.addCell(6, 10);

	game.start();

	return 0;
}
