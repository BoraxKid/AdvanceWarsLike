#include "Unit.h"

Unit::Unit(sf::Vector2u position)
	: _graphics(nullptr)
{
}

Unit::~Unit()
{
	if (this->_graphics != nullptr)
		delete (this->_graphics);
}

void Unit::setGraphicsComponent(GraphicsComponent *graphics)
{
	this->_graphics = graphics;
}

void Unit::update(const sf::Time &elapsedTime)
{
}

void Unit::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (this->_graphics != nullptr)
		this->_graphics->draw(target, states);
}
