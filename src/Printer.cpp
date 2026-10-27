#include "Printer.h"

#include <iostream>
#include <cctype>

std::stack<std::string> Printer::s_Texts;
const std::unordered_set<std::string> Printer::s_FUNCTIONS = {
	s_DIG_STR,
	s_FLAG_STR,
	s_UNFLAG_STR
};

void Printer::Print(size_t count, const std::string& text)
{
	// Clear text
	CursorUpBy(count);

	// Print
	s_Texts.push(text);
	std::cout << text;
}

void Printer::CursorUpBy(size_t count)
{
	while (count > 0 && !s_Texts.empty())
	{
		size_t line = GetLine();

		// Move to the first line of the previous output
		if (line > 1)
			std::cout << "\033[" << line - 1 << "A";

		// Clear each line
		for (size_t j = 0; j < line; ++j)
		{
			std::cout << "\r\033[2K";
			if (j < line - 1)
				std::cout << "\033[1B";
		}

		// Return to the first line
		if (line > 1)
			std::cout << "\033[" << line - 1 << "A";

		s_Texts.pop();
		--count;
	}
}

size_t Printer::GetLine() // Currently the implementation break when the text wrap to the next console line
{
	size_t result = 1;
	for (const auto& i : s_Texts.top())
	{
		if (i == '\n')
			result++;
	}

	return result;
}

Point Printer::GetPoint()
{
	std::string row, col;
	std::cin >> row >> col;

	for (size_t i = 0; i < row.size(); i++)
	{
		if (!std::isdigit(row[i]))
			return POINT_ERROR;
	}

	for (size_t i = 0; i < col.size(); i++)
	{
		if (!std::isdigit(col[i]))
			return POINT_ERROR;
	}

	return Point{ std::stoi(row), std::stoi(col) };
}

Input Printer::ParseInput()
{
	std::string function;
	std::cin >> function;

	function = ToLower(function);
	if (s_FUNCTIONS.count(function) == 0)
		return INPUT_ERROR;

	Point point = GetPoint();
	if (point == POINT_ERROR)
		return INPUT_ERROR;

	return Input{ function, point };
}

std::string Printer::ToLower(std::string text)
{
	for (char& c : text)
	{
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	}

	return text;
}

Point Printer::PromptPoint(const std::string& prompt) // TODO: Error still doesnt delete well
{
	size_t offset = 0;
	Point point;
	do
	{
		Print(offset, prompt);
		offset = 1;
	} while ((point = GetPoint()) == POINT_ERROR);

	std::cout << "\033[1A"; // Move up from the line after Enter (TODO: Warp this in a helper func its disgusting)
	CursorUpBy(offset);

	return point;
}

Input Printer::PromptInput(const std::string& prompt) // TODO: Error still doesnt delete well
{
	size_t offset = 0;
	Input input;
	do
	{
		Print(offset, prompt);
		offset = 1;
	} while ((input = ParseInput()) == INPUT_ERROR);

	std::cout << "\033[1A"; // Move up from the line after Enter (TODO: Warp this in a helper func its disgusting)
	CursorUpBy(offset);

	return input;
}
