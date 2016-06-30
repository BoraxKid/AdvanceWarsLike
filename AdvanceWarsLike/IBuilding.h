#ifndef IBUILDING_H_
#define IBUILDING_H_
#pragma once

class IBuilding
{
public:
	virtual ~IBuilding() {}
	virtual void update() = 0;
};

#endif // IBUILDING_H_
