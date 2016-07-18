#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_
#pragma once

#include <iostream>

#include "IComponent.h"
#include "AnimatedSprite.h"

class GraphicsComponent : public IComponent, public sf::Drawable, public sf::Transformable
{
public:
	GraphicsComponent(AnimatedSprite &sprite)
		: _sprite(sprite), _color(sf::Color::White), _gray(false)
	{
		if (!this->_shader.loadFromFile("shader.glsl", sf::Shader::Fragment))
			std::cerr << "Can't load shader" << std::endl;
		this->_shader.setParameter(std::string("texture"), sf::Shader::CurrentTexture);
		this->_shader.setParameter(std::string("color"), this->_color);
	}

	virtual ~GraphicsComponent()
	{}

	sf::Uint32 getHeight() const
	{
		return (this->_sprite.getHeight());
	}

	void setColor(sf::Color color)
	{
		this->_color = color;
		this->_color * color;
		this->_shader.setParameter(std::string("color"), this->_color);
	}

	void grayOut(bool gray)
	{
		if (!gray)
			this->_shader.setParameter(std::string("color"), this->_color);
		else
			this->_shader.setParameter(std::string("color"), sf::Color(80, 80, 80));
		this->_gray = gray;
	}

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.shader = &this->_shader;
		states.transform *= this->getTransform();
		target.draw(this->_sprite, states);
	}

	AnimatedSprite &_sprite;
	sf::Shader _shader;
	sf::Color _color;
	bool _gray;
};

class BuildingGraphicsComponent : public GraphicsComponent
{
public:
	BuildingGraphicsComponent(AnimatedSprite &sprite)
		: GraphicsComponent(sprite)
	{}

	virtual ~BuildingGraphicsComponent()
	{}

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= this->getTransform();
		target.draw(this->_sprite, states);
	}
};

#endif // GRAPHICSCOMPONENT_H_
