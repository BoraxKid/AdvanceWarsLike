#ifndef PLAYER_H_
#define PLAYER_H_
#pragma once

#include "IUnit.h"
#include "MapManager.h"

class Player
{
public:
	Player(sf::Uint8 id);
	virtual ~Player();

	void click(MapManager &mapManager, const sf::Vector2i &tilePos);
	void addUnit(IUnit *unit);
	const std::vector<IUnit *> &getUnits() const;
	void setMapSize(const sf::Vector2u &mapSize);
	void drawMovement(sf::RenderWindow &window);

private:
	void resetMovementMap();
	void calculateMovement(const sf::Vector2i &tilePos, sf::Uint8 movement = 5);
	bool checkMovement(const sf::Vector2i &tilePos, sf::Uint8 movement = 5) const;

	const sf::Uint8 _id;
	sf::Vector2u _mapSize;
	std::vector<IUnit *> _units;
	std::vector<IUnit *>::iterator _selectedUnit;
	std::vector<std::vector<sf::Int32>> _movement;
};

#endif // PLAYER_H_
