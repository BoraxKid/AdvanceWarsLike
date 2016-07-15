#include "GameState.h"
#include "Unit.h"

GameState::GameState(ResourcesManager &resourcesManager)
	: _resourcesManager(resourcesManager), _mapManager(_resourcesManager), _menuManager(_font), _playersNumber(0), _mousePosition(0.0f, 0.0f)
{
	this->_mapManager.loadMap("map.tmx");
	this->_mapSize = this->_mapManager.getMapSize();
	this->addPlayer();
	this->addPlayer();
	this->_currentPlayer = this->_players.begin();
	if (!this->_font.loadFromFile("uni0553-webfont.ttf"))
		std::cerr << "Can't load font in file " << __FILE__ << " at line " << __LINE__ << std::endl;
	this->_menuManager.createMenus(this);
}

GameState::~GameState()
{
	std::vector<Player *>::iterator iter = this->_players.begin();
	std::vector<Player *>::iterator iter2 = this->_players.end();

	while (iter != iter2)
	{
		delete (*iter);
		++iter;
	}
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
			this->_menuManager.mouseMoved(this->_mousePosition);
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Right)
				this->_menuManager.openStartMenu(this->_mousePosition);
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				tilePos = sf::Vector2i(static_cast<sf::Int32>(this->_mousePosition.x / 16), static_cast<sf::Int32>(this->_mousePosition.y / 16));
				if (this->_menuManager.mouseMoved(this->_mousePosition))
					this->_menuManager.click(this->_mousePosition);
				else
				{
					this->_menuManager.reset();
					if (this->_currentPlayer != this->_players.end())
						(*this->_currentPlayer)->click(this->_mapManager, tilePos);
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
	if (this->_currentPlayer != this->_players.end())
		(*this->_currentPlayer)->drawMovement(window);
	this->_menuManager.draw(window);
}

void GameState::changeTurn()
{
	if (this->_currentPlayer != this->_players.end() - 1)
		++this->_currentPlayer;
	else
		this->_currentPlayer = this->_players.begin();
	std::cout << "Changing turn" << std::endl;
}

void GameState::addPlayer()
{
	Player *player = new Player(this->_playersNumber);
	player->setMapSize(this->_mapSize);
	if (this->_playersNumber == 0)
	{
		this->spawnUnit(player, new Unit(), sf::Vector2u(2, 4));
		this->spawnUnit(player, new Unit(), sf::Vector2u(12, 2));
	}
	else if (this->_playersNumber == 1)
	{
		this->spawnUnit(player, new Unit(), sf::Vector2u(4, 2));
		this->spawnUnit(player, new Unit(), sf::Vector2u(2, 12));
	}
	this->_players.push_back(player);
	++this->_playersNumber;
}

void GameState::spawnUnit(Player *player, IUnit *unit, sf::Vector2u position)
{
	unit->setGraphicsComponent(new GraphicsComponent(this->_resourcesManager.at("unit_tank")));
	unit->setStatisticsComponent(new TankStatisticsComponent());
	unit->move(position);
	player->addUnit(unit);
	this->_mapManager.addUnit(unit, unit->getTilePosition());
}
