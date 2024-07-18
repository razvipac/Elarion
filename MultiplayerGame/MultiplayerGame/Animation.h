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
	void setFrames(const std::vector<sf::IntRect>& frames);
	void setDuration(float duration);
	void setLoop(bool loop);

	int getCurrentFrameIndex() const;
	const sf::IntRect& getFrame(unsigned int index) const;
	const sf::IntRect& getCurrentFrame() const;
	float getDuration() const;
	bool getLoop() const;
	unsigned int getSize() const;
	void addFrame(const sf::IntRect& frame);
	void removeFrame(unsigned int index);

	bool update(float deltaTime);
	void resetTime();

	void saveAnimation(const std::string& filename) const;
	void loadAnimation(const std::string& filename);

};