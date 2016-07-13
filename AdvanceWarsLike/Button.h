#ifndef BUTTON_H_
#define BUTTON_H_
#pragma once

#include <SFML/Graphics.hpp>

class Button : public sf::Drawable, public sf::Transformable
{
public:
	Button();
	Button(const sf::Font &font, const sf::String &text, sf::Uint16 width = 100, sf::Uint16 height = 16);
	virtual ~Button();

	void activate();

	void setHovered(const bool &hovered);
	void setFont(const sf::Font &font);
	void setText(const sf::String &text);
	void setWidth(sf::Uint16 width);
	void setHeight(sf::Uint16 height);
	sf::Uint16 getHeight() const;
	sf::Uint16 getWidth() const;

private:
	void init();

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	bool _hovered;
	sf::Text _text;
	sf::RectangleShape _rect;
	sf::Uint16 _width;
	sf::Uint16 _height;
};

#endif // BUTTON_H_
