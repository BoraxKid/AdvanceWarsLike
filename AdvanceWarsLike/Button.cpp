#include "Button.h"

Button::Button()
	: _hovered(false), _text(sf::Text()), _width(100), _height(16)
{
	this->init();
}

Button::Button(const sf::Font &font, const sf::String &text, sf::Uint16 width, sf::Uint16 height)
	: _hovered(false), _text(text, font), _width(width), _height(16)
{
	this->init();
}

Button::~Button()
{
}

void Button::activate()
{

}

void Button::init()
{
	this->_text.setCharacterSize(12);
	this->_text.setColor(sf::Color::White);
	sf::FloatRect bounds = this->_text.getLocalBounds();
	if (bounds.width > this->_width)
		this->_width = bounds.width + 6;
	this->_rect.setSize(sf::Vector2f(this->_width, this->_height));
	this->_rect.setFillColor(sf::Color(102, 102, 102));
	this->_rect.setOutlineColor(sf::Color(239, 239, 239));
	this->_rect.setOutlineThickness(1.0f);
	this->_rect.setPosition(sf::Vector2f(0.0f, 0.0f));
	this->_text.setPosition(sf::Vector2f(2.0f, 0.0f));
}

void Button::setHovered(const bool &hovered)
{
	this->_hovered = hovered;
	if (this->_hovered)
		this->_rect.setFillColor(sf::Color::Black);
	else
		this->_rect.setFillColor(sf::Color(102, 102, 102));
}

void Button::setFont(const sf::Font &font)
{
	this->_text.setFont(font);
}

void Button::setText(const sf::String &text)
{
	this->_text.setString(text);
}

void Button::setHeight(sf::Uint16 height)
{
	this->_height = height;
	this->_rect.setSize(sf::Vector2f(this->_width, this->_height));
}

void Button::setWidth(sf::Uint16 width)
{
	this->_width = width;
	this->_rect.setSize(sf::Vector2f(this->_width, this->_height));
}

sf::Uint16 Button::getHeight() const
{
	return (this->_height);
}

sf::Uint16 Button::getWidth() const
{
	return (this->_width);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(this->_rect, states);
	target.draw(this->_text, states);
}
