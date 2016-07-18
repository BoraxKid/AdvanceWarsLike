#ifndef IBUILDING_H_
#define IBUILDING_H_
#pragma once

#include <SFML/Graphics.hpp>

#include "GraphicsComponent.h"

class IBuilding : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~IBuilding() {}

	virtual void setGraphicsComponent(GraphicsComponent *graphics) = 0;
	virtual const sf::String &getType() const = 0;
	virtual sf::Uint32 getHeight() const = 0;
	virtual void setPlayer(sf::Uint8 id) = 0;
	virtual const sf::Uint8 &getPlayerId() const = 0;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif // IBUILDING_H_
