#include "cell.h"

using namespace Conway;

Cell& Cell::operator=(const Cell& cell) {
	x = cell.x;
	y = cell.y;
	return *this;
}

std::array<Cell, 8> Cell::getNeighbors() const {
	std::array<Cell, 8> neighbors = {
		Cell(x - 1, y), 
		Cell(x - 1, y - 1), 
		Cell(x - 1, y + 1), 
		Cell(x, y - 1), 
		Cell(x, y + 1), 
		Cell(x + 1, y), 
		Cell(x + 1, y - 1), 
		Cell(x + 1, y + 1), 
	};
	return neighbors;
}

bool Cell::operator==(const Cell& cell) const {
	return x == cell.x && y == cell.y;
}
