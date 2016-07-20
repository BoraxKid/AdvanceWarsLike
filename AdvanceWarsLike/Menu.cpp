#include "Menu.h"

Menu::Menu(const sf::Vector2f &size, bool mainMenu)
	: _size(size), _mainMenu(mainMenu), _currentHeight(0), _buttonWidth(100)
{
}

Menu::~Menu()
{
	std::map<sf::String, GenericButton *>::iterator iter = this->_buttons.begin();
	std::map<sf::String, GenericButton *>::iterator iter2 = this->_buttons.end();

	while (iter != iter2)
	{
		delete (iter->second);
		++iter;
	}
	this->_buttons.clear();
}

void Menu::addButton(const sf::String &id, GenericButton *button)
{
	this->_buttonIds.push_back(id);
	this->_buttons[id] = button;
	this->_buttons.at(id)->move(sf::Vector2f(0.0f, this->_currentHeight));
	sf::Uint16 tmp = this->_buttons.at(id)->getWidth();
	if (this->_buttonWidth == 0)
		this->_buttonWidth = tmp;
	else if (tmp > this->_buttonWidth)
	{
		this->_buttonWidth = tmp;
		this->resizeButtons();
	}
	this->_currentHeight += this->_buttons.at(id)->getHeight();
	this->_hoveredButton = this->_buttonIds.end();
}

void Menu::setButtonPointer(const sf::String &id, void *ptr)
{
	if (this->_buttons.find(id) != this->_buttons.end())
		this->_buttons.at(id)->setPointer(ptr);
}

bool Menu::contains(const sf::Vector2f &point)
{
	sf::Vector2f pos = this->getPosition();

	if (point.x >= pos.x && point.x < pos.x + this->_buttonWidth &&
		point.y >= pos.y && point.y < pos.y + this->_currentHeight)
	{
		if (this->_hoveredButton != this->_buttonIds.end())
			this->_buttons.at(*this->_hoveredButton)->setHovered(false);
		pos.y = (point.y - pos.y) / this->_buttons.begin()->second->getHeight();
		if (static_cast<sf::Uint16>(pos.y) > this->_buttonIds.size())
			this->_buttons.at(this->_buttonIds.back())->setHovered(true);
		else
			this->_buttons.at(this->_buttonIds.at(static_cast<sf::Uint16>(pos.y)))->setHovered(true);
		this->_hoveredButton = this->_buttonIds.begin() + static_cast<sf::Uint16>(pos.y);
		return (true);
	}
	if (this->_hoveredButton != this->_buttonIds.end())
	{
		this->_buttons.at(*this->_hoveredButton)->setHovered(false);
		this->_hoveredButton = this->_buttonIds.end();
	}
	return (false);
}

void Menu::click(const sf::Vector2f &point)
{
	if (this->_hoveredButton != this->_buttonIds.end())
		this->_buttons.at(*this->_hoveredButton)->activate();
}

sf::Vector2<sf::Uint16> Menu::getSize() const
{
	return (sf::Vector2<sf::Uint16>(this->_buttonWidth, this->_currentHeight));
}

void Menu::resizeButtons()
{
	std::map<sf::String, GenericButton *>::iterator iter = this->_buttons.begin();
	std::map<sf::String, GenericButton *>::iterator iter2 = this->_buttons.end();

	while (iter != iter2)
	{
		iter->second->setWidth(this->_buttonWidth);
		++iter;
	}
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	std::map<sf::String, GenericButton *>::const_iterator iter = this->_buttons.begin();
	std::map<sf::String, GenericButton *>::const_iterator iter2 = this->_buttons.end();

	states.transform *= this->getTransform();
	while (iter != iter2)
	{
		target.draw(*(iter->second), states);
		++iter;
	}
}
