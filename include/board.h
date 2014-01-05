#pragma once

#include "cell.h"

#include <array>
#include <unordered_set>

namespace Conway {
class Board {
public:
	Board() {}

	bool isAlive(const Cell& cell) const { return cells.find(cell) != cells.end(); }

	void addCell(const Cell& cell) { this->cells.insert(cell); }
	void killCell(const Cell& cell) { this->cells.erase(cell); }

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


	std::unordered_set<Cell> cells;
};

} // namespace Conway
