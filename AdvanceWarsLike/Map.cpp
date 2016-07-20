#include "Map.h"

Map::Map(ResourcesManager &resourcesManager, const sf::Font &font)
	: _resourcesManager(resourcesManager), _font(font)
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

sf::Vector2u Map::getTileSize() const
{
	return (this->_tileSize);
}

sf::Uint8 Map::getTileMovement(Tile tile) const
{
	switch (tile)
	{
	case NONE:
		return (sf::Uint8(0));
		break;
	case GROUND:
		return (sf::Uint8(0));
		break;
	case WATER:
		return (sf::Uint8(0));
		break;
	case FOREST:
		return (sf::Uint8(1));
		break;
	case MOUNTAIN:
		return (sf::Uint8(2));
		break;
	case BUILDING:
		return (sf::Uint8(1));
		break;
	default:
		break;
	}
	return (sf::Uint8(0));
}

const std::vector<std::vector<IBuilding *>> &Map::getBuildings() const
{
	return (this->_buildings);
}

const std::vector<std::vector<IUnit *>> &Map::getUnits() const
{
	return (this->_units);
}

Tile Map::getTile(sf::Vector2i pos) const
{
	Tile tmp = NONE;
	if (pos.x >= 0 && static_cast<sf::Uint32>(pos.x) < this->_size.x && pos.y >= 0 && static_cast<sf::Uint32>(pos.y) < this->_size.y)
	{
		if (this->_buildings.at(pos.x).at(pos.y) != nullptr)
			return (BUILDING);
		std::map<sf::String, std::vector<std::vector<Tile>>>::const_iterator iter = this->_tiles.begin();
		std::map<sf::String, std::vector<std::vector<Tile>>>::const_iterator iter2 = this->_tiles.end();

		while (iter != iter2)
		{
			if (iter->second.at(pos.x).at(pos.y) != NONE)
				tmp = iter->second.at(pos.x).at(pos.y);
			++iter;
		}
	}
	return (tmp);
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
	IBuilding *building;
	IUnit *unit;
	sf::Vector2f pos;

	states.transform *= this->getTransform();
	while (iterMap != this->_tiles.end())
	{
		i = 0;
		while (i < x)
		{
			j = 0;
			while (j < y)
			{
				pos.x = static_cast<float>(i * this->_tileSize.x);
				pos.y = static_cast<float>(j * this->_tileSize.y);
				iter = this->_tilesNames.find(iterMap->second.at(i).at(j)); 
				if (iter != this->_tilesNames.end())
				{
					AnimatedSprite &sprite = this->_resourcesManager.at(iter->second);
					sprite.setPosition(pos.x, pos.y + this->_tileSize.y - sprite.getHeight());
					target.draw(sprite, states);
				}
				if ((building = this->_buildings.at(i).at(j)) != nullptr)
				{
					building->setPosition(sf::Vector2f(pos.x, pos.y + this->_tileSize.y - building->getHeight()));
					target.draw(*building, states);
				}
				if ((unit = this->_units.at(i).at(j)) != nullptr)
				{
					unit->setSpritePosition(sf::Vector2f(pos.x, pos.y + this->_tileSize.y - unit->getHeight()));
					target.draw(*unit, states);
				}
				++j;
			}
			++i;
		}
		++iterMap;
	}
}
