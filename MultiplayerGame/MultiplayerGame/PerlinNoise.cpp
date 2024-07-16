#include "PerlinNoise.h"
#include <iostream>

using namespace std;
using namespace sf;

PerlinNoise::PerlinNoise() {
	// Initialize the noiseSeed1D array with random values
	size = 1024;
	noiseSeed1D = new float[size];
	perlinNoise1D = new float[size];

	// Set the number of octaves and the scale
	octaves = 10;
	scale = 2.0f;
	bias = 0.7f;

	changePerlinNoise1DSeed();

	// Initialize the noiseSeed2D array with random values
	width = 1024;
	height = 1024;
	noiseSeed2D = new float[width * height];
	perlinNoise2D = new float[width * height];

	changePerlinNoise2DSeed();
}

void PerlinNoise::changePerlinNoise1DSeed() {
	// Change the seed of the perlin noise
	for (int i = 0; i < size; i++)
		noiseSeed1D[i] = (float)rand() / RAND_MAX;

	generatePerlinNoise1D();
}
void PerlinNoise::generatePerlinNoise1D() {
	// Generate the perlin noise values
	for (int i = 0; i < size; i++) {
		float noise = 0.0f;
		float localScale = scale;
		float scaleAcc = 0.0f;
		for (int j = 0; j < octaves; j++) {
			int pitch = size >> j;
			int sample1 = (i / pitch) * pitch;
			int sample2 = (sample1 + pitch) % size;

			float blend = (float)(i - sample1) / (float)pitch;
			float sample = (1.0f - blend) * noiseSeed1D[sample1] + blend * noiseSeed1D[sample2];

			noise += sample * localScale;
			scaleAcc += localScale;
			localScale *= bias;
		}

		// Normalize the noise to be between 0 and 1
		perlinNoise1D[i] = noise / scaleAcc;
	}
}
void PerlinNoise::saveNoise1D(string path) {
	//Save the perlin noise values as an image to the specified path (make it square and represent the 1D perlin noise as a 2D image)
	Image image;
	image.create(size, size, Color::White);

	//If the value is 0, don't draw anything on the first column, if the value is 1 draw 255 pixels on the first column
	for (int i = 0; i < size; i++) {
		int value = (int)(perlinNoise1D[i] * size);
		for (int j = 0; j < value; j++) {
			image.setPixel(i, size - j - 1, Color::Black);
		}
	}

	image.saveToFile(path);
}

void PerlinNoise::changePerlinNoise2DSeed() {
	// Change the seed of the perlin noise
	for (int i = 0; i < width * height; i++)
		noiseSeed2D[i] = (float)rand() / RAND_MAX;

	generatePerlinNoise2D();
}
void PerlinNoise::generatePerlinNoise2D() {
	// Generate the perlin noise values
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float noise = 0.0f;
			float localScale = scale;
			float scaleAcc = 0.0f;
			for (int k = 0; k < octaves; k++) {
				int pitch = width >> k;
				int sampleX1 = (i / pitch) * pitch;
				int sampleY1 = (j / pitch) * pitch;

				int sampleX2 = (sampleX1 + pitch) % width;
				int sampleY2 = (sampleY1 + pitch) % height;

				float blendX = (float)(i - sampleX1) / (float)pitch;
				float blendY = (float)(j - sampleY1) / (float)pitch;

				float sampleT = (1.0f - blendX) * noiseSeed2D[sampleY1 * width + sampleX1] + blendX * noiseSeed2D[sampleY1 * width + sampleX2];
				float sampleB = (1.0f - blendX) * noiseSeed2D[sampleY2 * width + sampleX1] + blendX * noiseSeed2D[sampleY2 * width + sampleX2];

				noise += (blendY * (sampleB - sampleT) + sampleT) * localScale;
				scaleAcc += localScale;
				localScale *= bias;
			}

			// Normalize the noise to be between 0 and 1
			perlinNoise2D[j * width + i] = noise / scaleAcc;
		}
	}
}
void PerlinNoise::saveNoise2D(string path) {
	//Save the perlin noise values as an image to the specified path
	Image image;
	image.create(width, height, Color::White);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int value = (int)(perlinNoise2D[j * width + i] * 255);
			image.setPixel(i, j, Color(value, value, value));
		}
	}

	image.saveToFile(path);
}