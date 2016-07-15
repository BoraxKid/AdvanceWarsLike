#ifndef BUTTONS_H_
#define BUTTONS_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

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
	void setWidth(sf::Uint16 width);
	void setHeight(sf::Uint16 height);
	sf::Uint16 getHeight() const;
	sf::Uint16 getWidth() const;

protected:
	void init();

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
	
	Button(T *obj, void (T::*ptr)(), const sf::Font &font, const sf::String &text, sf::Uint16 width = 100, sf::Uint16 height = 16)
		: GenericButton(font, text, width, height), _obj(obj), _ptr(ptr)
	{}

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

private:
	T *_obj;
	void (T::*_ptr)();
};

#endif // BUTTONS_H_
