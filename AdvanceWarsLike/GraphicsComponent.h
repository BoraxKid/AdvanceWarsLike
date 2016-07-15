#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_
#pragma once

#include "IComponent.h"
#include "AnimatedSprite.h"

class GraphicsComponent : public IComponent, public sf::Drawable, public sf::Transformable
{
public:
	GraphicsComponent(AnimatedSprite &sprite)
		: _sprite(sprite)
	{}

	virtual ~GraphicsComponent()
	{}

	void setColor(sf::Color color)
	{
	}

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= this->getTransform();
		target.draw(this->_sprite, states);
	}

	AnimatedSprite &_sprite;
};

#endif // GRAPHICSCOMPONENT_H_
