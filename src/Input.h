#pragma once

#include <string>

#include "Point.h"

struct Input
{
	std::string function;
	Point point;

	bool operator==(const Input& other) const
	{
		return (function == other.function) && (point == other.point);
	}

	Input& operator--()
	{
		--point;
		return *this;
	}

	Input operator--(int)
	{
		Input old = *this;
		--(*this);
		return old;
	}
};

#define INPUT_ERROR Input{ "unknown", Point{ -1, -1 } }