#include "Unit.h"

Unit::Unit()
	: _playerId(0), _position(0, 0), _acted(false), _graphics(nullptr), _stats(nullptr)
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

sf::Uint32 Unit::getHeight() const
{
	if (this->_graphics != nullptr)
		return (this->_graphics->getHeight());
	return (0);
}

const sf::Uint8 &Unit::getMovement() const
{
	return (this->_stats->getMovement());
}

void Unit::setTilePosition(const sf::Vector2u &position)
{
	this->_position = position;
}

sf::Vector2u Unit::getTilePosition() const
{
	return (this->_position);
}

void Unit::setSpritePosition(const sf::Vector2f &position)
{
	this->_graphics->setSpritePosition(position);
}

void Unit::setPlayer(sf::Uint8 id)
{
	this->_playerId = id;
	if (this->_playerId == 1)
		this->_graphics->setColor(sf::Color::Red);
	if (this->_playerId == 2)
		this->_graphics->setColor(sf::Color::Blue);
	if (this->_playerId == 3)
		this->_graphics->setColor(sf::Color::Green);
	if (this->_playerId == 4)
		this->_graphics->setColor(sf::Color::Yellow);
}

const sf::Uint8 &Unit::getPlayerId() const
{
	return (this->_playerId);
}

void Unit::acted()
{
	this->_acted = true;
	this->_graphics->grayOut(true);
}

bool Unit::hasActed() const
{
	return (this->_acted);
}

void Unit::update(const sf::Time &elapsedTime)
{
}

void Unit::resetState()
{
	this->_acted = false;
	this->_graphics->grayOut(false);
}

void Unit::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (this->_graphics != nullptr)
	{
		states.transform *= this->getTransform();
		target.draw(*this->_graphics, states);
	}
}
