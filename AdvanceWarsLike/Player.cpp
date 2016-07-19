#include "Player.h"

Player::Player(sf::Uint8 id, MapManager &mapManager)
	: _id(id), _mapManager(mapManager), _money(0), _selectedUnit(_units.end()), _towers(0)
{
}

Player::~Player()
{
	std::vector<IUnit *>::iterator iter = this->_units.begin();
	std::vector<IUnit *>::iterator iter2 = this->_units.end();

	while (iter != iter2)
	{
		delete (*iter);
		++iter;
	}
}

void Player::endTurn()
{
	std::vector<IUnit *>::iterator iter = this->_units.begin();
	std::vector<IUnit *>::iterator iter2 = this->_units.end();

	while (iter != iter2)
	{
		(*iter)->resetState();
		++iter;
	}
	this->_selectedUnit = this->_units.end();
}

void Player::startTurn()
{
	this->_selectedUnit = this->_units.end();
	this->checkBuildings();
	this->_money += this->_towers * 1000;
}

void Player::moveUnit()
{
	//this->_mapManager.move((*this->_selectedUnit)->getTilePosition(), this->_aimedTile);
	(*this->_selectedUnit)->acted();
	this->_selectedUnit = this->_units.end();
}

void Player::prepareAttackUnit()
{
	this->_mapManager.move((*this->_selectedUnit)->getTilePosition(), this->_aimedTile);
}

void Player::endAttack()
{
	(*this->_selectedUnit)->acted();
}

Player::Click Player::click(const sf::Vector2i &tilePos, MapManager &mapManager)
{
	std::vector<IUnit *>::iterator iter;
	std::vector<IUnit *>::iterator iter2;
	IUnit *tmp = this->_mapManager.getUnit(sf::Vector2u(tilePos.x, tilePos.y));
	bool actedUnit = false;

	if (this->_selectedUnit != this->_units.end())
	{
		if (this->checkMovement(tilePos, (*this->_selectedUnit)->getMovement()))
		{
			if (this->_mapManager.canMove((*this->_selectedUnit)->getTilePosition(), sf::Vector2u(tilePos.x, tilePos.y)))
			{
				this->_aimedTile = sf::Vector2u(tilePos.x, tilePos.y);
				return (Aimed);
			}
			else if (*this->_selectedUnit != tmp)
			{
				iter = this->_units.begin();
				iter2 = this->_units.end();
				while (iter != iter2)
				{
					if (*iter == tmp && (*iter)->hasActed() == false)
					{
						this->resetMovementMap();
						this->calculateMovement(mapManager, tilePos);
						this->_selectedUnit = iter;
						return (Selected);
					}
					else if (*iter == tmp && (*iter)->hasActed() == true)
						actedUnit = true;
					++iter;
				}
			}
		}
		else
			this->_selectedUnit = this->_units.end();
	}
	if (this->_selectedUnit == this->_units.end())
	{
		iter = this->_units.begin();
		iter2 = this->_units.end();
		while (iter != iter2)
		{
			if (*iter == tmp && (*iter)->hasActed() == false)
			{
				this->resetMovementMap();
				this->calculateMovement(mapManager, tilePos);
				this->_selectedUnit = iter;
				return (Selected);
			}
			else if (*iter == tmp && (*iter)->hasActed() == true)
				actedUnit = true;
			++iter;
		}
	}
	if (actedUnit)
		return (Acted);
	return (NotInRange);
}

void Player::destroyUnit(IUnit *unit)
{
	std::vector<IUnit *>::iterator iter = this->_units.begin();
	std::vector<IUnit *>::iterator iter2 = this->_units.end();

	while (iter != iter2)
	{
		if ((*iter) == unit)
		{
			delete ((*iter));
			(*iter) = nullptr;
			this->_units.erase(iter);
			this->_selectedUnit = this->_units.end();
			return;
		}
		++iter;
	}
	this->_selectedUnit = this->_units.end();
}

void Player::addUnit(IUnit *unit)
{
	unit->setPlayer(this->_id);
	this->_units.push_back(unit);
	this->_selectedUnit = this->_units.end();
}

