#ifndef MAPMANAGER_H_
#define MAPMANAGER_H_
#pragma once

#include <vector>

#include "ResourcesManager.h"
#include "MapLoader.h"
#include "Map.h"

class MapManager
{
public:
	MapManager(ResourcesManager &resourcesManager);
	virtual ~MapManager();

	void loadMap(const char *fileName);
	bool addUnit(IUnit *unit, sf::Vector2u position);
	IUnit *getUnit(sf::Vector2u position);
	sf::Vector2u getMapSize() const;
	bool canMove(const sf::Vector2u &unitPosition, const sf::Vector2u &position);
	void move(const sf::Vector2u &unitPosition, const sf::Vector2u &position);
	void draw(sf::RenderWindow &window) const;

private:
	ResourcesManager &_resourcesManager;
	MapLoader _mapLoader;
	std::vector<Map> _activeMaps;
};

#endif // MAPMANAGER_H_
