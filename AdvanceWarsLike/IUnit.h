#ifndef IUNIT_H_
#define IUNIT_H_
#pragma once

#include <SFML/Graphics.hpp>

#include "GraphicsComponent.h"

class IUnit : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~IUnit() {}

	virtual void setGraphicsComponent(GraphicsComponent *graphics) = 0;
	virtual void update(const sf::Time &elapsedTime) = 0;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif // IUNIT_H_
