#include "Map.h"

Map::Map(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager)
{
}

Map::~Map()
{
}

void Map::removeUnit(const sf::Vector2u &position)
{
	if (position.x < this->_size.x && position.y < this->_size.y && this->_units.at(position.x).at(position.y) != nullptr)
		this->_units.at(position.x).at(position.y) = nullptr;
}

bool Map::addUnit(IUnit *unit, sf::Vector2u position)
{
	if (position.x >= this->_size.x && position.y >= this->_size.y)
		return (false);
	if (this->_units.at(position.x).at(position.y) != nullptr)
		return (false);
	this->_units.at(position.x).at(position.y) = unit;
	return (true);
}

IUnit *Map::getUnit(const sf::Vector2u &position)
{
	if (position.x >= this->_size.x || position.y >= this->_size.y)
		return (nullptr);
	return (this->_units.at(position.x).at(position.y));
}

sf::Vector2u Map::getMapSize() const
{
	return (this->_size);
}

bool Map::canMove(const sf::Vector2u &unitPosition, const sf::Vector2u &position)
{
	if (unitPosition.x < this->_size.x && unitPosition.y < this->_size.y && position.x < this->_size.x && position.y < this->_size.y)
	{
		if (this->_units.at(unitPosition.x).at(unitPosition.y) != nullptr && this->_units.at(position.x).at(position.y) == nullptr)
		{
			return (true);
		}
	}
	return (false);
}

void Map::move(const sf::Vector2u &unitPosition, const sf::Vector2u &position)
{
	if (unitPosition.x < this->_size.x && unitPosition.y < this->_size.y && position.x < this->_size.x && position.y < this->_size.y)
	{
		if (this->_units.at(unitPosition.x).at(unitPosition.y) != nullptr && this->_units.at(position.x).at(position.y) == nullptr)
		{
			IUnit *tmp = this->_units.at(unitPosition.x).at(unitPosition.y);
			this->_units.at(unitPosition.x).at(unitPosition.y) = nullptr;
			this->_units.at(position.x).at(position.y) = tmp;
			tmp->setTilePosition(position);
		}
	}
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
	sf::Uint32 i;
	sf::Uint32 x = this->_size.x;
	sf::Uint32 j;
	sf::Uint32 y = this->_size.y;
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
