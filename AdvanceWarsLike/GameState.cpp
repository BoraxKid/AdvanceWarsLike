#include "GameState.h"
#include "Unit.h"

GameState::GameState(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager), _mapManager(_resourcesManager), _mousePosition(0.0f, 0.0f), _selectedUnit(nullptr)
{
	this->_mapManager.loadMap("map.tmx");
	this->_mapSize = this->_mapManager.getMapSize();
	this->resetMovementMap();
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
	this->_menu.setPosition(sf::Vector2f(300, 20));
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
	sf::Vector2i tilePos;

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
			if (event.mouseButton.button == sf::Mouse::Right)
				this->dump();
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				tilePos = sf::Vector2i(static_cast<sf::Int32>(this->_mousePosition.x / 16), static_cast<sf::Int32>(this->_mousePosition.y / 16));
				if (this->_menu.contains(this->_mousePosition))
					this->_menu.click(this->_mousePosition);
				else if (tilePos.x >= 0 && tilePos.y >= 0 && tilePos.x < static_cast<sf::Int32>(this->_mapSize.x) && tilePos.y < static_cast<sf::Int32>(this->_mapSize.y))
				{
					std::cout << "x: " << static_cast<int>(this->_mousePosition.x / 16) << " y: " << static_cast<int>(this->_mousePosition.y / 16) << std::endl;
					this->resetMovementMap();
					this->calculateMovement(tilePos);
					this->_selectedUnit = this->_mapManager.getUnit(sf::Vector2u(tilePos.x, tilePos.y));
				}
			}
		}
		events.pop();
	}
}

void GameState::update(const sf::Time &time)
{
	this->_resourcesManager.update(time);
}

void GameState::display(sf::RenderWindow &window)
{
	this->_mapManager.draw(window);

	if (this->_selectedUnit != nullptr)
	{
		const sf::Uint8 move = this->_selectedUnit->getMovement();
		sf::Vector2u tmp;
		sf::RectangleShape rect(sf::Vector2f(16, 16));
		rect.setFillColor(sf::Color(0, 0, 255, 127));
		while (tmp.y < this->_mapSize.y)
		{
			tmp.x = 0;
			while (tmp.x < this->_mapSize.x)
			{
				if (this->_movement.at(tmp.x).at(tmp.y) <= move)
				{
					rect.setPosition(sf::Vector2f(16 * tmp.x, 16 * tmp.y));
					window.draw(rect);
				}
				++tmp.x;
			}
			++tmp.y;
		}
	}
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

void GameState::resetMovementMap()
{
	this->_movement.clear();
	this->_movement = std::vector<std::vector<sf::Int32>>(this->_mapSize.x);
	std::vector<std::vector<sf::Int32>>::iterator iter = this->_movement.begin();
	std::vector<std::vector<sf::Int32>>::iterator iter2 = this->_movement.end();
	while (iter != iter2)
	{
		(*iter) = std::vector<sf::Int32>(this->_mapSize.y, -1);
		++iter;
	}
}

void GameState::calculateMovement(const sf::Vector2i &tilePos, sf::Uint8 movement)
{
	std::vector<sf::Vector2i> q;
	q.push_back(sf::Vector2i(tilePos.x, tilePos.y));
	sf::Vector2i tmp = tilePos;
	sf::Vector2i tmp2;
	this->_movement.at(tmp.x).at(tmp.y) = 0;

	while (!q.empty())
	{
		tmp = q.front();
		q.erase(q.begin());
		if (tmp.x - 1 >= 0)
			if (this->_movement.at(tmp.x - 1).at(tmp.y) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x - 1, tmp.y);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
		if (tmp.y - 1 >= 0)
			if (this->_movement.at(tmp.x).at(tmp.y - 1) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x, tmp.y - 1);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
		if (tmp.x + 1 < this->_mapSize.x)
			if (this->_movement.at(tmp.x + 1).at(tmp.y) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x + 1, tmp.y);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
		if (tmp.y + 1 < this->_mapSize.y)
			if (this->_movement.at(tmp.x).at(tmp.y + 1) == -1)
			{
				tmp2 = sf::Vector2i(tmp.x, tmp.y + 1);
				this->_movement.at(tmp2.x).at(tmp2.y) = this->_movement.at(tmp.x).at(tmp.y) + 1;
				q.push_back(tmp2);
			}
	}
}

void GameState::dump()
{
	sf::Vector2u tmp(0, 0);

	while (tmp.y < this->_mapSize.y)
	{
		tmp.x = 0;
		while (tmp.x < this->_mapSize.x)
		{
			if (tmp.x != 0)
				std::cout << "\t";
			std::cout << this->_movement.at(tmp.x).at(tmp.y);
			++tmp.x;
		}
		std::cout << std::endl;
		++tmp.y;
	}
}
