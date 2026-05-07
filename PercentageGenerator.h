#pragma once
#include <random>
class RandomNumberGenerator {
	std::mt19937 gen;
public:
	std::random_device rd;
	RandomNumberGenerator() { gen.seed(rd()); };
	float getFloat(float top) { std::uniform_real_distribution<float> dist(0.0f, top); return dist(gen); }
	int getInt(int top) { std::uniform_int_distribution<int> dist(0, top); return dist(gen); }
};