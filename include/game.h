#pragma once

#include "board.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
	class Game {
	public:
		Game() : paused(false), speed(15) {}

		bool isRunning() const { return window.isOpen(); }

		void handleEvents();
		void loadSettings() { loadSettings(settings_filename); }
		void loadSettings(const std::string& filename);
		void render();
		void start();
		void stop() { window.close(); }

		void setResolution(unsigned height, unsigned width);
		void setSettingsFile(const std::string& filename) { settings_filename = filename; }
		void setWindowTitle(const std::string& title);

		void addCell(int x, int y) { board.addCell(Cell(x, y)); }

	private:
		Board board;

		bool paused;
		int speed;

		sf::Color color_background;
		sf::Color color_cell;

		sf::RenderWindow window;

		std::string settings_filename;
		std::string window_title;      //SFML doesn't provide a getter, so we have to keep track of it manually

		sf::Vector2f cell_size;
	};
} // namespace Conway
