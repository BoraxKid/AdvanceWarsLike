#include "Building.h"
#include <iostream>

Building::Building(sf::String type)
	: _type(type), _graphics(nullptr)
{
}

Building::~Building()
{
}

void Building::setGraphicsComponent(GraphicsComponent *graphics)
{
	if (this->_graphics != nullptr)
		delete (this->_graphics);
	this->_graphics = graphics;
}

const sf::String &Building::getType() const
{
	return (this->_type);
}

void Building::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (this->_graphics != nullptr)
	{
		states.transform *= this->getTransform();
		target.draw(*this->_graphics, states);
	}
}