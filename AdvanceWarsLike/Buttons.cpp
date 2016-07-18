#include "Buttons.h"

GenericButton::GenericButton()
	: _hovered(false), _text(sf::Text()), _width(100), _height(16)
{
	this->init();
}

GenericButton::GenericButton(const sf::Font &font, const sf::String &text, sf::Uint16 width, sf::Uint16 height)
	: _hovered(false), _text(text, font), _width(width), _height(16)
{
	this->init();
}

GenericButton::~GenericButton()
{
}

void GenericButton::activate()
{

}

void GenericButton::setPointer(void *ptr)
{
}

void GenericButton::init()
{
	this->_text.setCharacterSize(8);
	this->_text.setColor(sf::Color::White);
	sf::Uint16 tmp = static_cast<sf::Uint16>(this->_text.getLocalBounds().width);
	if (tmp > this->_width)
		this->_width = tmp + 6;
	this->_rect.setSize(sf::Vector2f(this->_width, this->_height));
	this->_rect.setFillColor(sf::Color(102, 102, 102));
	this->_rect.setOutlineColor(sf::Color(239, 239, 239));
	this->_rect.setOutlineThickness(1.0f);
	this->_rect.setPosition(sf::Vector2f(0.0f, 0.0f));
	this->_text.setPosition(sf::Vector2f(2.0f, this->_height - this->_text.getCharacterSize() - 4.0f));
}

void GenericButton::setHovered(const bool &hovered)
{
	this->_hovered = hovered;
	if (this->_hovered)
		this->_rect.setFillColor(sf::Color::Black);
	else
		this->_rect.setFillColor(sf::Color(102, 102, 102));
}

void GenericButton::setFont(const sf::Font &font)
{
	this->_text.setFont(font);
}

void GenericButton::setText(const sf::String &text)
{
	this->_text.setString(text);
}

void GenericButton::setHeight(sf::Uint16 height)
{
	this->_height = height;
	this->_rect.setSize(sf::Vector2f(this->_width, this->_height));
}

void GenericButton::setWidth(sf::Uint16 width)
{
	this->_width = width;
	this->_rect.setSize(sf::Vector2f(this->_width, this->_height));
}

sf::Uint16 GenericButton::getHeight() const
{
	return (this->_height);
}

sf::Uint16 GenericButton::getWidth() const
{
	return (this->_width);
}

void GenericButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(this->_rect, states);
	target.draw(this->_text, states);
}