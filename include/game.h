#pragma once

#include "board.h"
#include "GameSettings.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
	class Game {
	public:
		Game() {}

		bool isRunning() const { return window.isOpen(); }

		int getBoardHeight() const { return board.getHeight(); }
		int getBoardWidth() const { return board.getWidth(); }

		void loadSettings(const std::string& filename);
		void start();
		void stop() { window.close(); }
		void handleEvents();
		void render();
		void setResolution(unsigned height, unsigned width);
		void setTitle(const std::string& title) { windowtitle = title; }

		void addCell(int x, int y) { board.addCell(Cell(x, y)); }
		void setBoardSize(int width, int height);

	private:
		void drawCell(const Cell& cell);

		bool paused;
		int speed;

		Board board;

		std::string windowtitle;

		sf::Color backgroundColor;
		sf::Color cellColor;

		//sf::RectangleShape background;
		
		sf::RenderWindow window;
		
		sf::Vector2f cellsize;
		sf::VideoMode videomode;

	};
} // namespace Conway
