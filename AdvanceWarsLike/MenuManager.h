#ifndef MENUMANAGER_H_
#define MENUMANAGER_H_
#pragma once

#include "Menu.h"

class Player;
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
	void openUnitActionMenu(Player *player, sf::Vector2f pos, sf::Vector2f maxSize);
	void openStartMenu(sf::Vector2f pos, sf::Vector2f maxSize);
	void openGFactoryMenu(sf::Vector2f pos, sf::Vector2f maxSize);
	void draw(sf::RenderWindow &window) const;

private:

	const sf::Font &_font;
	std::map<sf::String, Menu> _menus;
	sf::String _displayedMenu;
};

#endif // MENUMANAGER_H_
