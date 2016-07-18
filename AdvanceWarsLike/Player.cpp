#include "Player.h"

Player::Player(sf::Uint8 id, MapManager &mapManager)
	: _id(id), _mapManager(mapManager), _selectedUnit(_units.end())
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
}

void Player::moveUnit()
{
	this->_mapManager.move((*this->_selectedUnit)->getTilePosition(), this->_aimedTile);
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

Player::Click Player::click(const sf::Vector2i &tilePos)
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
						this->calculateMovement(tilePos);
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
				this->calculateMovement(tilePos);
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
}

void Player::addBuilding(IBuilding *building)
{
	building->setPlayer(this->_id);
	this->_buildings.push_back(building);
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
		while (tmp.y < this->_mapSize.y)
		{
			tmp.x = 0;
			while (tmp.x < this->_mapSize.x)
			{
				if (this->_movement.at(tmp.x).at(tmp.y) <= move)
				{
					rect.setPosition(sf::Vector2f(static_cast<float>(16 * tmp.x), static_cast<float>(16 * tmp.y)));
					window.draw(rect);
				}
				++tmp.x;
			}
			++tmp.y;
		}
	}
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

void Player::calculateMovement(const sf::Vector2i & tilePos, sf::Uint8 movement)
{
	std::vector<sf::Vector2i> q;
	q.push_back(sf::Vector2i(tilePos.x, tilePos.y));
	sf::Vector2i tmp = tilePos;
	sf::Vector2i tmp2;
	this->_movement.at(tmp.x).at(tmp.y) = 0;

	while (!q.empty())
	{
		tmp = q.front();
		q.erase(q.begin());
		if (tmp.x - 1 >= 0)
			if (this->_movement.at(tmp.x - 1).at(tmp.y) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x - 1, tmp.y);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
		if (tmp.y - 1 >= 0)
			if (this->_movement.at(tmp.x).at(tmp.y - 1) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x, tmp.y - 1);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
		if (tmp.x + 1 < static_cast<sf::Int32>(this->_mapSize.x))
			if (this->_movement.at(tmp.x + 1).at(tmp.y) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x + 1, tmp.y);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
		if (tmp.y + 1 < static_cast<sf::Int32>(this->_mapSize.y))
			if (this->_movement.at(tmp.x).at(tmp.y + 1) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x, tmp.y + 1);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
	}
}

bool Player::checkMovement(const sf::Vector2i & tilePos, sf::Uint8 movement) const
{
	if ((tilePos.x >= 0 && tilePos.y >= 0 && tilePos.x < static_cast<sf::Int32>(this->_mapSize.x) && tilePos.y < static_cast<sf::Int32>(this->_mapSize.y)) && this->_movement.at(tilePos.x).at(tilePos.y) <= movement)
		return (true);
	return (false);
}
