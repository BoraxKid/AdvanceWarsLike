#ifndef PLAYER_H_
#define PLAYER_H_
#pragma once

#include "IUnit.h"

class Player
{
public:
	Player();
	virtual ~Player();

	void addUnit(IUnit *unit);
	const std::vector<IUnit *> &getUnits() const;

private:
	std::vector<IUnit *> _units;
};

#endif // PLAYER_H_
