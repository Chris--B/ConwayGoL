#pragma once

#include "Cell.hpp"

#include <array>
#include <functional>
#include <unordered_set>

namespace Conway {
class Board {
public:
	Board() {}
	// TODO: Constructor which takes in some initial cells.

	void advanceBoard();
	int countLivingNeighbors(const Cell& cell) const;
	std::array<Cell, 8> getNeighbors(const Cell& cell) const;

	void randomize(int lowerx, int lowery, int upperx, int uppery);

	void clear() {
		cells = std::unordered_set<Cell>();
	}

	// TODO: The render-er needs access to the living cells in order to
	//           get the vertices to render.
	//    There's a couple ways to do it:
	//    1. Copy all the cells and return that.
	//    2. Return a const& to the internal structure.
	//    3. Take in a function to operate on the cells, adding
	//        verticies to a list the function has access to.
	//    For now, we use #3.
	void forEachLivingCell(const std::function<void(const Cell&)>& func)
	 const {
		for (const Cell& cell : cells) {
			func(cell);
		}
	}

	bool isAlive(const Cell& cell) const {
		return cells.find(cell) != cells.end();
	}

	int population() const {
		return cells.size();
	}

	void addCell(int x, int y) {
		this->cells.insert(Cell(x, y));
	}

	void addCell(const Cell& cell) {
		this->cells.insert(cell);
	}

	void killCell(int x, int y) {
		this->cells.erase(Cell(x, y));
	}

	void killCell(const Cell& cell) {
		this->cells.erase(cell);
	}

private:
	std::unordered_set<Cell> cells;
};

} // namespace Conway
