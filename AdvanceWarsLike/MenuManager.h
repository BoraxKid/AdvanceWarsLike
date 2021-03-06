#ifndef MENUMANAGER_H_
#define MENUMANAGER_H_
#pragma once

#include "Menu.h"
#include "ResourcesManager.h"

class Player;
class GameState;
class MainMenuState;

class MenuManager
{
public:
	MenuManager(const sf::Font &font, const sf::Vector2f &size);
	virtual ~MenuManager();

	void createMenusMainMenuState(MainMenuState *menuState, ResourcesManager &resourcesManager);
	void createMenusGameState(GameState *gameState, ResourcesManager &resourcesManager);
	bool mouseMoved(const sf::Vector2f &mousePosition);
	void click(const sf::Vector2f &mousePosition);
	void reset();
	void openMainMenu();
	void openStartConfigPlayer();
	void openUnitActionMenu(Player *player, sf::Vector2f pos, sf::Vector2f maxSize);
	void openStartMenu(sf::Vector2f pos, sf::Vector2f maxSize);
	void openGFactoryMenu(sf::Vector2f pos, sf::Vector2f maxSize);
	void draw(sf::RenderWindow &window) const;

private:
	void setupMenu(sf::Vector2f pos, sf::Vector2f maxSize);

	const sf::Font &_font;
	std::map<sf::String, Menu *> _menus;
	sf::String _displayedMenu;
	const sf::Vector2f _size;
};

#endif // MENUMANAGER_H_
