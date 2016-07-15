#ifndef IUNIT_H_
#define IUNIT_H_
#pragma once

#include <SFML/Graphics.hpp>

#include "GraphicsComponent.h"
#include "StatisticsComponent.h"

class IUnit : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~IUnit() {}

	virtual void setGraphicsComponent(GraphicsComponent *graphics) = 0;
	virtual void setStatisticsComponent(StatisticsComponent *stats) = 0;
	virtual const sf::Uint8 getMovement() = 0;
	virtual sf::Vector2u getTilePosition() const = 0;
	virtual void update(const sf::Time &elapsedTime) = 0;
	virtual void move(sf::Vector2u position) = 0;
	virtual void setPlayer(sf::Uint8 id) = 0;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif // IUNIT_H_
