#pragma once

#include "board.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
	class Game {
	public:
		Game() : paused(false) {}

		bool isRunning() const;

		void start();
		void stop();
		void handleEvents();
		void render();
		void setResolution(unsigned height, unsigned width);
		void setTitle(std::string title);

		void addCell(int x, int y);

	private:
		void drawCell(const Cell& cell);

		bool paused;

		Board board;

		std::string windowtitle;

		sf::Color fillColor;
		sf::RenderWindow window;
		sf::Vector2f cellsize;
		sf::VideoMode videomode;

	};
} // namespace Conway