void Player::removeBuilding(IBuilding *building)
{
	std::vector<IBuilding *>::iterator iter = this->_buildings.begin();
	std::vector<IBuilding *>::iterator iter2 = this->_buildings.end();

	while (iter != iter2)
	{
		if ((*iter) == building)
		{
			(*iter) = nullptr;
			this->_buildings.erase(iter);
			return;
		}
		++iter;
	}
	this->checkBuildings();
}

void Player::addBuilding(IBuilding *building)
{
	building->setPlayer(this->_id);
	this->_buildings.push_back(building);
	this->checkBuildings();
}

const std::vector<IUnit *> &Player::getUnits() const
{
	return (this->_units);
}

const sf::Vector2u &Player::getAimedTile() const
{
	return (this->_aimedTile);
}

const sf::Uint8 &Player::getId() const
{
	return (this->_id);
}

void Player::setMapSize(const sf::Vector2u &mapSize)
{
	this->_mapSize = mapSize;
	this->resetMovementMap();
}

void Player::drawMovement(sf::RenderWindow &window)
{
	if (this->_selectedUnit != this->_units.end() && !(*this->_selectedUnit)->hasActed())
	{
		const sf::Uint8 move = (*this->_selectedUnit)->getMovement();
		sf::Vector2u tmp;
		sf::RectangleShape rect(sf::Vector2f(16, 16));
		rect.setFillColor(sf::Color(0, 0, 255, 127));
#ifdef _DEBUG
		sf::Font font;
		font.loadFromFile("uni0553-webfont.ttf");
		const_cast<sf::Texture &>(font.getTexture(8)).setSmooth(false);
		sf::Text text("", font, 8);
		text.setColor(sf::Color::Black);
#endif
		while (tmp.y < this->_mapSize.y)
		{
			tmp.x = 0;
			while (tmp.x < this->_mapSize.x)
			{
				if (this->_movement.at(tmp.x).at(tmp.y) >= 0 && this->_movement.at(tmp.x).at(tmp.y) <= move)
				{
					rect.setPosition(sf::Vector2f(static_cast<float>(16 * tmp.x), static_cast<float>(16 * tmp.y)));
					window.draw(rect);
				}
#ifdef _DEBUG
				text.setString(std::to_string(this->_movement.at(tmp.x).at(tmp.y)));
				text.setPosition(sf::Vector2f(static_cast<float>(16 * tmp.x), static_cast<float>(16 * tmp.y)));
				window.draw(text);
#endif
				++tmp.x;
			}
			++tmp.y;
		}
	}
}

const sf::Uint32 &Player::currentBalance() const
{
	return (this->_money);
}

bool Player::buy(const sf::Uint32 &cost)
{
	if (this->_money >= cost)
	{
		this->_money -= cost;
		return (true);
	}
	return (false);
}

std::vector<sf::Vector2u> Player::findPath()
{
	sf::Vector2u unitPos = (*this->_selectedUnit)->getTilePosition();
	sf::Vector2u tilePos = this->_aimedTile;
	sf::Vector2u tmp;
	sf::Int32 cost;
	std::stack<sf::Vector2u> revPath;
	std::vector<sf::Vector2u> path;

	while (tilePos != unitPos)
	{
		if (tilePos.x > 0)
		{
			tmp = sf::Vector2u(tilePos.x - 1, tilePos.y);
			cost = this->_movement.at(tmp.x).at(tmp.y);
			if (cost < this->_movement.at(tilePos.x).at(tilePos.y) && cost >= 0)
			{
				revPath.push(tilePos);
				tilePos = tmp;
				continue;
			}
		}
		if (tilePos.y > 0)
		{
			tmp = sf::Vector2u(tilePos.x, tilePos.y - 1);
			cost = this->_movement.at(tmp.x).at(tmp.y);
			if (cost < this->_movement.at(tilePos.x).at(tilePos.y) && cost >= 0)
			{
				revPath.push(tilePos);
				tilePos = tmp;
				continue;
			}
		}
		if (tilePos.x + 1 < this->_mapSize.x)
		{
			tmp = sf::Vector2u(tilePos.x + 1, tilePos.y);
			cost = this->_movement.at(tmp.x).at(tmp.y);
			if (cost < this->_movement.at(tilePos.x).at(tilePos.y) && cost >= 0)
			{
				revPath.push(tilePos);
				tilePos = tmp;
				continue;
			}
		}
		if (tilePos.y + 1 < this->_mapSize.y)
		{
			tmp = sf::Vector2u(tilePos.x, tilePos.y + 1);
			cost = this->_movement.at(tmp.x).at(tmp.y);
			if (cost < this->_movement.at(tilePos.x).at(tilePos.y) && cost >= 0)
			{
				revPath.push(tilePos);
				tilePos = tmp;
				continue;
			}
		}
	}
	revPath.push(unitPos);
	while (!revPath.empty())
	{
		path.push_back(revPath.top());
		std::cout << "x: " << path.back().x << " y: " << path.back().y << std::endl;
		revPath.pop();
	}
	return (path);
}

