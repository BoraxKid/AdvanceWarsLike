#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::addUnit(IUnit *unit)
{
	this->_units.push_back(unit);
}

const std::vector<IUnit *> &Player::getUnits() const
{
	return (this->_units);
}
