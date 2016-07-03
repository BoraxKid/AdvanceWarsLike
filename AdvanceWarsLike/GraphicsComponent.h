#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_
#pragma once

#include "IComponent.h"
#include "AnimatedSprite.h"

class GraphicsComponent : public IComponent
{
public:
	GraphicsComponent(AnimatedSprite &sprite)
		: _sprite(sprite)
	{}

	virtual ~GraphicsComponent()
	{}

	void draw(sf::RenderTarget &target, sf::RenderStates &states) const
	{
		target.draw(this->_sprite, states);
	}

private:
	AnimatedSprite &_sprite;
};

#endif // GRAPHICSCOMPONENT_H_