void Player::resetMovementMap()
{
	this->_movement.clear();
	this->_movement = std::vector<std::vector<sf::Int32>>(this->_mapSize.x);
	std::vector<std::vector<sf::Int32>>::iterator iter = this->_movement.begin();
	std::vector<std::vector<sf::Int32>>::iterator iter2 = this->_movement.end();
	while (iter != iter2)
	{
		(*iter) = std::vector<sf::Int32>(this->_mapSize.y, -1);
		++iter;
	}
}

void Player::calculateMovement(MapManager &mapManager, const sf::Vector2i &tilePos, sf::Uint8 movement)
{
	std::vector<sf::Vector2i> q;
	q.push_back(sf::Vector2i(tilePos.x, tilePos.y));
	sf::Vector2i tmp = tilePos;
	this->_movement.at(tmp.x).at(tmp.y) = 0;

	while (!q.empty())
	{
		tmp = q.front();
		q.erase(q.begin());
		if (tmp.x - 1 >= 0 && this->_movement.at(tmp.x - 1).at(tmp.y) == -1)
			this->checkTile(mapManager, q, tmp, sf::Vector2i(tmp.x - 1, tmp.y));
		if (tmp.y - 1 >= 0 && this->_movement.at(tmp.x).at(tmp.y - 1) == -1)
			this->checkTile(mapManager, q, tmp, sf::Vector2i(tmp.x, tmp.y - 1));
		if (tmp.x + 1 < static_cast<sf::Int32>(this->_mapSize.x) && this->_movement.at(tmp.x + 1).at(tmp.y) == -1)
			this->checkTile(mapManager, q, tmp, sf::Vector2i(tmp.x + 1, tmp.y));
		if (tmp.y + 1 < static_cast<sf::Int32>(this->_mapSize.y) && this->_movement.at(tmp.x).at(tmp.y + 1) == -1)
			this->checkTile(mapManager, q, tmp, sf::Vector2i(tmp.x, tmp.y + 1));
	}
}

void Player::checkTile(MapManager &mapManager, std::vector<sf::Vector2i> &q, sf::Vector2i pos, sf::Vector2i pos2)
{
	Tile tile = mapManager.getTile(pos2);
	if (tile != WATER && tile != MOUNTAIN)
	{
		this->_movement.at(pos2.x).at(pos2.y) = this->_movement.at(pos.x).at(pos.y) + 1 + mapManager.getTileMovement(tile);
		q.push_back(pos2);
	}
}

bool Player::checkMovement(const sf::Vector2i &tilePos, sf::Uint8 movement) const
{
	if ((tilePos.x >= 0 && tilePos.y >= 0 && tilePos.x < static_cast<sf::Int32>(this->_mapSize.x) && tilePos.y < static_cast<sf::Int32>(this->_mapSize.y)) && this->_movement.at(tilePos.x).at(tilePos.y) >= 0 && this->_movement.at(tilePos.x).at(tilePos.y) <= movement)
		return (true);
	return (false);
}

void Player::checkBuildings()
{
	this->_towers = this->_buildings.size();
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}
