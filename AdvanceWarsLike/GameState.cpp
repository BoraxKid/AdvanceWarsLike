#include "GameState.h"
#include "Unit.h"
GameState::GameState(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager), _mapManager(_resourcesManager)
{
	this->_mapManager.loadMap("map.tmx");
	this->spawnUnit(this->_player, new Unit(sf::Vector2u(2, 2)));
	this->spawnUnit(this->_player, new Unit(sf::Vector2u(12, 2)));
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
	unit->setStatisticsComponent(new TankStatisticsComponent());
	player.addUnit(unit);
	this->_mapManager.addUnit(unit, unit->getTilePosition());
}
