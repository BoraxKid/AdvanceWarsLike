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

	virtual void setGraphicsComponent(const sf::String &sprite, GraphicsComponent *graphics);
	virtual void setStatisticsComponent(StatisticsComponent *stats);
	virtual void changeSprite(const sf::String &sprite);
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
	sf::Color _color;
	std::map<sf::String, GraphicsComponent *> _graphics;
	sf::String _currentGraphics;
	StatisticsComponent *_stats;
};

#endif // UNIT_H_
