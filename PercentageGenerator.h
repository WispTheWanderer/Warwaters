#pragma once
#include <random>
class PercentageGenerator {
	std::mt19937 gen;
public:
	PercentageGenerator() { gen.seed(std::time(nullptr)); };
	float getPercentage(float top) { std::uniform_real_distribution<float> dist(0.0f, top); return dist(gen); }
	int getInt(int top) { std::uniform_int_distribution<int> dist(0, top); return dist(gen); }
};