#include "Menu.h"

Menu::Menu()
	: _currentHeight(0), _buttonWidth(0)
{
}

Menu::~Menu()
{
}

void Menu::addButton(const sf::Font &font, const sf::String &text)
{
	this->_buttons.push_back(Button(font, text));
	this->_buttons.back().move(sf::Vector2f(0.0f, this->_currentHeight));
	sf::Uint16 tmp = this->_buttons.back().getWidth();
	if (this->_buttonWidth == 0)
		this->_buttonWidth = tmp;
	else if (tmp > this->_buttonWidth)
	{
		this->_buttonWidth = tmp;
		this->resizeButtons();
	}
	this->_currentHeight += this->_buttons.back().getHeight();
	this->_hoveredButton = this->_buttons.end();
}

bool Menu::contains(sf::Vector2f point)
{
	sf::Vector2f pos = this->getPosition();

	if (point.x >= pos.x && point.x < pos.x + this->_buttonWidth &&
		point.y >= pos.y && point.y < pos.y + this->_currentHeight)
	{
		if (this->_hoveredButton != this->_buttons.end())
			this->_hoveredButton->setHovered(false);
		pos.y = (point.y - pos.y) / this->_buttons.back().getHeight();
		this->_buttons.at(static_cast<sf::Uint16>(pos.y)).setHovered(true);
		this->_hoveredButton = this->_buttons.begin() + static_cast<sf::Uint16>(pos.y);
		return (true);
	}
	if (this->_hoveredButton != this->_buttons.end())
	{
		this->_hoveredButton->setHovered(false);
		this->_hoveredButton = this->_buttons.end();
	}
	return (false);
}

void Menu::click(sf::Vector2f point)
{
	if (this->_hoveredButton != this->_buttons.end())
		this->_hoveredButton->activate();
}

void Menu::resizeButtons()
{
	std::vector<Button>::iterator iter = this->_buttons.begin();
	std::vector<Button>::iterator iter2 = this->_buttons.end();

	while (iter != iter2)
	{
		iter->setWidth(this->_buttonWidth);
		++iter;
	}
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	std::vector<Button>::const_iterator iter = this->_buttons.begin();
	std::vector<Button>::const_iterator iter2 = this->_buttons.end();

	states.transform *= this->getTransform();
	while (iter != iter2)
	{
		target.draw(*iter, states);
		++iter;
	}
}
