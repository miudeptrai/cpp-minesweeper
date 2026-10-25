#include "Application.h"

#include <memory>
#include <string>
#include <iostream>

static constexpr size_t s_SIZE_INDEX = 1;
static constexpr size_t s_MINE_CHANCE_INDEX = 2;

int main(int argc, char** argv)
{
	// Validate argv
	if (argc <= s_MINE_CHANCE_INDEX || argc <= s_SIZE_INDEX)
	{
		std::cout << "Not enough arguments received, expected 2 'size mine_chance', got " << argc - 1 << '\n';
		return 1;
	}

	int size;
	int mine_chance;
	try
	{
		size = std::stoi(argv[s_SIZE_INDEX]);
		mine_chance = std::stoi(argv[s_MINE_CHANCE_INDEX]);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Invalid argument(s), expected 2 'size mine_chance'" << '\n';
		return 1;
	}

	if (size <= 0)
	{
		std::cout << "Size must be a positive integer\n";
		return 1;
	}

	// Run
	std::unique_ptr<Application> app = std::make_unique<Application>();
	app->Run(static_cast<size_t>(size), mine_chance);
}