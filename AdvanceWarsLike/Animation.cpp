#include "Animation.h"

NewTurnAnimation::NewTurnAnimation(const sf::Font &font, const sf::String &player, const sf::Uint32 &turns, const sf::Vector2f &size, const sf::Time animationTime)
	: _animationTime(animationTime), _size(size)
{
	this->_turns.setFont(font);
	this->_turns.setCharacterSize(8);
	this->_turns.setColor(sf::Color::Black);
	this->_player.setFont(font);
	this->_player.setCharacterSize(8);
	this->_player.setColor(sf::Color::Black);
	this->changeText(player, turns);
}

NewTurnAnimation::~NewTurnAnimation()
{
}

bool NewTurnAnimation::update(const sf::Time &elapsedTime)
{
	this->_elapsedTime += elapsedTime;
	if (this->_elapsedTime > this->_animationTime)
	{
		this->_elapsedTime = sf::Time();
		return (false);
	}
	if (this->_elapsedTime.asSeconds() < this->_animationTime.asSeconds() / 4.0f)
	{
		float tmp = ((this->_elapsedTime.asSeconds() / (this->_animationTime.asSeconds() / 4.0f)));
		this->_turns.setPosition(sf::Vector2f(this->_size.x / 2.0f * tmp - this->_turns.getLocalBounds().width / 2.0f, this->_size.y / 2.0f));
		this->_player.setPosition(sf::Vector2f(this->_size.x / 2.0f * tmp - this->_player.getLocalBounds().width / 2.0f, this->_size.y / 2.0f + this->_turns.getLocalBounds().height + 4));
	}
	else if (this->_elapsedTime.asSeconds() < this->_animationTime.asSeconds() / 4.0f * 3.0f)
	{
		this->_turns.setPosition(sf::Vector2f(this->_size.x / 2.0f - this->_turns.getLocalBounds().width / 2.0f, this->_size.y / 2.0f));
		this->_player.setPosition(sf::Vector2f(this->_size.x / 2.0f - this->_player.getLocalBounds().width / 2.0f, this->_size.y / 2.0f + this->_turns.getLocalBounds().height + 4));
	}
	else
	{
		float tmp = (this->_elapsedTime.asSeconds() - (this->_animationTime.asSeconds() / 4.0f * 3.0f)) / (this->_animationTime.asSeconds() / 4.0f);
		this->_turns.setPosition(sf::Vector2f(this->_size.x / 2.0f + this->_size.x / 2.0f * tmp - this->_turns.getLocalBounds().width / 2.0f, this->_size.y / 2.0f));
		this->_player.setPosition(sf::Vector2f(this->_size.x / 2.0f + this->_size.x / 2.0f * tmp - this->_player.getLocalBounds().width / 2.0f, this->_size.y / 2.0f + this->_turns.getLocalBounds().height + 4));
	}
	return (true);
}

void NewTurnAnimation::changeText(const sf::String &player, const sf::Uint32 &turns)
{
	this->_turns.setString(std::string("Turn ") + std::to_string(turns));
	std::string tmp = player.toAnsiString();
	if (!tmp.empty())
	{
		tmp[0] = ::toupper(tmp[0]);

		for (std::size_t i = 1; i < tmp.length(); ++i)
			tmp[i] = ::tolower(tmp[i]);
	}
	this->_player.setString(tmp + std::string("'s turn"));
}

void NewTurnAnimation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(this->_turns, states);
	target.draw(this->_player, states);
}

MovementAnimation::MovementAnimation(MapManager &mapManager, const sf::Time animationTime)
	: _mapManager(mapManager), _unit(nullptr), _player(nullptr), _movePtr(nullptr), _animationTime(animationTime)
{
	this->_currentPosition = this->_positions.end();
}

MovementAnimation::~MovementAnimation()
{
}

bool MovementAnimation::update(const sf::Time &elapsedTime)
{
	this->_elapsedTime += elapsedTime;
	if (this->_currentPosition == this->_positions.end() || this->_unit == nullptr)
	{
		this->_elapsedTime = sf::Time();
		if (this->_player != nullptr && this->_movePtr != nullptr)
		{
			(this->_player->*this->_movePtr)();
			this->_mapManager.addUnit(this->_unit, (*(this->_positions.end() - 1)));
		}
		return (false);
	}
	this->_unit->setTilePosition(*this->_currentPosition);
	this->_unitPos = this->_unit->getPosition();
	if (this->_elapsedTime >= this->_animationTime)
	{
		std::cout << "change pos x: " << this->_currentPosition->x << " y: " << this->_currentPosition->y << std::endl;
		++this->_currentPosition;
		this->_elapsedTime -= this->_animationTime;
	}
	return (true);
}

void MovementAnimation::changePath(Player *player, void (Player::*movePtr)())
{
	this->_player = player;
	this->_movePtr = movePtr;
	this->_positions = player->findPath();
	this->_currentPosition = this->_positions.begin();
	this->_unit = this->_mapManager.getUnit(this->_positions.at(0));
	this->_mapManager.removeUnit(this->_positions.at(0));
	std::cout << this->_mapManager.getUnit(this->_positions.at(0)) << std::endl;
}

void MovementAnimation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	if (this->_unit != nullptr)
	{
		//std::cout << "draw" << std::endl;
		this->_unit->setTilePosition(sf::Vector2u(this->_unitPos));
		target.draw(*this->_unit, states);
	}
}