#ifndef MENU_H_
#define MENU_H_
#pragma once

#include <vector>
#include "Button.h"

class Menu : public sf::Drawable, public sf::Transformable
{
public:
	Menu();
	virtual ~Menu();

	void addButton(const sf::Font &font, const sf::String &text);
	bool contains(sf::Vector2f point);
	void click(sf::Vector2f point);

private:
	void resizeButtons();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Uint16 _currentHeight;
	sf::Uint16 _buttonWidth;
	std::vector<Button> _buttons;
	std::vector<Button>::iterator _hoveredButton;
};

#endif // MENU_H_
