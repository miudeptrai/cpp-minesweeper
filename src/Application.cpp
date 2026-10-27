#include "Application.h"

#include "Board.h"
#include "Printer.h"
#include "Input.h"

#include <memory>

Application::Application()
{
}

void Application::Run(size_t boardSize, int mineChance)
{
	// Start
	Printer::Print(0, "---- miu's C++ Minesweeper\n");
	Printer::Print(0, "1. Type 'dig row col' to dig.\n");
	Printer::Print(0, "2. Type 'flag row col' to flag.\n");
	Printer::Print(0, "3. Type 'unflag row col' to unflag.\n");

	size_t offset = 0;

	std::unique_ptr<Board> board = std::make_unique<Board>(boardSize);
	Printer::Print(0, board->GetViewBoard());
	offset++;

	// Prompt
	Point start = --Printer::PromptPoint("->Starting position 'row col': ");
	board->GenerateBoard(start, mineChance);

	board->Place(start);

	while (board->IsAlive() && !board->Won())
	{
		Printer::Print(offset, board->GetViewBoard());
		
		Input input = --Printer::PromptInput("->");

		if (input.function == Printer::s_DIG_STR)
		{
			board->Place(input.point);
		}
		else if (input.function == Printer::s_FLAG_STR)
		{
			board->Flag(input.point);
		}
		else if (input.function == Printer::s_UNFLAG_STR)
		{
			board->Unflag(input.point);
		}
	}

	if (!board->IsAlive())
	{
		Printer::Print(offset, board->GetViewBoard());
		Printer::Print(0, "----You lose!\n");
	}
	else
	{
		Printer::Print(offset, board->GetViewBoard());
		Printer::Print(0, "----You win!\n");
	}
}