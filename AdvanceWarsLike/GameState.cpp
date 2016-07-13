#include "GameState.h"
#include "Unit.h"

GameState::GameState(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager), _mapManager(_resourcesManager), _mousePosition(0.0f, 0.0f)
{
	this->_mapManager.loadMap("map.tmx");
	this->spawnUnit(this->_player, new Unit(), sf::Vector2u(2, 4));
	this->spawnUnit(this->_player, new Unit(), sf::Vector2u(12, 2));
	if (!this->_font.loadFromFile("uni0553-webfont.ttf"))
		std::cerr << "Can't load font in file " << __FILE__ << " at line " << __LINE__ << std::endl;
	this->_menu.addButton(this->_font, "button 1");
	this->_menu.addButton(this->_font, "BUTTON 2");
	this->_menu.addButton(this->_font, "button 3");
	this->_menu.addButton(this->_font, "BUTTON 4 (FAR TOO LONG)");
	this->_menu.addButton(this->_font, "BUTTON 5");
	this->_menu.addButton(this->_font, "button 6");
	this->_menu.setPosition(sf::Vector2f(30, 20));
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

	while (!events.empty())
		{
			event = events.front();
			if (event.type == sf::Event::MouseMoved)
			{
				this->_mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				this->_menu.contains(this->_mousePosition);
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Event::MouseLeft)
					this->_menu.click(this->_mousePosition);
			}
			events.pop();
		}
	// std::cout << "x: " << static_cast<int>(this->_mousePosition.x / 16) << " y: " << static_cast<int>(this->_mousePosition.y / 16) << std::endl;
}

void GameState::update(const sf::Time &time)
{
	this->_resourcesManager.update(time);
}

void GameState::display(sf::RenderWindow &window)
{
	this->_mapManager.draw(window);
	window.draw(this->_menu);
}

void GameState::spawnUnit(Player &player, IUnit *unit, sf::Vector2u position)
{
	unit->setGraphicsComponent(new GraphicsComponent(this->_resourcesManager.at("unit_tank")));
	unit->setStatisticsComponent(new TankStatisticsComponent());
	unit->move(position);
	player.addUnit(unit);
	this->_mapManager.addUnit(unit, unit->getTilePosition());
}
