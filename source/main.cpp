#include "game.h"

int main() {

	Conway::Game game;

	game.setResolution(800, 800);
	game.setTitle("Conway's Game of Life");
	// game.loadSettings("settings.txt");

	// Gliders
	for(int i = 0; i < Conway::Board::DEFAULT_HEIGHT; i += 5) {
		game.addCell(2, i + 2);
		game.addCell(2, i + 1);
		game.addCell(2, i);
		game.addCell(1, i + 2);
		game.addCell(0, i + 1);
	}
	game.start();

	return 0;
}
