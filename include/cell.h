#pragma once

#include <functional>
#include <ostream>

namespace Conway {
// Glorified 2d vector
struct Cell {
	Cell() : x(0), y(0) {}
	Cell(int x, int y) : x(x), y(y) {}

	Cell& operator=(const Cell& cell);
	bool operator==(const Cell& cell) const;
	bool operator!=(const Cell& cell) const;

	int x;
	int y;
};
} // namespace Conway

// hash is used for unordered_set
namespace std {
	template<>
	struct hash<Conway::Cell> {
		size_t operator()(const Conway::Cell& cell) const {
			auto hash_op = std::hash<int>();
			return hash_op(cell.x) ^ hash_op(cell.y);
		}
	};
} // namepsace std

inline std::ostream& operator<<(std::ostream& os, const Conway::Cell& cell) {
	return os << "(" << cell.x << ", " << cell.y << ")";
}
