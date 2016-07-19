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

	virtual void setGraphicsComponent(const sf::String &sprite, GraphicsComponent *graphics) = 0;
	virtual void setStatisticsComponent(StatisticsComponent *stats) = 0;
	virtual void changeSprite(const sf::String &sprite) = 0;
	virtual sf::Uint32 getHeight() const = 0;
	virtual const sf::Uint8 &getMovement() const = 0;
	virtual void setTilePosition(const sf::Vector2u &position) = 0;
	virtual sf::Vector2u getTilePosition() const = 0;
	virtual void setSpritePosition(const sf::Vector2f &position) = 0;
	virtual void setPlayer(sf::Uint8 id) = 0;
	virtual const sf::Uint8 &getPlayerId() const = 0;
	virtual void acted() = 0;
	virtual bool hasActed() const = 0;
	virtual void update(const sf::Time &elapsedTime) = 0;
	virtual void resetState() = 0;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif // IUNIT_H_
