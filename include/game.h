#pragma once


#include "cell.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_set>

namespace Conway {
	class Game {
	public:
		Game() : paused(false), gridsize(25) {}

		bool isRunning() const;

		void start();
		void stop();
		void handleEvents();
		void render();
		void setResolution(unsigned height, unsigned width);
		void setTitle(std::string title);

		void addCell(const Cell& cell);
		void addCell(int x, int y);

		void log(std::string format, ...) const;

	private:
		bool isCellAlive(const Cell& cell) const;

		int advanceBoard(unsigned generations = 1);
		int countLivingNeighbors(const Cell& cell) const;

		void drawCell(const Cell& cell);

		bool paused;
		float gridsize;

		std::unordered_set<Cell> cells;
		std::string windowtitle;
		
		sf::Color fillColor;
		sf::RenderWindow window;
		sf::VideoMode videomode;
		
	};
} // namespace Conway
