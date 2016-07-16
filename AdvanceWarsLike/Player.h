#ifndef PLAYER_H_
#define PLAYER_H_
#pragma once

#include "IUnit.h"
#include "MapManager.h"

class Player
{
public:
	Player(sf::Uint8 id, MapManager &mapManager);
	virtual ~Player();

	void moveUnit();
	bool click(const sf::Vector2i &tilePos);
	void destroyUnit(IUnit *unit);
	void addUnit(IUnit *unit);
	const std::vector<IUnit *> &getUnits() const;
	const sf::Vector2u &getAimedTile() const;
	const sf::Uint8 &getId() const;
	void setMapSize(const sf::Vector2u &mapSize);
	void drawMovement(sf::RenderWindow &window);

private:
	void resetMovementMap();
	void calculateMovement(const sf::Vector2i &tilePos, sf::Uint8 movement = 5);
	bool checkMovement(const sf::Vector2i &tilePos, sf::Uint8 movement = 5) const;

	const sf::Uint8 _id;
	MapManager &_mapManager;
	sf::Vector2u _mapSize;
	std::vector<IUnit *> _units;
	std::vector<IUnit *>::iterator _selectedUnit;
	sf::Vector2u _aimedTile;
	std::vector<std::vector<sf::Int32>> _movement;
};

#endif // PLAYER_H_
