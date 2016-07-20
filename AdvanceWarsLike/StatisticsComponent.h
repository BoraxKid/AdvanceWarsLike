#ifndef STATISTICSCOMPONENT_H_
#define STATISTICSCOMPONENT_H_
#pragma once

#include "IComponent.h"

class StatisticsComponent : public IComponent
{
public:
	StatisticsComponent(sf::Uint8 move = 5, sf::Uint8 vision = 4, sf::Uint8 damage = 1)
		: _move(move), _vision(vision), _damage(damage), _life(10), _currentDamage(_damage * _life)
	{}

	virtual ~StatisticsComponent()
	{}

	const sf::Uint8 &getMovement() const
	{
		return (this->_move);
	}

	const sf::Uint8 &getDamage() const
	{
		return (this->_currentDamage);
	}

	const sf::Uint8 &getLife() const
	{
		return (this->_life);
	}

	bool hit(const sf::Uint8 &damage)
	{
		if (this->_life > damage)
		{
			this->_life -= damage;
			this->_currentDamage = this->_life * this->_damage;
			return (false);
		}
		this->_life = 0;
		return (true);
	}

protected:
	const sf::Uint8 _move;
	const sf::Uint8 _vision;
	const sf::Uint8 _damage;
	sf::Uint8 _life;
	sf::Uint8 _currentDamage;
};

class TankStatisticsComponent : public StatisticsComponent
{
public:
	TankStatisticsComponent()
		: StatisticsComponent(10, 4, 1)
	{}

	virtual ~TankStatisticsComponent()
	{}
};

#endif // STATISTICSCOMPONENT_H_
