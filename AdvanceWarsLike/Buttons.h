#ifndef BUTTONS_H_
#define BUTTONS_H_
#pragma once

#include <SFML/Graphics.hpp>

#include "AnimatedSprite.h"

class GenericButton : public sf::Drawable, public sf::Transformable
{
public:
	GenericButton();
	GenericButton(const sf::Font &font, const sf::String &text, sf::Uint16 width = 100, sf::Uint16 height = 16);
	virtual ~GenericButton();

	virtual void activate();
	virtual void setPointer(void *ptr);

	void setHovered(const bool &hovered);
	void setFont(const sf::Font &font);
	void setText(const sf::String &text);
	virtual void setWidth(sf::Uint16 width);
	void setHeight(sf::Uint16 height);
	sf::Uint16 getHeight() const;
	sf::Uint16 getWidth() const;

protected:
	virtual void init();

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	bool _hovered;
	sf::Text _text;
	sf::RectangleShape _rect;
	sf::Uint16 _width;
	sf::Uint16 _height;
};

template <typename T>
class Button : public GenericButton
{
public:
	Button()
		: GenericButton()
	{}
	
	Button(T *obj, void (T::*ptr)(), const sf::Font &font, const sf::String &text)
		: GenericButton(font, text, 100, 16), _obj(obj), _ptr(ptr)
	{
		this->init();
	}

	Button(T *obj, void (T::*ptr)(), const sf::Font &font, const sf::String &text, sf::Uint16 width, sf::Uint16 height)
		: GenericButton(font, text, width, height), _obj(obj), _ptr(ptr)
	{
	}

	virtual ~Button()
	{}

	virtual void setPointer(void *ptr)
	{
		this->_obj = reinterpret_cast<T *>(ptr);
	}

	virtual void activate()
	{
		if (this->_obj != nullptr && this->_ptr != nullptr)
			(this->_obj->*_ptr)();
	}

protected:
	T *_obj;
	void (T::*_ptr)();
};

template <typename T>
class SpriteButton : public Button<T>
{
public:
	SpriteButton(AnimatedSprite &sprite, T *obj, void (T::*ptr)(), const sf::Font &font, const sf::String &text, sf::Uint16 width = 100, sf::Uint16 height = 16)
		: Button(obj, ptr, font, text, width, height), _sprite(sprite)
	{
		this->init();
	}

	virtual ~SpriteButton()
	{}

protected:
	virtual void init()
	{
		GenericButton::_text.setCharacterSize(8);
		GenericButton::_text.setColor(sf::Color::White);
		sf::Uint16 tmp = static_cast<sf::Uint16>(GenericButton::_text.getLocalBounds().width) + 18;
		if (tmp > GenericButton::_width)
			GenericButton::_width = tmp + 6 + 18;
		GenericButton::_rect.setSize(sf::Vector2f(GenericButton::_width, GenericButton::_height));
		GenericButton::_rect.setFillColor(sf::Color(102, 102, 102));
		GenericButton::_rect.setOutlineColor(sf::Color(239, 239, 239));
		GenericButton::_rect.setOutlineThickness(1.0f);
		GenericButton::_rect.setPosition(sf::Vector2f(0.0f, 0.0f));
		GenericButton::_text.setPosition(sf::Vector2f(20.0f, GenericButton::_height - GenericButton::_text.getCharacterSize() - 4.0f));
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= this->getTransform();
		target.draw(GenericButton::_rect, states);
		this->_sprite.setPosition(sf::Vector2f(2.0f, static_cast<float>(GenericButton::_height - this->_sprite.getHeight() - 1.0f)));
		target.draw(this->_sprite, states);
		target.draw(GenericButton::_text, states);
	}

	AnimatedSprite &_sprite;
};

template <typename T>
class BuyButton : public SpriteButton<T>
{
public:
	BuyButton(AnimatedSprite &sprite, T *obj, void (T::*ptr)(sf::Uint32), sf::Uint32 cost, const sf::Font &font, const sf::String &text, sf::Uint16 width = 100, sf::Uint16 height = 16)
		: SpriteButton(sprite, obj, nullptr, font, text, width, height), _ptr(ptr), _cost(cost)
	{}

	virtual ~BuyButton()
	{}

	virtual void activate()
	{
		if (this->_obj != nullptr && this->_ptr != nullptr)
			(this->_obj->*_ptr)(this->_cost);
	}

private:
	void (T::*_ptr)(sf::Uint32);
	sf::Uint32 _cost;
};

#endif // BUTTONS_H_
