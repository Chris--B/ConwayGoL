#include "cell.h"

using namespace Conway;

Cell& Cell::operator=(const Cell& cell) {
	x = cell.x;
	y = cell.y;
	return *this;
}

bool Cell::operator==(const Cell& cell) const {
	return x == cell.x && y == cell.y;
}

bool Cell::operator!=(const Cell& cell) const {
	return !(*this == cell);
}

