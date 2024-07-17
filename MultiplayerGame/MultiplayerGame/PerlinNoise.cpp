#include "PerlinNoise.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

PerlinNoise::PerlinNoise() {
	// Initialize the noiseSeed1D array with random values
	size = 1024;
	noiseSeed1D = new float[size];
	perlinNoise1D = new float[size];

	// Set the number of octaves and the scale
	octaves = 6;
	scale = 2.0f;
	bias = 0.7f;

	changePerlinNoise1DSeed();

	// Initialize the noiseSeed2D array with random values
	width = 256;
	height = 256;
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
void PerlinNoise::saveNoise2DColored(string path) {
	//Save the perlin noise values as an image to the specified path
	Image image;
	image.create(width, height, Color::White);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int value = (int)(perlinNoise2D[j * width + i] * 255);
			Color color;
			Color corruptColor(150, 50, 150);
			Color blendedColor;

			if (value < 50)
				color = Color(0, 0, 255); //water
			else if (value < 100)
				color = Color(255, 255, 0); //sand
			else if (value < 150)
				color = Color(0, 255, 0); //grass
			else if (value < 200)
				color = Color(139, 69, 19); //dirt
			else
				color = Color(255, 255, 255); //snow

			if(i < width / 2)
				blendedColor = color;
			else {
				float fraction = (float)(i - width / 2) / (width / 2);
				blendedColor.r = (1.0f - fraction) * color.r + fraction * corruptColor.r;
				blendedColor.g = (1.0f - fraction) * color.g + fraction * corruptColor.g;
				blendedColor.b = (1.0f - fraction) * color.b + fraction * corruptColor.b;
			}

			image.setPixel(i, j, blendedColor);
		}
	}

	image.saveToFile(path);
}
void PerlinNoise::get2DNoiseColored(int* level) {
	//Save the perlin noise values as an image to the specified path
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int value = (int)(perlinNoise2D[j * width + i] * 255);
			int index = j * width + i;
			if (value < 70)
				level[index] = 0; //water
			else if (value < 100)
				level[index] = 2; //sand
			else if (value < 150)
				level[index] = 1; //grass
			else if (value < 200)
				level[index] = 4; //dirt (does not exist yet)
			else
				level[index] = 3; //snow
		}
	}
}

void PerlinNoise::increaseOctaves() {
	octaves++;
	generatePerlinNoise1D();
	generatePerlinNoise2D();
}
void PerlinNoise::decreaseOctaves() {
	if (octaves > 1) {
		octaves--;
		generatePerlinNoise1D();
		generatePerlinNoise2D();
	}
}

void PerlinNoise::increaseScale() {
	scale += 0.1f;
	generatePerlinNoise1D();
	generatePerlinNoise2D();
}
void PerlinNoise::decreaseScale() {
	if (scale > 0.1f) {
		scale -= 0.1f;
		generatePerlinNoise1D();
		generatePerlinNoise2D();
	}
}

void PerlinNoise::increaseBias() {
	bias += 0.1f;
	generatePerlinNoise1D();
	generatePerlinNoise2D();
}
void PerlinNoise::decreaseBias() {
	if (bias > 0.1f) {
		bias -= 0.1f;
		generatePerlinNoise1D();
		generatePerlinNoise2D();
	}
}

PerlinNoise::~PerlinNoise() {
	delete[] noiseSeed1D;
	delete[] perlinNoise1D;
	delete[] noiseSeed2D;
	delete[] perlinNoise2D;
}