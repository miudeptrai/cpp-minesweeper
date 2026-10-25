#pragma once

#include <functional>

struct Point
{
	int row;
	int col;

	bool operator==(const Point& other) const
	{
		return (row == other.row) && (col == other.col);
	}

	Point& operator--()
	{
		--row;
		--col;
		return *this;
	}

	Point operator--(int)
	{
		Point old = *this;
		--(*this);
		return old;
	}
};

struct PointHash
{
	size_t operator()(const Point& point) const
	{
		size_t h1 = std::hash<int>{}(point.row);
		size_t h2 = std::hash<int>{}(point.col);

		return h1 ^ (h2 << 1);
	}
};

#define POINT_ERROR Point{ -1, -1 }