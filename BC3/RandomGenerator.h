#pragma once
#include <random>

class RandomGenerator
{
private:
	int seed;
	std::mt19937 generator;
	std::uniform_real_distribution<> distribution;

	int max;
		 
public:
	RandomGenerator();
	RandomGenerator(int newSeed, int min, int max);
	~RandomGenerator() {};

	double nextRandom();

	int getMax() const;
};

