#include "RandomGenerator.h"
#include <time.h>


RandomGenerator::RandomGenerator()
{
	seed = time(0);
}


RandomGenerator::RandomGenerator(int newSeed, int min, int pmax)
{
	max = pmax;
	seed = newSeed;
	generator = std::mt19937(seed);
	distribution = std::uniform_real_distribution<>(min,max);
}

double RandomGenerator::nextRandom()
{
	return distribution(generator);
}

int RandomGenerator::getMax() const
{
	return max;
}
