#pragma once

#include "cell.h"

#include <array>
#include <unordered_set>

namespace Conway {
class Board {
public:
	static const int DEFAULT_WIDTH = 25;
	static const int DEFAULT_HEIGHT = 25;

	Board()
		: width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {}
	Board(size_t width, size_t height)
		: width(width), height(height) {}

	bool isAlive(const Cell& cell) const { return cells.find(cell) != cells.end(); }

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void addCell(const Cell& cell) { this->cells.insert(cell); }
	
	/* Call func() on each cell in cells, in some order
		Equivilent to
			for(const Cell& cell : board.cells) {
				func(cell);
			}
		but preserves encapsulation, since the caller can't access board.cells otherwise.
	*/
	void forEachCell(std::function<void(const Cell&)> func) const;

	int advanceBoard(unsigned generations = 1);
	int countLivingNeighbors(const Cell& cell) const;

	std::array<Cell, 8> getNeighbors(const Cell& cell) const;
	
private:
	int width;
	int height;

	std::unordered_set<Cell> cells;
};

} // namespace Conway

