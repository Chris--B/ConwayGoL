#pragma once

#include "cell.h"

#include <array>
#include <unordered_set>

namespace Conway {
class Board {
public:
	Board() : width(0), height(0) {}
	Board(size_t width, size_t height)
		: width(width), height(height) {}

	bool isAlive(const Cell& cell) const { return cells.find(cell) != cells.end(); }

	int getHeight() const { return height; }
	int getWidth() const { return width; }

	void setHeight(int height) { this->height = height; }
	void setWidth(int width) { this->width = width; }
	void addCell(const Cell& cell) { this->cells.insert(cell); }
	
	/* Call func() on each cell in cells, in some order
		Equivilent to
			for(const Cell& cell : board.cells) {
				func(cell);
			}
		but preserves encapsulation, since the caller can't access board.cells otherwise.
	*/
	void forEachCell(const std::function<void(const Cell&)>& func) const;

	int advanceBoard(unsigned generations = 1);

private:
	int countLivingNeighbors(const Cell& cell) const;

	std::array<Cell, 8> getNeighbors(const Cell& cell) const;

	int width;
	int height;

	std::unordered_set<Cell> cells;
};

} // namespace Conway
