#include "game.h"

int main() {
	Conway::Game game;
	game.loadSettings("conway.ini");

	// Gliders
	for(int i = 0; i < game.getBoardHeight(); i += 5) {
		game.addCell(2, i + 2);
		game.addCell(2, i + 1);
		game.addCell(2, i);
		game.addCell(1, i + 2);
		game.addCell(0, i + 1);
	}
	game.start();

	return 0;
}
