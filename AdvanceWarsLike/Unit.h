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
	virtual sf::Uint32 getHeight() const;
	virtual const sf::Uint8 &getMovement() const;
	virtual void setTilePosition(const sf::Vector2u &position);
	virtual sf::Vector2u getTilePosition() const;
	virtual void setSpritePosition(const sf::Vector2f &position);
	virtual void setPlayer(sf::Uint8 id);
	virtual const sf::Uint8 &getPlayerId() const;
	virtual void acted();
	virtual bool hasActed() const;
	virtual void update(const sf::Time &elapsedTime);
	virtual void resetState();

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Uint8 _playerId;
	sf::Vector2u _position;
	bool _acted;

	GraphicsComponent *_graphics;
	StatisticsComponent *_stats;
};

#endif // UNIT_H_
