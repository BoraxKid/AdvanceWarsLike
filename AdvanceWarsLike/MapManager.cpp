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

bool MapManager::addUnit(IUnit *unit, sf::Vector2u position)
{
	if (!this->_activeMaps.empty())
		return (this->_activeMaps.back().addUnit(unit, position));
	return (false);
}

IUnit *MapManager::getUnit(sf::Vector2u position)
{
	if (this->_activeMaps.empty())
		return (nullptr);
	return (this->_activeMaps.back().getUnit(position));
}

sf::Vector2u MapManager::getMapSize() const
{
	return (this->_activeMaps.back().getMapSize());
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
