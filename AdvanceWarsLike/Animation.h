#ifndef ANIMATION_H_
#define ANIMATION_H_
#pragma once

#include <SFML/Graphics.hpp>

#include "MapManager.h"
#include "Player.h"

class IAnimation : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~IAnimation() {}

	virtual bool update(const sf::Time &elapsedTime) = 0;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

class NewTurnAnimation : public IAnimation
{
public:
	NewTurnAnimation(const sf::Font &font, const sf::String &player, const sf::Uint32 &turns, const sf::Vector2f &size, const sf::Time animationTime = sf::seconds(1.5f));
	virtual ~NewTurnAnimation();

	virtual bool update(const sf::Time &elapsedTime);
	void changeText(const sf::String &player, const sf::Uint32 &turns);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	const sf::Time _animationTime;
	const sf::Vector2f &_size;
	sf::Time _elapsedTime;
	sf::Text _turns;
	sf::Text _player;
};

class MovementAnimation : public IAnimation
{
public:
	MovementAnimation(ResourcesManager &resourcesManager, MapManager &mapManager, const sf::Time animationTime = sf::seconds(0.1f));
	virtual ~MovementAnimation();

	virtual bool update(const sf::Time &elapsedTime);
	void changePath(Player *player);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	ResourcesManager &_resourcesManager;
	MapManager &_mapManager;
	IUnit *_unit;
	Player *_player;
	std::vector<sf::Vector2u> _positions;
	std::vector<sf::Vector2u>::iterator _currentPosition;
	sf::Vector2f _unitPos;
	const sf::Time _animationTime;
	sf::Time _elapsedTime;
};

#endif // ANIMATION_H_
