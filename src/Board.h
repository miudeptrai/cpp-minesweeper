#pragma once

#include <vector>
#include <string>

#include "Point.h"
#include "RNG.h"

class Board
{
private:
	static constexpr unsigned int s_MINE_NUM = 9;
	static constexpr unsigned int s_ERROR_NUM = 10;

	static constexpr unsigned char s_MINE_CHAR = 'x';
	static constexpr unsigned char s_FLAG_CHAR = 'f';
	static constexpr unsigned char s_EMPTY_CHAR = '.';

	static constexpr int s_MINE_CHANCE_MIN = 10;
	static constexpr int s_MINE_CHANCE_MAX = 90;

	std::vector<std::vector<unsigned int>> m_Board;
	std::vector<std::vector<unsigned char>> m_ViewBoard;
	size_t m_Size;
	bool m_IsAlive = true;

	RNG m_Rng;

	void Open(Point at);
	unsigned int Peek(Point at);
public:
	Board(size_t size);
	~Board() = default;

	std::string GetViewBoard() const;
	bool GenerateBoard(Point start, int mineChance);
	void Place(Point at);
	void Flag(Point at);
	bool IsAlive() const;
};
