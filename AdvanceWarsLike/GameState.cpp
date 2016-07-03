#include "GameState.h"

GameState::GameState(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager), _mapManager(_resourcesManager)
{
	this->_mapManager.loadMap("map.tmx");
}

GameState::~GameState()
{
}

void GameState::pause()
{
}

void GameState::resume()
{
}

void GameState::handleEvents(std::queue<sf::Event> &events)
{
}

void GameState::update(const sf::Time &time)
{
	this->_resourcesManager.update(time);
}

void GameState::display(sf::RenderWindow &window)
{
	this->_mapManager.draw(window);
}

void GameState::spawnUnit(Player &player, IUnit *unit)
{
	unit->setGraphicsComponent(new GraphicsComponent(this->_resourcesManager.at("unit_tank")));
	player.addUnit(unit);
}
