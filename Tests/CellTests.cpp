#include "gtest/gtest.h"
#include "cell.h"

#include <functional>

using Conway::Cell;

TEST(ConwayCell, JavaStyleIntializers) {
	Cell cell;
	EXPECT_EQ(Cell(0, 0), cell);
}

TEST(ConwayCell, EqualityOperators) {
	EXPECT_EQ(Cell(1, 2), Cell(1, 2));
	EXPECT_NE(Cell(1, 2), Cell(2, 1));
}

TEST(ConwayCell, Hashing) {
	auto hash = std::hash<Cell>();
	EXPECT_EQ(hash(Cell(1, 2)), hash(Cell(1, 2)));
	EXPECT_NE(hash(Cell(1, 2)), hash(Cell(2, 1)));
}
