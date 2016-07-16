#include "GameState.h"
#include "Unit.h"

GameState::GameState(ResourcesManager &resourcesManager)
	: _gameMode(NORMAL), _resourcesManager(resourcesManager), _mapManager(_resourcesManager), _menuManager(_font), _playersNumber(0), _mousePosition(0.0f, 0.0f)
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
			tilePos = sf::Vector2i(static_cast<sf::Int32>(this->_mousePosition.x / 16), static_cast<sf::Int32>(this->_mousePosition.y / 16));
			if (this->_gameMode == NORMAL)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
					this->_menuManager.openStartMenu(this->_mousePosition);
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (this->_menuManager.mouseMoved(this->_mousePosition))
						this->_menuManager.click(this->_mousePosition);
					else
					{
						this->_menuManager.reset();
						if (this->_currentPlayer != this->_players.end())
						{
							if ((*this->_currentPlayer)->click(tilePos))
								this->_menuManager.openUnitActionMenu(*this->_currentPlayer, this->_mousePosition);
						}
					}
				}
			}
			else if (this->_gameMode == BATTLE)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::vector<IUnit *>::iterator iter = this->_targets.begin();
					std::vector<IUnit *>::iterator iter2 = this->_targets.end();
					sf::Vector2u uTilePos = sf::Vector2u(tilePos);

					if (tilePos.x >= 0 && tilePos.y >= 0)
					{
						while (iter != iter2)
						{
							if ((*iter)->getTilePosition() == uTilePos)
							{
								// TODO: calculate damages output
								this->_mapManager.removeUnit(uTilePos);
								this->_players.at((*iter)->getPlayerId())->destroyUnit(*iter);
								this->_targets.clear();
								this->_gameMode = NORMAL;
								break;
							}
							++iter;
						}
					}
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
	std::vector<IUnit *>::const_iterator iter = this->_targets.begin();
	std::vector<IUnit *>::const_iterator iter2 = this->_targets.end();
	sf::RectangleShape rect(sf::Vector2f(16, 16));
	rect.setFillColor(sf::Color(255, 0, 0, 127));
	while (iter != iter2)
	{
		rect.setPosition(sf::Vector2f((*iter)->getTilePosition().x * 16, (*iter)->getTilePosition().y * 16));
		window.draw(rect);
		++iter;
	}
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

void GameState::findTargets()
{
	const sf::Vector2u &unitPosition = (*this->_currentPlayer)->getAimedTile();
	IUnit *tmp;
	this->_targets.clear();

	if (unitPosition.x > 0)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x - 1, unitPosition.y))) != nullptr && tmp->getPlayerId() != (*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (unitPosition.y > 0)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x, unitPosition.y - 1))) != nullptr && tmp->getPlayerId() != (*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (unitPosition.x + 1 < this->_mapSize.x)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x + 1, unitPosition.y))) != nullptr && tmp->getPlayerId() != (*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (unitPosition.y + 1 < this->_mapSize.y)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x, unitPosition.y + 1))) != nullptr && tmp->getPlayerId() != (*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (!this->_targets.empty())
	{
		(*this->_currentPlayer)->moveUnit();
		this->_gameMode = BATTLE;
	}
}

void GameState::addPlayer()
{
	Player *player = new Player(this->_playersNumber, this->_mapManager);
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
