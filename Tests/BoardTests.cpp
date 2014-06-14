#include "gtest/gtest.h"
#include "board.h"

#include <array>

using namespace Conway;

TEST(ConwayBoard, CountLivingNeighbors) {
	// TODO: Move this into startup code.
	std::array<Cell, 8> neighbors = {
		Cell(-1, -1),
		Cell(-1,  0),
		Cell(-1,  1),
		Cell( 0, -1),
		Cell( 0,  1),
		Cell( 1, -1),
		Cell( 1,  0),
		Cell( 1,  1),
	};
	Board board;
	Cell origin = Cell(0, 0);

	EXPECT_EQ(0, board.countLivingNeighbors(origin));

	for (size_t i = 0; i < neighbors.size(); i += 1) {
		board.addCell(neighbors[i]);
		EXPECT_EQ(i + 1, board.countLivingNeighbors(origin));
	}

	for (Cell cell : neighbors) {
		if (cell.x == 0 || cell.y == 0) {
			EXPECT_EQ(4, board.countLivingNeighbors(cell));
		} else {
			EXPECT_EQ(2, board.countLivingNeighbors(cell));
		}
	}
}

TEST(ConwayBoard, GetNeighbors) {
	// TODO: Move this into startup code.
	std::array<Cell, 8> neighbors = {
		Cell(-1, -1),
		Cell(-1,  0),
		Cell(-1,  1),
		Cell( 0, -1),
		Cell( 0,  1),
		Cell( 1, -1),
		Cell( 1,  0),
		Cell( 1,  1),
	};
	Board board;
	Cell origin = Cell(0, 0);

	for (size_t i = 0; i < neighbors.size(); i += 1) {
		board.addCell(neighbors[i]);
	}

	auto actual_neighbors = board.getNeighbors(origin);
	// We don't really care about the order Board::getNeighbors() returns
	//     things, so we'll treat it as a mathematical set.
	// Two sets are equivilent iff everything in one is in the either.
	for (Cell cell : actual_neighbors) {
		EXPECT_NE(
			std::find(neighbors.begin(), neighbors.end(), cell),
			neighbors.end());
	}

	for (Cell cell : neighbors) {
		EXPECT_NE(
			std::find(actual_neighbors.begin(), actual_neighbors.end(), cell),
			actual_neighbors.end());
	}
}

// This is the important test.
// TODO: Find patterns to manually add which exhaustively test the game rules.
TEST(ConwayBoard, AdvanceBoard) {
	EXPECT_EQ("TODO: Implement me!", std::string());
}
