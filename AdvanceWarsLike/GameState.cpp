#include "GameState.h"
#include "Unit.h"

//#include <Windows.h>

GameState::GameState(ResourcesManager &resourcesManager)
	: _gameMode(NORMAL), _resourcesManager(resourcesManager), _mapManager(_resourcesManager, _font), _menuManager(_font), _animationManager(_resourcesManager, _mapManager, this, _font, _realMapSize), _paused(false), _playersNumber(0), _mousePosition(0.0f, 0.0f), _turns(1)
{
	//std::vector<std::string> names;
	//std::string search_path = "./maps/*.*";
	//WIN32_FIND_DATA fd;
	//HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	//if (hFind != INVALID_HANDLE_VALUE) {
	//	do {
	//		// read all (real) files in current folder
	//		// , delete '!' read other 2 default folder . and ..
	//		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
	//			std::cout << fd.cFileName << std::endl;
	//			names.push_back(fd.cFileName);
	//		}
	//	} while (::FindNextFile(hFind, &fd));
	//	::FindClose(hFind);
	//}
	this->_mapManager.loadMap("map.tmx");
	this->_mapSize = this->_mapManager.getMapSize();
	this->_tileSize = this->_mapManager.getTileSize();
	this->_realMapSize = sf::Vector2f(static_cast<float>(this->_mapSize.x * this->_tileSize.x), static_cast<float>(this->_mapSize.y * this->_tileSize.y));
	this->setupTeams();
	this->setupBuildings(this->_mapManager.getBuildings());
	this->_players.at(*this->_currentPlayer)->startTurn();
	if (!this->_font.loadFromFile("uni0553-webfont.ttf"))
		std::cerr << "Can't load font in file " << __FILE__ << " at line " << __LINE__ << std::endl;
	const_cast<sf::Texture &>(this->_font.getTexture(8)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(10)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(16)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(24)).setSmooth(false);
	this->_hp = sf::Text("", this->_font, 8);
	this->_hp.setColor(sf::Color::White);
	this->_menuManager.createMenus(this, this->_resourcesManager);
}

GameState::~GameState()
{
	std::map<sf::String, Player *>::iterator iter = this->_players.begin();
	std::map<sf::String, Player *>::iterator iter2 = this->_players.end();

	while (iter != iter2)
	{
		delete (iter->second);
		++iter;
	}
}

void GameState::pause()
{
	this->_paused = true;
}

void GameState::resume()
{
	this->_paused = false;
}

void GameState::handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events)
{
	sf::Event event;
	sf::Vector2i tilePos;

	while (!events.empty() && !this->_paused)
	{
		event = events.front();
		if (event.type == sf::Event::MouseMoved)
		{
			this->_mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			this->_menuManager.mouseMoved(this->_mousePosition);
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			tilePos = sf::Vector2i(static_cast<sf::Int32>(this->_mousePosition.x) / this->_tileSize.x, static_cast<sf::Int32>(this->_mousePosition.y) / this->_tileSize.y);
			if (this->_gameMode == NORMAL && this->_currentPlayer != this->_playersTeams.end())
			{
				if (event.mouseButton.button == sf::Mouse::Right)
					this->_menuManager.openStartMenu(this->_mousePosition, this->_realMapSize);
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (this->_menuManager.mouseMoved(this->_mousePosition))
						this->_menuManager.click(this->_mousePosition);
					else
					{
						const std::vector<std::vector<IBuilding *>> &buildings = this->_mapManager.getBuildings();
						Tile tile = this->_mapManager.getTile(tilePos);
						// TODO: display tiles stats
						this->_menuManager.reset();
						Player::Click click = this->_players.at(*this->_currentPlayer)->click(tilePos, this->_mapManager);
						if (click == Player::Click::AIMED)
							this->_menuManager.openUnitActionMenu(this->_players.at(*this->_currentPlayer), this->_mousePosition, this->_realMapSize);
						else if (click == Player::Click::NOTINRANGE)
						{
							if (tilePos.x >= 0 && static_cast<sf::Uint32>(tilePos.x) < this->_mapSize.x && tilePos.y >= 0 && static_cast<sf::Uint32>(tilePos.y) < this->_mapSize.y)
							{
								IBuilding *tmp = buildings.at(tilePos.x).at(tilePos.y);
								if (tmp != nullptr && tmp->getPlayerId() != 255 && this->_playersTeams.at(tmp->getPlayerId()) == *this->_currentPlayer && this->_mapManager.getUnit(sf::Vector2u(tilePos)) == nullptr && tmp->getType().find("factory") != std::string::npos)
								{
									this->_tilePosition = sf::Vector2u(tilePos);
									this->_menuManager.openGFactoryMenu(this->_mousePosition, this->_realMapSize);
								}
							}
						}
					}
				}
			}
			else if (this->_gameMode == BATTLE)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					this->battle(tilePos);
				}
			}
			else if (this->_gameMode == ENDED)
				this->_finished = true;
		}
		events.pop();
	}
}

