#pragma once

#include <array>

namespace Conway {
	class Cell {
	public:
		Cell()
			: x(0), y(0) {}
		Cell(int x, int y)
			: x(x), y(y) {}

		Cell& operator=(const Cell& cell);
		bool operator==(const Cell& cell) const;

		std::array<Cell, 8> getNeighbors() const;

		int x, y;
	};
} // namespace Conway

// hash is used for unordered_set
namespace std {
	template<>
	class hash<Conway::Cell> {
	public:
		inline size_t operator()(const Conway::Cell& cell) const {
			return std::hash<int>()(cell.x) ^ std::hash<int>()(cell.y);
		}
	};
} // namepsace std

