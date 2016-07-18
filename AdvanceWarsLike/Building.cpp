#include "Building.h"
#include <iostream>

Building::Building(sf::String type)
	: _type(type), _playerId(255), _graphics(nullptr)
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

sf::Uint32 Building::getHeight() const
{
	if (this->_graphics != nullptr)
		return (this->_graphics->getHeight());
	return (0);
}

void Building::setPlayer(sf::Uint8 id)
{
	this->_playerId = id;
}

const sf::Uint8 &Building::getPlayerId() const
{
	return (this->_playerId);
}

void Building::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (this->_graphics != nullptr)
	{
		states.transform *= this->getTransform();
		target.draw(*this->_graphics, states);
	}
}