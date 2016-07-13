#include "Map.h"

Map::Map(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager)
{
}

Map::~Map()
{
}

bool Map::addUnit(IUnit *unit, sf::Vector2u position)
{
	if (position.x > this->_size.x && position.y > this->_size.y)
		return (false);
	if (this->_units.at(position.x).at(position.y) != nullptr)
		return (false);
	this->_units.at(position.x).at(position.y) = unit;
	return (true);
}

void Map::dump() const
{
	std::map<sf::String, std::vector<std::vector<Tile>>>::const_iterator iterMap = this->_tiles.begin();

	while (iterMap != this->_tiles.end())
	{
		std::vector<std::vector<Tile>>::const_iterator iter = iterMap->second.begin();
		std::vector<Tile>::const_iterator iter2;
		while (iter != iterMap->second.end())
		{
			iter2 = (*iter).begin();
			while (iter2 != (*iter).end())
			{
				std::cout << " - " << (*iter2);
				++iter2;
			}
			std::cout << std::endl;
			++iter;
		}
		std::cout << std::endl;
		++iterMap;
	}
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	std::size_t i;
	std::size_t x = this->_size.x;
	std::size_t j;
	std::size_t y = this->_size.y;
	std::map<Tile, sf::String>::const_iterator iter;
	std::map<sf::String, std::vector<std::vector<Tile>>>::const_iterator iterMap = this->_tiles.begin();

	states.transform *= this->getTransform();
	while (iterMap != this->_tiles.end())
	{
		i = 0;
		while (i < x)
		{
			j = 0;
			while (j < y)
			{
				if (iterMap->first == "layer 1")
				{
					iter = this->_tilesNames.find(this->_buildings.at(i).at(j));
					if (iter != this->_tilesNames.end())
					{
						AnimatedSprite &sprite = this->_resourcesManager.at(iter->second);
						sprite.setPosition(sf::Vector2f(static_cast<float>(i * 16), static_cast<float>(j * 16)));
						target.draw(sprite, states);
					}
				}
				iter = this->_tilesNames.find(iterMap->second.at(i).at(j)); 
				if (iter != this->_tilesNames.end())
				{
					AnimatedSprite &sprite = this->_resourcesManager.at(iter->second);
					sprite.setPosition(sf::Vector2f(static_cast<float>(i * 16), static_cast<float>(j * 16)));
					target.draw(sprite, states);
				}
				if (this->_units.at(i).at(j) != nullptr)
				{
					this->_units.at(i).at(j)->setPosition(sf::Vector2f(static_cast<float>(i * 16), static_cast<float>(j * 16)));
					target.draw(*this->_units.at(i).at(j), states);
				}
				++j;
			}
			++i;
		}
		++iterMap;
	}
}
