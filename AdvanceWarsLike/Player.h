#ifndef PLAYER_H_
#define PLAYER_H_
#pragma once

#include <stack>

#include "IUnit.h"
#include "IBuilding.h"
#include "MapManager.h"

class Player : public sf::Drawable, public sf::Transformable
{
public:
	enum Click { NOTINRANGE, AIMED, SELECTED, ACTED };
	Player(sf::Uint8 id, MapManager &mapManager, const sf::Font &font, sf::String player);
	virtual ~Player();

	void endTurn();
	void startTurn();
	void moveUnit();
	void attackMoveUnit();
	void endAttack();
	Click click(const sf::Vector2i &tilePos, MapManager &mapManager);
	void destroyUnit(IUnit *unit);
	void addUnit(IUnit *unit);
	void removeBuilding(IBuilding *building);
	void addBuilding(IBuilding *building);
	const std::vector<IUnit *> &getUnits() const;
	const sf::Vector2u &getAimedTile() const;
	const sf::Uint8 &getId() const;
	void setMapSize(const sf::Vector2u &mapSize);
	void drawMovement(sf::RenderWindow &window);
	const sf::Uint32 &currentBalance() const;
	bool buy(const sf::Uint32 &cost);
	std::vector<sf::Vector2u> findPath();
	void unselect();
	IUnit *getSelectedUnit() const;
	sf::Uint32 getUnitsNumber() const;

private:
	void resetMovementMap();
	void calculateMovement(MapManager &mapManager, const sf::Vector2i &tilePos, sf::Uint8 movement = 5);
	void checkTile(MapManager &mapManager, std::vector<sf::Vector2i> &q, sf::Vector2i pos, sf::Vector2i pos2);
	bool checkMovement(const sf::Vector2i &tilePos, sf::Uint8 movement = 5) const;
	void checkBuildings();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	const sf::Uint8 _id;
	MapManager &_mapManager;
	PlayerGraphicsComponent _graphics;
	sf::Vector2u _mapSize;
	sf::Uint32 _money;
	std::vector<IUnit *> _units;
	std::vector<IUnit *>::iterator _selectedUnit;
	std::vector<IBuilding *> _buildings;
	sf::Vector2u _aimedTile;
	std::vector<std::vector<sf::Int32>> _movement;
	sf::Uint32 _towers;
	bool _moving;
};

#endif // PLAYER_H_
