#include "MenuManager.h"
#include "GameState.h"

MenuManager::MenuManager(const sf::Font &font)
	: _font(font), _displayedMenu("")
{
}

MenuManager::~MenuManager()
{
}

void MenuManager::createMenus(GameState *gameState)
{
	this->_menus["unit_action"] = Menu();
	this->_menus.at("unit_action").addButton(new Button(this->_font, "Move"));
	this->_menus.at("unit_action").addButton(new Button(this->_font, "Attack"));
	this->_menus["player_start"] = Menu();
	this->_menus.at("player_start").addButton(new GameStateButton(gameState, &GameState::changeTurn, this->_font, "End turn"));
}

bool MenuManager::mouseMoved(const sf::Vector2f &mousePosition)
{
	if (this->_displayedMenu != "")
		return (this->_menus.at(this->_displayedMenu).contains(mousePosition));
	return (false);
}

void MenuManager::click(const sf::Vector2f &mousePosition)
{
	if (this->_displayedMenu != "")
		this->_menus.at(this->_displayedMenu).click(mousePosition);
	this->_displayedMenu = "";
}

void MenuManager::reset()
{
	this->_displayedMenu = "";
}

void MenuManager::openStartMenu(sf::Vector2f pos)
{
	if (this->_displayedMenu == "")
	{
		this->_displayedMenu = "player_start";
		this->_menus.at(this->_displayedMenu).setPosition(pos);
	}
}

void MenuManager::draw(sf::RenderWindow &window) const
{
	if (this->_displayedMenu != "")
		window.draw(this->_menus.at(this->_displayedMenu));
}
