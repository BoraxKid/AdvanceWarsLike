#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_
#pragma once

#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
	AnimatedSprite(const sf::Time frameTime = sf::seconds(0.5f));
	AnimatedSprite(const AnimatedSprite &cpy);
	AnimatedSprite &operator=(const AnimatedSprite &cpy);
	virtual ~AnimatedSprite();

	void update(const sf::Time &elapsed);
	void setTexture(const sf::Texture *texture);
	void addFrame(sf::IntRect rect);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	const sf::Texture *_texture;
	std::vector<sf::IntRect> _frames;
	std::vector<sf::Sprite> _sprites;
	std::vector<sf::Sprite>::iterator _currentFrame;
	sf::Time _frameTime;
	sf::Time _elapsedTime;
};

#endif // ANIMATEDSPRITE_H_