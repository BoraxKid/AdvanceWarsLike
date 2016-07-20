#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_
#pragma once

#include <iostream>

#include "IComponent.h"
#include "AnimatedSprite.h"
#include "StrokeText.h"

class GraphicsComponent : public IComponent, public sf::Drawable, public sf::Transformable
{
public:
	GraphicsComponent(AnimatedSprite &sprite)
		: _sprite(sprite), _color(sf::Color::White), _gray(false)
	{
		if (!this->_shader.loadFromFile("resources/shader.glsl", sf::Shader::Fragment))
			std::cerr << "Can't load shader.glsl" << std::endl;
		this->_shader.setParameter(std::string("texture"), sf::Shader::CurrentTexture);
		this->_shader.setParameter(std::string("color"), this->_color);
	}

	virtual ~GraphicsComponent()
	{}

	void setSpritePosition(const sf::Vector2f &position)
	{
		this->_sprite.setPosition(position);
	}

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

class Player;

class PlayerGraphicsComponent : public sf::Drawable, public sf::Transformable
{
public:
	PlayerGraphicsComponent(Player *player, const sf::Font &font, sf::String name)
		: _player(player), _font(font), _text("", _font, 8)
	{
		this->_text.setColor(sf::Color::Black);
		this->_text.setOutlineColor(sf::Color::White);
		this->setPosition(sf::Vector2f(4.0f, 0.0f));
		if (name == "red")
			this->_text.setColor(sf::Color(192, 64, 64));
		if (name == "blue")
			this->_text.setColor(sf::Color(65, 65, 255));
		if (name == "green")
			this->_text.setColor(sf::Color::Green);
		if (name == "yellow")
			this->_text.setColor(sf::Color(219, 169, 0));
		std::string tmp = name.toAnsiString();
		if (!tmp.empty())
		{
			tmp[0] = ::toupper(tmp[0]);

			for (std::size_t i = 1; i < tmp.length(); ++i)
				tmp[i] = ::tolower(tmp[i]);
		}
		this->_name = tmp;
	}

	virtual ~PlayerGraphicsComponent()
	{}

	void setBalance(const sf::Uint32 &balance)
	{
		this->_text.setString(this->_name + "'s balance: $" + std::to_string(balance));
	}

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= this->getTransform();
		target.draw(this->_text, states);
	}

	sf::RenderTexture _texture;
	Player *_player;
	const sf::Font &_font;
	sf::String _name;
	StrokeText _text;
};

#endif // GRAPHICSCOMPONENT_H_
