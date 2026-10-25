#pragma once

#include <random>

class RNG
{
private:
	std::random_device m_RandomDevice;
	std::mt19937 m_Generator;


public:
	RNG();
	~RNG() = default;

	int Randi(int min, int max);
};

