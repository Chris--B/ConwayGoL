#include "board.h"

#include <cstdio>
#include <cstdarg>

using namespace Conway;

void Board::advanceBoard() {
	int living_count = 0;
	std::unordered_set<Cell> next_gen;

	for(auto cell : cells) {
		for(auto neighbor : getNeighbors(cell)) {
			auto living_neighbors = countLivingNeighbors(neighbor);
			if (living_neighbors == 3 || (living_neighbors == 2 && isAlive(neighbor))) {
				next_gen.insert(neighbor);
				++living_count;
			}
		}
	}
	cells = next_gen;
}

int Board::countLivingNeighbors(const Cell& cell) const {
	auto living = 0;
	auto neighbors = getNeighbors(cell);
	for(auto neighbor : neighbors) {
		if (isAlive(neighbor)) {
			++living;
		}
	}
	return living;
}

std::array<Cell, 8> Board::getNeighbors(const Cell& cell) const {
	auto x = cell.x;
	auto y = cell.y;

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
