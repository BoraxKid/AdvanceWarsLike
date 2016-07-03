#ifndef UNIT_H_
#define UNIT_H_
#pragma once

#include "IUnit.h"

class Unit : public IUnit
{
public:
	Unit(sf::Vector2u position = sf::Vector2u(0, 0));
	virtual ~Unit();

	virtual void setGraphicsComponent(GraphicsComponent *graphics);
	virtual void update(const sf::Time &elapsedTime);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	GraphicsComponent *_graphics;
};

#endif // UNIT_H_
