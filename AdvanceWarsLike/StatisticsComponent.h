#ifndef STATISTICSCOMPONENT_H_
#define STATISTICSCOMPONENT_H_
#pragma once

#include "IComponent.h"

class StatisticsComponent : public IComponent
{
public:
	StatisticsComponent(char move = 5, char vision = 4)
		: _move(move), _vision(vision), _currentMove(_move)
	{}

	virtual ~StatisticsComponent()
	{}

protected:
	const char _move;
	const char _vision;
	char _currentMove;
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
