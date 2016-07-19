#include "MenuManager.h"
#include "Player.h"
#include "GameState.h"

MenuManager::MenuManager(const sf::Font &font)
	: _font(font), _displayedMenu("")
{
}

MenuManager::~MenuManager()
{
}

void MenuManager::createMenus(GameState *gameState, ResourcesManager &resourcesManager)
{
	this->_menus["unit_action"] = Menu();
	this->_menus.at("unit_action").addButton("move", new Button<GameState>(gameState, &GameState::movePlayerUnit, this->_font, "Move"));
	this->_menus.at("unit_action").addButton("attack", new Button<GameState>(gameState, &GameState::findTargets, this->_font, "Attack"));
	this->_menus["player_start"] = Menu();
	this->_menus.at("player_start").addButton("end_turn", new Button<GameState>(gameState, &GameState::changeTurn, this->_font, "End turn"));
	this->_menus["enemy_building"] = Menu();
	this->_menus.at("enemy_building").addButton("capture", new GenericButton(this->_font, "Capture"));
	this->_menus["gfactory"] = Menu();
	this->_menus.at("gfactory").addButton("tank", new BuyButton<GameState>(resourcesManager.at("unit_tank"), gameState, &GameState::buyUnit, 1000, this->_font, "Create tank ($1000)"));
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

void MenuManager::openUnitActionMenu(Player *player, sf::Vector2f pos, sf::Vector2f maxSize)
{
	if (this->_displayedMenu == "")
	{
		this->_displayedMenu = "unit_action";
		this->setupMenu(pos, maxSize);
	}
}

void MenuManager::openStartMenu(sf::Vector2f pos, sf::Vector2f maxSize)
{
	if (this->_displayedMenu == "")
	{
		this->_displayedMenu = "player_start";
		this->setupMenu(pos, maxSize);
	}
}

void MenuManager::openGFactoryMenu(sf::Vector2f pos, sf::Vector2f maxSize)
{
	if (this->_displayedMenu == "")
	{
		this->_displayedMenu = "gfactory";
		this->setupMenu(pos, maxSize);
	}
}

void MenuManager::draw(sf::RenderWindow &window) const
{
	if (this->_displayedMenu != "")
		window.draw(this->_menus.at(this->_displayedMenu));
}

void MenuManager::setupMenu(sf::Vector2f pos, sf::Vector2f maxSize)
{
	sf::Vector2<sf::Uint16> size = this->_menus.at(this->_displayedMenu).getSize();
	if (pos.x + size.x > maxSize.x)
		pos.x -= size.x;
	if (pos.y + size.y > maxSize.y)
		pos.y -= size.y;
	this->_menus.at(this->_displayedMenu).setPosition(pos);
}
