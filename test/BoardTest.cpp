#include "gtest/gtest.h"
#include "Board.hpp"
#include "Cell.hpp"

using namespace Conway;

TEST(BoardTest, BasicGlider) {
	Board board;

	// Hard code a glider in.
	// This specific pattern moves down and left.
	std::array<Cell, 5> glider = {
		Cell(2, 2),
		Cell(2, 1),
		Cell(2, 0),
		Cell(1, 2),
		Cell(0, 1),
	};
	for (const Cell& cell : glider) {
		board.addCell(cell);
	}

	// Gliders repeat themselves every 4 generations, while moving.
	// We'll run through 10 such cycles.
	for (int i = 0; i < 4 * 10; i += 1) {
		board.advanceBoard();
		// The glider is made up of 5 different cells.
		EXPECT_EQ(board.population(), 5);
	}
}

TEST(BoardTest, RulesForLiveCells) {
	Cell origin = Cell(0, 0);
	std::array<Cell, 8> neighbors = Board().getNeighbors(origin);

	for (int neighbors_count = 0; neighbors_count < 9; neighbors_count += 1) {
		Board board;
		board.addCell(origin);
		for (int i = 0; i < neighbors_count; i += 1) {
			board.addCell(neighbors[i]);
		}
		board.advanceBoard();

		switch (neighbors_count) {
		// "... as if caused by under-population."
		case 0: case 1:
		// "... as if by overcrowding."
		case 4: case 5: case 6: case 7: case 8:
			EXPECT_FALSE(board.isAlive(origin));
			break;

		// "... lives on to the next generation."
		case 2: case 3:
			EXPECT_TRUE(board.isAlive(origin));
			break;

		// This shouldn't happen.
		default:
			FAIL() << "(0, 0) has more than 8 or fewer than 0 neighbors.";
		}
	}
}

TEST(BoardTest, RulesForDeadCells) {
	Cell origin = Cell(0, 0);
	std::array<Cell, 8> neighbors = Board().getNeighbors(origin);

	for (int neighbors_count = 0; neighbors_count < 9; neighbors_count += 1) {
		Board board;
		EXPECT_FALSE(board.isAlive(origin));
		for (int i = 0; i < neighbors_count; i += 1) {
			board.addCell(neighbors[i]);
		}
		board.advanceBoard();

		// "... as if by reproduction."
		if (neighbors_count == 3) {
			EXPECT_TRUE(board.isAlive(origin));
		} else if (0 <= neighbors_count && neighbors_count <= 8) {
			EXPECT_FALSE(board.isAlive(origin));
		} else {
			// This shouldn't happen.
			FAIL() << "(0, 0) has more than 8 or fewer than 0 neighbors.";
		}
	}
}
