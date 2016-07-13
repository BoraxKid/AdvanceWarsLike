#include "Unit.h"

Unit::Unit()
	: _position(0, 0), _graphics(nullptr), _stats(nullptr)
{
}

Unit::~Unit()
{
	if (this->_graphics != nullptr)
		delete (this->_graphics);
	if (this->_stats != nullptr)
		delete (this->_stats);
}

void Unit::setGraphicsComponent(GraphicsComponent *graphics)
{
	if (this->_graphics != nullptr)
		delete (this->_graphics);
	this->_graphics = graphics;
}

void Unit::setStatisticsComponent(StatisticsComponent *stats)
{
	if (this->_stats != nullptr)
		delete (this->_stats);
	this->_stats = stats;
}

void Unit::update(const sf::Time &elapsedTime)
{
}

void Unit::move(sf::Vector2u position)
{
	this->_position = position;
}

sf::Vector2u Unit::getTilePosition() const
{
	return (this->_position);
}

void Unit::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (this->_graphics != nullptr)
	{
		states.transform *= this->getTransform();
		this->_graphics->draw(target, states);
	}
}
