#pragma once

#include "board.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
	class Game {
	public:
		Game() : paused(false), generations_per_sec(15), backgroundColor(sf::Color::White), cellColor(sf::Color::Black) {}

		bool isRunning() const { return window.isOpen(); }

		int getBoardHeight() const { return board.getHeight(); }
		int getBoardWidth() const { return board.getWidth(); }

		void start();
		void stop() { window.close(); }
		void handleEvents();
		void render();
		void setResolution(unsigned height, unsigned width);
		void setTitle(std::string title) { windowtitle = title; }

		void addCell(int x, int y) { board.addCell(Cell(x, y)); }
		void setBoardSize(int width, int height);

	private:
		void drawCell(const Cell& cell);

		bool paused;
		int generations_per_sec;

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
