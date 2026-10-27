#include "Board.h"

#include <unordered_set>
#include <queue>

Board::Board(size_t size)
	: m_Size(size)
	, m_Board(std::vector<std::vector<unsigned int>>(size, std::vector<unsigned int>(size)))
	, m_ViewBoard(std::vector<std::vector<unsigned char>>(size, std::vector<unsigned char>(size, s_EMPTY_CHAR)))
	, m_Mines(std::vector<std::vector<unsigned short>>(size, std::vector<unsigned short>(size, 0)))
{
}

std::string Board::GetViewBoard() const
{
	std::string result;
	for (const auto& row : m_ViewBoard)
	{
		for (const auto& c : row)
		{
			result += c;
			result += ' ';
		}
		result += '\n';
	}

	return result;
}

bool Board::GenerateBoard(Point start, int mineChance)
{
	if (start.row < 0 || start.row >= m_Size || start.col < 0 || start.col >= m_Size)
		return false;

	// Clamp minechance
	if (mineChance < s_MINE_CHANCE_MIN)
		mineChance = s_MINE_CHANCE_MIN;
	if (mineChance > s_MINE_CHANCE_MAX)
		mineChance = s_MINE_CHANCE_MAX;

	// Determine mine cells
	std::unordered_set<Point, PointHash> startAdj = {
		{start.row + 1, start.col},
		{start.row - 1, start.col},
		{start.row, start.col + 1},
		{start.row, start.col - 1},
		{start.row + 1, start.col + 1},
		{start.row - 1, start.col - 1},
		{start.row + 1, start.col - 1},
		{start.row - 1, start.col + 1}
	};
	for (size_t row = 0; row < m_Size; row++)
	{
		for (size_t col = 0; col < m_Size; col++)
		{
			// Conventionally the tile player start should be 0
			Point cur = Point{ static_cast<int>(row), static_cast<int>(col) };
			if (startAdj.count(cur) > 0 || cur == start)
				continue;

			int p = m_Rng.Randi(1, 100);
			if (p <= mineChance)
			{
				m_Board[row][col] = s_MINE_NUM;
			}
		}
	}

	// Calc cells number
	std::vector<Point> adjAdd = {
		{1, 0}, 
		{-1, 0}, 
		{0, 1}, 
		{0, -1},
		{1, 1},
		{-1, -1},
		{1, -1},
		{-1, 1}
	};
	for (size_t row = 0; row < m_Size; row++)
	{
		for (size_t col = 0; col < m_Size; col++)
		{
			if (m_Board[row][col] == s_MINE_NUM)
				continue;

			// Count mines in adj cells
			unsigned int mineCount = 0;
			for (const auto& add : adjAdd)
			{
				int newRow = static_cast<int>(row) + add.row;
				int newCol = static_cast<int>(col) + add.col;
				if (newRow < 0 || newRow >= m_Size || newCol < 0 || newCol >= m_Size)
					continue;

				mineCount += m_Board[newRow][newCol] == s_MINE_NUM ? 1 : 0;
			}
			m_Board[row][col] = mineCount;
		}
	}

	return true;
}

void Board::Open(Point at)
{
	if (at.row < 0 || at.row >= m_Size || at.col < 0 || at.col >= m_Size)
		return;

	switch (m_Board[at.row][at.col])
	{
	case s_MINE_NUM:
		m_ViewBoard[at.row][at.col] = s_MINE_CHAR;
		break;
	default:
		m_ViewBoard[at.row][at.col] = static_cast<unsigned char>(m_Board[at.row][at.col] + '0');
		break;
	}
}

unsigned int Board::Peek(Point at)
{
	if (at.row < 0 || at.row >= m_Size || at.col < 0 || at.col >= m_Size)
		return s_ERROR_NUM;

	return m_Board[at.row][at.col];
}

void Board::Place(Point at)
{
	if (at.row < 0 || at.row >= m_Size || at.col < 0 || at.col >= m_Size)
		return;

	if (Peek(at) == s_MINE_NUM)
		m_IsAlive = false;

	std::vector<Point> adjAdd = {
		{1, 0},
		{-1, 0},
		{0, 1},
		{0, -1},
		{1, 1},
		{-1, -1},
		{1, -1},
		{-1, 1}
	};
	std::vector<std::vector<short>> visited(m_Size, std::vector<short>(m_Size, 0));
	std::queue<Point> queue;
	queue.push(at);
	
	// BFS
	while (!queue.empty())
	{
		Point cur = queue.front();
		queue.pop();

		if (visited[cur.row][cur.col] > 0)
			continue;

		visited[cur.row][cur.col] = 1;

		unsigned int cell = Peek(cur);

		// If not alive then open every cells
		if (m_IsAlive && cell == s_MINE_NUM)
			continue;

		Open(cur);

		// Add adj to queue if 0
		if (m_IsAlive && cell != 0)
			continue;

		for (const auto& add : adjAdd)
		{
			int newRow = cur.row + add.row;
			int newCol = cur.col + add.col;
			if (newRow < 0 || newRow >= m_Size || newCol < 0 || newCol >= m_Size)
				continue;

			queue.push(Point{ newRow, newCol });
		}
	}
}

void Board::Flag(Point at)
{
	if (at.row < 0 || at.row >= m_Size || at.col < 0 || at.col >= m_Size)
		return;

	m_ViewBoard[at.row][at.col] = s_FLAG_CHAR;
	if (m_Board[at.row][at.col] == s_MINE_NUM)
	{
		m_Mines[at.row][at.col] = 1;
	}
}

void Board::Unflag(Point at)
{
	if (at.row < 0 || at.row >= m_Size || at.col < 0 || at.col >= m_Size)
		return;

	m_ViewBoard[at.row][at.col] = s_EMPTY_CHAR;
	if (m_Board[at.row][at.col] == s_MINE_NUM)
	{
		m_Mines[at.row][at.col] = 0;
	}
}

bool Board::IsAlive() const
{
	return m_IsAlive;
}

bool Board::Won() const
{
	for (size_t row = 0; row < m_Size; row++)
	{
		for (size_t col = 0; col < m_Size; col++)
		{
			if ((m_Mines[row][col] == 0) && (m_Board[row][col] == s_MINE_NUM))
				return false;
		}
	}

	return true;
}
