#ifndef STROKETEXT_H_
#define STROKETEXT_H_
#pragma once

#include <SFML/Graphics.hpp>

class StrokeText : public sf::Drawable, public sf::Transformable
{
public:
	StrokeText(const sf::String &str, const sf::Font &font, const sf::Uint32 &size)
		: _str(str), _font(font), _size(size), _outlineThickness(1)
	{
		this->_texts[0].setFont(this->_font);
		this->_texts[1].setFont(this->_font);
		this->_texts[2].setFont(this->_font);
		this->_texts[3].setFont(this->_font);
		this->_texts[4].setFont(this->_font);
		this->setOutlineThickness(1);
		this->init();
	}

	virtual ~StrokeText()
	{}

	void init()
	{
		this->_texts[0].setString(this->_str);
		this->_texts[0].setColor(this->_color);
		this->_texts[0].setCharacterSize(this->_size);

		this->_texts[1].setString(this->_str);
		this->_texts[1].setColor(this->_outlineColor);
		this->_texts[1].setCharacterSize(this->_size);

		this->_texts[2].setString(this->_str);
		this->_texts[2].setColor(this->_outlineColor);
		this->_texts[2].setCharacterSize(this->_size);

		this->_texts[3].setString(this->_str);
		this->_texts[3].setColor(this->_outlineColor);
		this->_texts[3].setCharacterSize(this->_size);

		this->_texts[4].setString(this->_str);
		this->_texts[4].setColor(this->_outlineColor);
		this->_texts[4].setCharacterSize(this->_size);
		this->setPosition(0, 0);
	}

	const sf::Vector2f &getTextPosition() const
	{
		return (this->_position);
	}

	void setTextPosition(sf::Vector2f position)
	{
		this->_position = position;
		this->setPosition(this->_position);
		//this->init();
	}

	void setTextPosition(float x, float y)
	{
		this->setTextPosition(sf::Vector2f(x, y));
	}

	void setColor(sf::Color color)
	{
		this->_color = color;
		this->init();
	}

	void setOutlineThickness(sf::Uint8 thickness)
	{
		this->_outlineThickness = thickness;
		float tmp = static_cast<float>(this->_outlineThickness);
		this->_texts[0].setPosition(sf::Vector2f(0.0f, 0.0f));
		this->_texts[1].setPosition(sf::Vector2f(tmp, 0.0f));
		this->_texts[2].setPosition(sf::Vector2f(-tmp, 0.0f));
		this->_texts[3].setPosition(sf::Vector2f(0.0f, tmp));
		this->_texts[4].setPosition(sf::Vector2f(0.0f, -tmp));
	}

	void setOutlineColor(sf::Color color)
	{
		this->_outlineColor = color;
		this->init();
	}

	void setString(sf::String str)
	{
		this->_str = str;
		this->init();
	}

	sf::FloatRect getTextLocalBounds() const
	{
		return (this->_texts[0].getLocalBounds());
	}

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= this->getTransform();
		target.draw(this->_texts[1], states);
		target.draw(this->_texts[2], states);
		target.draw(this->_texts[3], states);
		target.draw(this->_texts[4], states);
		target.draw(this->_texts[0], states);
	}

	sf::String _str;
	const sf::Font &_font;
	sf::Uint32 _size;
	sf::Text _texts[5];
	sf::Vector2f _position;
	sf::Color _color;
	sf::Uint8 _outlineThickness;
	sf::Color _outlineColor;
};

#endif // STROKETEXT_H_