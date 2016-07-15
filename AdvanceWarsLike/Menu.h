#ifndef MENU_H_
#define MENU_H_
#pragma once

#include <vector>
#include "Buttons.h"

class Menu : public sf::Drawable, public sf::Transformable
{
public:
	Menu();
	virtual ~Menu();

	void addButton(const sf::String &id, GenericButton *button);
	void setButtonPointer(const sf::String &id, void *ptr);
	bool contains(const sf::Vector2f &point);
	void click(const sf::Vector2f &point);

private:
	void resizeButtons();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Uint16 _currentHeight;
	sf::Uint16 _buttonWidth;
	std::vector<sf::String> _buttonIds;
	std::vector<sf::String>::iterator _hoveredButton;
	std::map<sf::String, GenericButton *> _buttons;
	//std::map<sf::String, GenericButton *>::iterator _hoveredButton;
};

#endif // MENU_H_
