#include "game.h"

int main(int argc, const char** argv) {
	Conway::Game game;
	game.setSettingsFile(argc > 1 ? argv[1] : "conway.ini");
	game.loadSettings();

	// Gliders
	for(int i = 0; i < 100; i += 5) {
		game.addCell(2, i + 2);
		game.addCell(2, i + 1);
		game.addCell(2, i);
		game.addCell(1, i + 2);
		game.addCell(0, i + 1);
	}
	game.start();

	return 0;
}
