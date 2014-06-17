#include "cell.h"
#include <gtest/gtest.h>

#include <functional>
#include <unordered_set>

using namespace Conway;

TEST(CellTest, HashCollisions) {
	auto hasher = std::hash<Cell>();

	// Is the hash determinsitic?
	EXPECT_EQ(hasher(Cell(55, 27)), hasher(Cell(55, 27)));

	// Make sure the order changes the hash!
	EXPECT_NE(hasher(Cell(1, 2)), hasher(Cell(2, 1)));

	// A couple of sanity checks.
	EXPECT_NE(hasher(Cell(0, 0)), hasher(Cell(0, 1)));
	EXPECT_NE(hasher(Cell(0, 0)), hasher(Cell(1, 0)));

}
