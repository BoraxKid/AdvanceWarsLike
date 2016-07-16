#ifndef STATISTICSCOMPONENT_H_
#define STATISTICSCOMPONENT_H_
#pragma once

#include "IComponent.h"

class StatisticsComponent : public IComponent
{
public:
	StatisticsComponent(sf::Uint8 move = 5, sf::Uint8 vision = 4)
		: _move(move), _vision(vision)
	{}

	virtual ~StatisticsComponent()
	{}

	const sf::Uint8 &getMovement() const
	{
		return (this->_move);
	}

protected:
	const sf::Uint8 _move;
	const sf::Uint8 _vision;
};

class TankStatisticsComponent : public StatisticsComponent
{
public:
	TankStatisticsComponent()
		: StatisticsComponent(5, 4)
	{}

	virtual ~TankStatisticsComponent()
	{}
};

#endif // STATISTICSCOMPONENT_H_
