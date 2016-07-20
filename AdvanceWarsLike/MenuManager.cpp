#include "MenuManager.h"
#include "Player.h"
#include "GameState.h"
#include "MainMenuState.h"

MenuManager::MenuManager(const sf::Font &font, const sf::Vector2f &size)
	: _font(font), _displayedMenu(""), _size(size)
{
}

MenuManager::~MenuManager()
{
	std::map<sf::String, Menu *>::iterator iter = this->_menus.begin();
	std::map<sf::String, Menu *>::iterator iter2 = this->_menus.end();

	while (iter != iter2)
	{
		delete (iter->second);
		++iter;
	}
	this->_menus.clear();
}

void MenuManager::createMenusMainMenuState(MainMenuState *menuState, ResourcesManager &resourcesManager)
{
	this->_menus["main"] = new Menu(this->_size, true);
	this->_menus.at("main")->addButton("start", new Button<MenuManager>(this, &MenuManager::openStartConfigPlayer, this->_font, "New game", 200, 48, 16, true));
	this->_menus.at("main")->addButton("exit", new Button<MainMenuState>(menuState, &MainMenuState::quit, this->_font, "Exit", 200, 48, 16, true));
	this->_menus["main_start_config_player"] = new Menu(this->_size, true);
	this->_menus.at("main_start_config_player")->addButton("config_2", new ConfigButton<MainMenuState>(menuState, &MainMenuState::configPlayer, 2, this->_font, "2 players", 200, 32, 16, true));
	this->_menus.at("main_start_config_player")->addButton("config_3", new ConfigButton<MainMenuState>(menuState, &MainMenuState::configPlayer, 3, this->_font, "3 players", 200, 32, 16, true));
	this->_menus.at("main_start_config_player")->addButton("config_4", new ConfigButton<MainMenuState>(menuState, &MainMenuState::configPlayer, 4, this->_font, "4 players", 200, 32, 16, true));
	this->_menus.at("main_start_config_player")->addButton("return", new Button<MenuManager>(this, &MenuManager::openMainMenu, this->_font, "Go back", 200, 32, 16, true));
}

void MenuManager::createMenusGameState(GameState *gameState, ResourcesManager &resourcesManager)
{
	this->_menus["unit_action"] = new Menu(this->_size);
	this->_menus.at("unit_action")->addButton("move", new Button<GameState>(gameState, &GameState::movePlayerUnit, this->_font, "Move"));
	this->_menus.at("unit_action")->addButton("attack", new Button<GameState>(gameState, &GameState::findTargets, this->_font, "Attack"));
	this->_menus["player_start"] = new Menu(this->_size);
	this->_menus.at("player_start")->addButton("end_turn", new Button<GameState>(gameState, &GameState::changeTurn, this->_font, "End turn"));
	this->_menus["enemy_building"] = new Menu(this->_size);
	this->_menus.at("enemy_building")->addButton("capture", new GenericButton(this->_font, "Capture"));
	this->_menus["gfactory"] = new Menu(this->_size);
	this->_menus.at("gfactory")->addButton("tank", new BuyButton<GameState>(resourcesManager.at("unit_tank"), gameState, &GameState::buyUnit, 6000, this->_font, "Create tank ($6000)"));
}

bool MenuManager::mouseMoved(const sf::Vector2f &mousePosition)
{
	if (this->_displayedMenu != "")
		return (this->_menus.at(this->_displayedMenu)->contains(mousePosition));
	return (false);
}

void MenuManager::click(const sf::Vector2f &mousePosition)
{
	sf::String tmp = this->_displayedMenu;
	if (this->_displayedMenu != "")
		this->_menus.at(this->_displayedMenu)->click(mousePosition);
	if (tmp == this->_displayedMenu)
		this->_displayedMenu = "";
}

void MenuManager::reset()
{
	this->_displayedMenu = "";
}

void MenuManager::openMainMenu()
{
	this->_displayedMenu = "main";
	this->setupMenu(sf::Vector2f((this->_size.x - 200) / 2, (this->_size.y - 96) / 2), this->_size);
}

void MenuManager::openStartConfigPlayer()
{
	this->_displayedMenu = "main_start_config_player";
	this->setupMenu(sf::Vector2f((this->_size.x - 200) / 2, (this->_size.y - 128) / 2), this->_size);
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
		window.draw(*this->_menus.at(this->_displayedMenu));
}

void MenuManager::setupMenu(sf::Vector2f pos, sf::Vector2f maxSize)
{
	sf::Vector2<sf::Uint16> size = this->_menus.at(this->_displayedMenu)->getSize();
	if (pos.x + size.x > maxSize.x)
		pos.x -= size.x;
	if (pos.y + size.y > maxSize.y)
		pos.y -= size.y;
	this->_menus.at(this->_displayedMenu)->setPosition(pos);
}
