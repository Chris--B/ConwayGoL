#include "gtest/gtest.h"
#include "cell.h"

TEST(FooBar, JustATest) {
	using Conway::Cell;
	Cell cell;
	EXPECT_EQ(Cell(0, 0), cell);
}
