#ifndef BUILDING_H_
#define BUILDING_H_
#pragma once

#include "IBuilding.h"

class Building : public IBuilding
{
public:
	Building(sf::String type);
	virtual ~Building();

	virtual void setGraphicsComponent(GraphicsComponent *graphics);
	virtual const sf::String &getType() const;
	virtual sf::Uint32 getHeight() const;
	virtual void setPlayer(sf::Uint8 id);
	virtual const sf::Uint8 &getPlayerId() const;

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::String _type;
	sf::Uint8 _playerId;
	GraphicsComponent *_graphics;
};

#endif // BUILDING_H_
