#ifndef UNIT_H_
#define UNIT_H_
#pragma once

#include <iostream>

#include "IUnit.h"

class Unit : public IUnit
{
public:
	Unit();
	virtual ~Unit();

	virtual void setGraphicsComponent(GraphicsComponent *graphics);
	virtual void setStatisticsComponent(StatisticsComponent *stats);
	virtual const sf::Uint8 &getMovement() const;
	virtual sf::Vector2u getTilePosition() const;
	virtual const sf::Uint8 &getPlayerId() const;
	virtual void update(const sf::Time &elapsedTime);
	virtual void move(sf::Vector2u position);
	virtual void setPlayer(sf::Uint8 id);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Uint8 _playerId;
	sf::Vector2u _position;
	GraphicsComponent *_graphics;
	StatisticsComponent *_stats;
};

#endif // UNIT_H_
