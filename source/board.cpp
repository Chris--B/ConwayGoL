#include "board.h"

#include <cstdio>
#include <cstdarg>

using namespace Conway;

void Board::forEachCell(std::function<void(const Cell&)> func) const {
	for(const Cell& cell : cells) {
		func(cell);
	}
}

int Board::advanceBoard(unsigned generations) {
	int living_count = 0;
	while (generations--) {
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
	return living_count;
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

	// Wrap-around
	for (auto& c : neighbors) {
		if (c.x < 0) {
			c.x += width;
		}
		if (c.x >= width) {
			c.x = c.x % width;
		}

		if (c.y < 0) {
			c.y += height;
		}
		if (c.y >= height) {
			c.y = c.y % height;
		}
	}
	return neighbors;
}
