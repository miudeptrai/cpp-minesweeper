#include "RNG.h"


RNG::RNG()
	: m_Generator(m_RandomDevice())
{

}

int RNG::Randi(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(m_Generator);
}