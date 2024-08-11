#include "Animation.h"
#include <fstream>
#include <iostream>
#include "TextureManager.h"

using namespace std;
using namespace sf;

Animation::Animation() : duration(0), loop(false), time(0) {}
Animation::Animation(const vector<IntRect>& frames, float duration, bool loop) : duration(duration), loop(loop) {
	setFrames(frames);
}

void Animation::setFrames(const vector<IntRect>& frames) {
	this->frames = frames;
}
void Animation::setDuration(float duration) {
	this->duration = duration;
}
void Animation::setLoop(bool loop) {
	this->loop = loop;
}
int Animation::getCurrentFrameIndex() const {
	float relativeTime = fmod(time, duration);
	return (int)(relativeTime / duration * frames.size());
}
const IntRect& Animation::getFrame(unsigned int index) const {
	return frames[index];
}
const IntRect& Animation::getCurrentFrame() const {
	return frames[getCurrentFrameIndex()];
}
float Animation::getDuration() const {
	return duration;
}
bool Animation::getLoop() const {
	return loop;
}
unsigned int Animation::getSize() const {
	return frames.size();
}
void Animation::addFrame(const IntRect& frame) {
	frames.push_back(frame);
}
void Animation::removeFrame(unsigned int index) {
	if (index < frames.size())
		frames.erase(frames.begin() + index);
}
void Animation::removeAllFrames() {
	frames.clear();
}
bool Animation::update(float deltaTime) {
	int frame = getCurrentFrameIndex();
	time += deltaTime;
	int newFrame = getCurrentFrameIndex();

	return frame != newFrame;
}
void Animation::resetTime() {
	time = 0;
}
void Animation::createAnimation(const string& name, float duration, bool isLooped, int height, int width) {
	removeAllFrames();
	Texture& texture = TextureManager::getInstance().getRef(name);
	Vector2u size = texture.getSize();
	int framesX = size.x / width;
	int framesY = size.y / height;
	for (int y = 0; y < framesY; ++y) {
		for (int x = 0; x < framesX; ++x) {
			addFrame(IntRect(x * width, y * height, width, height));
		}
	}
	setDuration(duration);
	setLoop(isLooped);
	saveAnimation(name + ".anim");
}
void Animation::saveAnimation(const string& filename) const {
	string path = "Resources/Animations/" + filename;
	ofstream file(path, ios::binary);
	if (file.is_open()) {
		unsigned int size = frames.size();
		file.write((char*)&size, sizeof(size));
		file.write((char*)&duration, sizeof(duration));
		file.write((char*)&loop, sizeof(loop));
		for (unsigned int i = 0; i < size; ++i) {
			file.write((char*)&frames[i].left, sizeof(frames[i].left));
			file.write((char*)&frames[i].top, sizeof(frames[i].top));
			file.write((char*)&frames[i].width, sizeof(frames[i].width));
			file.write((char*)&frames[i].height, sizeof(frames[i].height));
		}
		file.close();
	}
}
void Animation::loadAnimation(const string& filename) {
	string path = "Resources/Animations/" + filename;
	ifstream file(path, ios::binary);
	if (file.is_open()) {
		unsigned int size;
		file.read((char*)&size, sizeof(size));
		file.read((char*)&duration, sizeof(duration));
		file.read((char*)&loop, sizeof(loop));
		frames.resize(size);
		for (unsigned int i = 0; i < size; ++i) {
			file.read((char*)&frames[i].left, sizeof(frames[i].left));
			file.read((char*)&frames[i].top, sizeof(frames[i].top));
			file.read((char*)&frames[i].width, sizeof(frames[i].width));
			file.read((char*)&frames[i].height, sizeof(frames[i].height));
		}
		file.close();

		cout << filename << ":\n";
		cout << size << " " << duration << " " << loop << "\n";

		for (int i = 0; i < frames.size(); i++)
			cout << frames[i].left << " " << frames[i].top << " " << frames[i].width << " " << frames[i].height << "\n";
	}
	else {
		cout << "Failed to open file for reading\n";
		cout << filename << "\n";
	}
}