void GameState::update(const sf::Time &time)
{
	this->_resourcesManager.update(time);
	if (!this->_paused)
		if (this->_gameMode == ANIMATION)
		{
			if (!this->_animationManager.update(time) && this->_gameMode == ANIMATION)
				this->_gameMode = NORMAL;
		}
}

void GameState::display(sf::RenderWindow &window)
{
	this->_mapManager.draw(window);
	if (this->_currentPlayer != this->_playersTeams.end() && this->_gameMode != BATTLE)
		this->_players.at(*this->_currentPlayer)->drawMovement(window);
	std::vector<IUnit *>::const_iterator iter = this->_targets.begin();
	std::vector<IUnit *>::const_iterator iter2 = this->_targets.end();
	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(this->_tileSize.x), static_cast<float>(this->_tileSize.y)));
	rect.setFillColor(sf::Color(255, 0, 0, 127));
	while (iter != iter2)
	{
		rect.setPosition(sf::Vector2f(static_cast<float>((*iter)->getTilePosition().x * this->_tileSize.x), static_cast<float>((*iter)->getTilePosition().y * this->_tileSize.y)));
		window.draw(rect);
		++iter;
	}
	const std::vector<std::vector<IUnit *>> &units = this->_mapManager.getUnits();
	std::vector<std::vector<IUnit *>>::const_iterator iter3 = units.begin();
	std::vector<std::vector<IUnit *>>::const_iterator iter4 = units.end();
	sf::Vector2u pos;
	while (iter3 != iter4)
	{
		iter = iter3->begin();
		iter2 = iter3->end();
		while (iter != iter2)
		{
			if ((*iter) != nullptr)
			{
				pos = (*iter)->getTilePosition();
				this->_hp.setCharacterSize(10);
				this->_hp.setColor(sf::Color::Black);
				this->_hp.setString(std::to_string((*iter)->getLife()));
				this->_hp.setPosition(static_cast<float>((pos.x + 1) * this->_tileSize.x - 10), static_cast<float>((pos.y + 1) * this->_tileSize.y - 8));
				window.draw(this->_hp);
				this->_hp.setCharacterSize(8);
				this->_hp.setColor(sf::Color::White);
				this->_hp.setString(std::to_string((*iter)->getLife()));
				this->_hp.setPosition(static_cast<float>((pos.x + 1) * this->_tileSize.x - 10), static_cast<float>((pos.y + 1) * this->_tileSize.y - 7));
				window.draw(this->_hp);
			}
			++iter;
		}
		++iter3;
	}
	this->_menuManager.draw(window);
	if (this->_gameMode == ANIMATION || this->_gameMode == ENDED)
		this->_animationManager.draw(window);
}

bool GameState::ended() const
{
	return (this->_finished);
}

sf::Vector2f GameState::getViewSize() const
{
	return (this->_realMapSize);
}

void GameState::movePlayerUnit()
{
	this->_animationManager.play(AnimationManager::Infos(AnimationManager::Type::MOVEUNIT, this->_players.at(*this->_currentPlayer)));
	this->_gameMode = ANIMATION;
}

