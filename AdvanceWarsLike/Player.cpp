#include "Player.h"

Player::Player()
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

void Player::addUnit(IUnit *unit)
{
	this->_units.push_back(unit);
}

const std::vector<IUnit *> &Player::getUnits() const
{
	return (this->_units);
}
