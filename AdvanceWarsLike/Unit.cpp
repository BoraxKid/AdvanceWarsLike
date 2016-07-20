#include "Unit.h"

Unit::Unit()
	: _playerId(0), _position(0, 0), _acted(false), _currentGraphics(""), _stats(nullptr)
{
}

Unit::~Unit()
{
	std::map<sf::String, GraphicsComponent *>::iterator iter = this->_graphics.begin();
	std::map<sf::String, GraphicsComponent *>::iterator iter2 = this->_graphics.end();

	while (iter != iter2)
	{
		delete (iter->second);
		++iter;
	}
	if (this->_stats != nullptr)
		delete (this->_stats);
}

void Unit::setGraphicsComponent(const sf::String &sprite, GraphicsComponent *graphics)
{
	if (this->_graphics.find(sprite) != this->_graphics.end())
	{
		delete (this->_graphics.at(sprite));
		this->_graphics.erase(sprite);
	}
	this->_graphics[sprite] = graphics;
	this->_currentGraphics = sprite;
}

void Unit::setStatisticsComponent(StatisticsComponent *stats)
{
	if (this->_stats != nullptr)
		delete (this->_stats);
	this->_stats = stats;
}

void Unit::changeSprite(const sf::String &sprite)
{
	if (this->_graphics.find(sprite) != this->_graphics.end())
	{
		this->_currentGraphics = sprite;
		this->_graphics.at(this->_currentGraphics)->setColor(this->_color);
	}
}

sf::Uint32 Unit::getHeight() const
{
	if (this->_currentGraphics != "")
		return (this->_graphics.at(this->_currentGraphics)->getHeight());
	return (0);
}

const sf::Uint8 &Unit::getMovement() const
{
	return (this->_stats->getMovement());
}

const sf::Uint8 &Unit::getDamage() const
{
	return (this->_stats->getDamage());
}

const sf::Uint8 &Unit::getLife() const
{
	return (this->_stats->getLife());
}

bool Unit::hit(const sf::Uint8 &damage) const
{
	return (this->_stats->hit(damage));
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
	if (this->_currentGraphics != "")
		this->_graphics.at(this->_currentGraphics)->setSpritePosition(position);
}

void Unit::setPlayer(sf::Uint8 id)
{
	this->_playerId = id;
	if (this->_playerId == 1)
		this->_color = sf::Color(192, 64, 64);
	if (this->_playerId == 2)
		this->_color = sf::Color(65, 65, 255);
	if (this->_playerId == 1)
		this->_color = sf::Color::Green;
	if (this->_playerId == 2)
		this->_color = sf::Color(219, 169, 0);
	if (this->_currentGraphics != "")
		this->_graphics.at(this->_currentGraphics)->setColor(this->_color);
}

const sf::Uint8 &Unit::getPlayerId() const
{
	return (this->_playerId);
}

void Unit::acted()
{
	this->_acted = true;
	this->changeSprite("afk");
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
	this->changeSprite("stand");
}

void Unit::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (this->_currentGraphics != "")
	{
		states.transform *= this->getTransform();
		target.draw(*this->_graphics.at(this->_currentGraphics), states);
	}
}