void GameState::changeTurn()
{
	this->_players.at(*this->_currentPlayer)->endTurn();
	++this->_currentPlayer;
	if (this->_currentPlayer == this->_playersTeams.end())
	{
		this->_currentPlayer = this->_playersTeams.begin();
		++this->_turns;
	}
	this->_players.at(*this->_currentPlayer)->startTurn();
	this->_gameMode = ANIMATION;
	this->_animationManager.play(AnimationManager::Infos(AnimationManager::Type::NEWTURN, *this->_currentPlayer, this->_turns));
}

void GameState::findTargets()
{
	const sf::Vector2u &unitPosition = this->_players.at(*this->_currentPlayer)->getAimedTile();
	IUnit *tmp;
	this->_targets.clear();

	if (unitPosition.x > 0)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x - 1, unitPosition.y))) != nullptr && tmp->getPlayerId() != this->_players.at(*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (unitPosition.y > 0)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x, unitPosition.y - 1))) != nullptr && tmp->getPlayerId() != this->_players.at(*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (unitPosition.x + 1 < this->_mapSize.x)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x + 1, unitPosition.y))) != nullptr && tmp->getPlayerId() != this->_players.at(*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (unitPosition.y + 1 < this->_mapSize.y)
	{
		if ((tmp = this->_mapManager.getUnit(sf::Vector2u(unitPosition.x, unitPosition.y + 1))) != nullptr && tmp->getPlayerId() != this->_players.at(*this->_currentPlayer)->getId())
			this->_targets.push_back(tmp);
	}
	if (!this->_targets.empty())
	{
		this->_animationManager.play(AnimationManager::Infos(AnimationManager::Type::ATTACKMOVEUNIT, this->_players.at(*this->_currentPlayer)));
		this->_gameMode = ANIMATION;
	}
}

void GameState::buyUnit(sf::Uint32 cost)
{
	if (this->_players.at(*this->_currentPlayer)->buy(cost))
	{
		IUnit *unit = new Unit();
		this->spawnUnit(this->_players.at(*this->_currentPlayer), unit, this->_tilePosition);
		unit->acted();
	}
}

void GameState::battleMode()
{
	if (this->_gameMode == ANIMATION)
		this->_gameMode = BATTLE;
}

void GameState::endGame()
{
	this->_gameMode = ENDED;
}

void GameState::battle(const sf::Vector2i &tilePos)
{
	std::vector<IUnit *>::iterator iter = this->_targets.begin();
	std::vector<IUnit *>::iterator iter2 = this->_targets.end();
	sf::Vector2u uTilePos = sf::Vector2u(tilePos);

	if (tilePos.x >= 0 && tilePos.y >= 0)
	{
		while (iter != iter2)
		{
			if ((*iter)->getTilePosition() == uTilePos && (*iter) != nullptr)
			{
				const sf::Uint8 &id = (*iter)->getPlayerId() - 1;
				IUnit *attacker = this->_players.at(*this->_currentPlayer)->getSelectedUnit();
				if (attacker != nullptr)
				{
					const sf::Uint8 &damageAttacker = static_cast<sf::Uint8>(attacker->getDamage() * 0.6f);
					const sf::Uint8 &damageDefender = static_cast<sf::Uint8>((*iter)->getDamage() * 0.4f);
					if ((*iter)->hit(damageAttacker))
					{
						this->_mapManager.removeUnit(uTilePos);
						this->_players.at(this->_playersTeams.at(id))->destroyUnit(*iter);
					}
					else if (attacker->hit(damageDefender))
					{
						this->_mapManager.removeUnit(attacker->getTilePosition());
						this->_players.at(*this->_currentPlayer)->destroyUnit(attacker);
					}
				}
				this->_targets.clear();
				this->_players.at(*this->_currentPlayer)->endAttack();
				this->_gameMode = NORMAL;
				if (this->_players.at(this->_playersTeams.at(id))->getUnitsNumber() == 0)
				{
					this->_animationManager.play(AnimationManager::Infos(AnimationManager::Type::WINNING, (*this->_currentPlayer), this->_turns));
					this->_gameMode = ANIMATION;
				}
				else if (this->_players.at(*this->_currentPlayer)->getUnitsNumber() == 0)
				{
					this->_animationManager.play(AnimationManager::Infos(AnimationManager::Type::WINNING, this->_playersTeams.at(id), this->_turns));
					this->_gameMode = ANIMATION;
				}
				break;
			}
			++iter;
		}
	}
}

