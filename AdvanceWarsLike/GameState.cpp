#include "GameState.h"
#include "Unit.h"

GameState::GameState(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager), _mapManager(_resourcesManager)
{
	this->_mapManager.loadMap("map.tmx");
	this->spawnUnit(this->_player, new Unit(), sf::Vector2u(2, 2));
	this->spawnUnit(this->_player, new Unit(), sf::Vector2u(12, 2));
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

void GameState::handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events)
{
	sf::Event event;
	sf::Vector2f tmp(-1, -1);

	while (!events.empty())
		{
			event = events.front();
			if (event.type == sf::Event::MouseMoved)
				tmp = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			events.pop();
		}
	if (tmp.x != -1 && tmp.y != -1)
		std::cout << "x: " << static_cast<int>(tmp.x / 16) << " y: " << static_cast<int>(tmp.y / 16) << std::endl;
}

void GameState::update(const sf::Time &time)
{
	this->_resourcesManager.update(time);
}

void GameState::display(sf::RenderWindow &window)
{
	this->_mapManager.draw(window);
}

void GameState::spawnUnit(Player &player, IUnit *unit, sf::Vector2u position)
{
	unit->setGraphicsComponent(new GraphicsComponent(this->_resourcesManager.at("unit_tank")));
	unit->setStatisticsComponent(new TankStatisticsComponent());
	unit->move(position);
	player.addUnit(unit);
	this->_mapManager.addUnit(unit, unit->getTilePosition());
}
