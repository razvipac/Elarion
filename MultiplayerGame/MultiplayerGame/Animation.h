#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class Animation {
private:
	std::vector<sf::IntRect> frames;

	float duration;
	bool loop;
	float time;
public:
	Animation();
	Animation(const std::vector<sf::IntRect>& frames, float duration, bool loop);
	const sf::IntRect& getFrame(unsigned int index) const;
	void setFrames(const std::vector<sf::IntRect>& frames);
	void addFrame(const sf::IntRect& frame);
	void removeFrame(unsigned int index);
	void removeAllFrames();

	float getDuration() const;
	void setDuration(float duration);

	bool getLoop() const;
	void setLoop(bool loop);

	const sf::IntRect& getCurrentFrame() const;
	int getCurrentFrameIndex() const;

	void createAnimation(const std::string& name, float duration, bool isLooped,int height = 64, int width = 96);
	void saveAnimation(const std::string& filename) const;
	void loadAnimation(const std::string& filename);

	unsigned int getSize() const;
	void resetTime();
	bool update(float deltaTime);
};