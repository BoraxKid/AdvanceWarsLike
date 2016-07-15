#ifndef MENUMANAGER_H_
#define MENUMANAGER_H_
#pragma once

#include "Menu.h"

class GameState;

class MenuManager
{
public:
	MenuManager(const sf::Font &font);
	virtual ~MenuManager();

	void createMenus(GameState *gameState);
	bool mouseMoved(const sf::Vector2f &mousePosition);
	void click(const sf::Vector2f &mousePosition);
	void reset();
	void openStartMenu(sf::Vector2f pos);
	void draw(sf::RenderWindow &window) const;

private:

	const sf::Font &_font;
	std::map<sf::String, Menu> _menus;
	sf::String _displayedMenu;
};

#endif // MENUMANAGER_H_
