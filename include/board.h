#pragma once

#include "cell.h"

#include <array>
#include <unordered_set>

namespace Conway {
class Board {
public:
	friend class Game;

	static const int DEFAULT_WIDTH = 25;
	static const int DEFAULT_HEIGHT = 25;

	Board()
		: width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {}
	Board(size_t width, size_t height)
		: width(width), height(height) {}

	bool isAlive(const Cell& cell) const;

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	int advanceBoard(unsigned generations = 1);
	int countLivingNeighbors(const Cell& cell) const;

	std::array<Cell, 8> getNeighbors(const Cell& cell) const;

private:
	int width;
	int height;

	std::unordered_set<Cell> cells;
};

} // namespace Conway

