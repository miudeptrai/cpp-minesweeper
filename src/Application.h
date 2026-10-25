#pragma once

class Application
{
public:
	Application();
	~Application() = default;

	void Run(size_t boardSize, int mineChance);
};