void GameState::addPlayer()
{
	Player *player = new Player(++this->_playersNumber, this->_mapManager);
	this->_playersTeams.push_back(this->_availablePlayersTeams.at(this->_playersNumber));
	player->setMapSize(this->_mapSize);
	if (this->_playersNumber == 1)
	{
		//this->spawnUnit(player, new Unit(), sf::Vector2u(2, 4));
		this->spawnUnit(player, new Unit(), sf::Vector2u(1, 1));
		//this->spawnUnit(player, new Unit(), sf::Vector2u(12, 2));
	}
	else if (this->_playersNumber == 2)
	{
		this->spawnUnit(player, new Unit(), sf::Vector2u(4, 2));
		this->spawnUnit(player, new Unit(), sf::Vector2u(2, 12));
	}
	this->_players[this->_playersTeams.at(this->_playersNumber - 1)] = player;
}

void GameState::spawnUnit(Player *player, IUnit *unit, sf::Vector2u position)
{
	unit->setGraphicsComponent("move_right", new GraphicsComponent(this->_resourcesManager.at("unit_tank_move_right")));
	unit->setGraphicsComponent("move_left", new GraphicsComponent(this->_resourcesManager.at("unit_tank_move_left")));
	unit->setGraphicsComponent("move_up", new GraphicsComponent(this->_resourcesManager.at("unit_tank_move_up")));
	unit->setGraphicsComponent("move_down", new GraphicsComponent(this->_resourcesManager.at("unit_tank_move_down")));
	unit->setGraphicsComponent("afk", new GraphicsComponent(this->_resourcesManager.at("unit_tank_afk")));
	unit->setGraphicsComponent("stand", new GraphicsComponent(this->_resourcesManager.at("unit_tank")));
	unit->setStatisticsComponent(new TankStatisticsComponent());
	unit->setTilePosition(position);
	player->addUnit(unit);
	this->_mapManager.addUnit(unit, unit->getTilePosition());
}

void GameState::setupBuildings(const std::vector<std::vector<IBuilding *>> &buildings)
{
	std::vector<std::vector<IBuilding *>>::const_iterator iter = buildings.begin();
	std::vector<std::vector<IBuilding *>>::const_iterator iter2 = buildings.end();
	std::vector<IBuilding *>::const_iterator iter3;
	std::vector<IBuilding *>::const_iterator iter4;
	std::vector<sf::String>::const_iterator iter5;
	std::vector<sf::String>::const_iterator iter6 = this->_playersTeams.end();

	while (iter != iter2)
	{
		iter3 = iter->begin();
		iter4 = iter->end();
		while (iter3 != iter4)
		{
			if ((*iter3) != nullptr)
			{
				const sf::String &type = (*iter3)->getType();
				iter5 = this->_playersTeams.begin();
				while (iter5 != iter6)
				{
					if (type.find(*iter5) != std::string::npos)
					{
						this->_players.at(*iter5)->addBuilding(*iter3);
						(*iter3)->setPlayer(iter5 - this->_playersTeams.begin());
					}
					++iter5;
				}
				(*iter3)->setGraphicsComponent(new BuildingGraphicsComponent(this->_resourcesManager.at(type)));
			}
			++iter3;
		}
		++iter;
	}
}

void GameState::setupTeams()
{
	this->_availablePlayersTeams.push_back("neutral");
	this->_availablePlayersTeams.push_back("red");
	this->_availablePlayersTeams.push_back("blue");
	this->_availablePlayersTeams.push_back("green");
	this->_availablePlayersTeams.push_back("yellow");
	this->_availablePlayersTeams.push_back("black");
	this->addPlayer();
	this->addPlayer();
	this->_currentPlayer = this->_playersTeams.begin();
}
