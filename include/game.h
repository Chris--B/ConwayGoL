#pragma once

#include "board.h"
#include "Settings.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
class Game {
public:
	static void errorMessage(const std::string& message);

	Game() {}

	void render();
	void handleEvents();

	void setup();
	void start();
	void stop();

	void setResolution(unsigned int height, unsigned int width);
	void setWindowTitle(const std::string& title);

	void setSettingsFile(const std::string& filename) {
		settings_filename = filename;
		settings.fromFile(filename);
	}

	// TODO: Remove this.
	void addCell(int x, int y) {
		board.addCell(Cell(x, y));
	}

private:
	Board board;

	bool paused = false;
	bool running = true;

	std::string settings_filename = "Conway.ini";
	Settings settings;

	// Graphics exclusive stuff here.
	// TODO: Hide this away, out of the header?
	sf::RenderWindow window;
};
} // namespace Conway
