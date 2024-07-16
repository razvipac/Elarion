#pragma once
#include "SFML/Graphics.hpp"

class PerlinNoise {
private:
	int octaves;
	float scale;
	float bias;

	float* noiseSeed1D; // 1D array of random floats
	float* perlinNoise1D; // 1D array of perlin noise values
	int size; // size of the 1D array

	float* noiseSeed2D; // 2D array of random floats
	float* perlinNoise2D; // 2D array of perlin noise values
	int width; // width of the 2D array
	int height; // height of the 2D array


public:
	PerlinNoise();
	void changePerlinNoise1DSeed();
	void generatePerlinNoise1D();
	void saveNoise1D(std::string path);

	void changePerlinNoise2DSeed();
	void generatePerlinNoise2D();
	void saveNoise2D(std::string path);
};