#pragma once

#include "board.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
	class Game {
	public:
		Game() : paused(false), speed(15) {}

		bool isRunning() const { return window.isOpen(); }

		int getBoardHeight() const { return board.getHeight(); }
		int getBoardWidth() const { return board.getWidth(); }

		void handleEvents();
		void loadSettings(const std::string& filename);
		void render();
		void start();
		void stop() { window.close(); }

		void setBoardSize(int height, int width);
		void setResolution(unsigned height, unsigned width);
		void setWindowTitle(const std::string& title) { window_title = title; }

		void addCell(int x, int y) { board.addCell(Cell(x, y)); }

	private:
		Board board;

		bool paused;
		int speed;

		sf::Color color_background;
		sf::Color color_cell;

		sf::RenderWindow window;

		std::string window_title;

		sf::Vector2f cell_size;
		sf::VideoMode video_mode;
	};
} // namespace Conway
