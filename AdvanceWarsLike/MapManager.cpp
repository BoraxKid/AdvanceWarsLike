#include "MapManager.h"

MapManager::MapManager(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager)
{
}

MapManager::~MapManager()
{
}

void MapManager::loadMap(const char *fileName)
{
	this->_activeMaps.push_back(Map(this->_resourcesManager));
	if (!this->_mapLoader.loadMap(this->_activeMaps.back(), fileName))
		this->_activeMaps.pop_back();
}

void MapManager::removeUnit(const sf::Vector2u &position)
{
	if (!this->_activeMaps.empty())
		this->_activeMaps.back().removeUnit(position);
}

bool MapManager::addUnit(IUnit *unit, sf::Vector2u position)
{
	if (!this->_activeMaps.empty())
		return (this->_activeMaps.back().addUnit(unit, position));
	return (false);
}

IUnit *MapManager::getUnit(const sf::Vector2u &position)
{
	if (this->_activeMaps.empty())
		return (nullptr);
	return (this->_activeMaps.back().getUnit(position));
}

sf::Vector2u MapManager::getMapSize() const
{
	if (!this->_activeMaps.empty())
		return (this->_activeMaps.back().getMapSize());
	return (sf::Vector2u(0, 0));
}

sf::Vector2u MapManager::getTileSize() const
{
	if (!this->_activeMaps.empty())
		return (this->_activeMaps.back().getTileSize());
	return (sf::Vector2u(0, 0));
}

const std::vector<std::vector<IBuilding *>> &MapManager::getBuildings() const
{
	return (this->_activeMaps.back().getBuildings());
}

bool MapManager::canMove(const sf::Vector2u & unitPosition, const sf::Vector2u & position)
{
	if (!this->_activeMaps.empty())
		return (this->_activeMaps.back().canMove(unitPosition, position));
	return (false);
}

void MapManager::move(const sf::Vector2u &unitPosition, const sf::Vector2u &position)
{
	if (!this->_activeMaps.empty())
		this->_activeMaps.back().move(unitPosition, position);
}

void MapManager::draw(sf::RenderWindow &window) const
{
	std::vector<Map>::const_iterator iter = this->_activeMaps.begin();
	while (iter != this->_activeMaps.end())
	{
		window.draw(*iter);
		++iter;
	}
}
