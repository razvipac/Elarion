#include "TextureManager.h"

using namespace std;
using namespace sf;

TextureManager::TextureManager() {}

void TextureManager::loadTexture(const string& name, const string& filename) {
	Texture texture;
	if (texture.loadFromFile(filename)) {
		textures[name] = texture;
	}
	else {
		cout << "Failed to load texture: " << filename << endl;
	}
}
Texture& TextureManager::getRef(const string& texture) {
	return textures.at(texture);
}

