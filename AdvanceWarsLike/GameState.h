#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#pragma once

#include "IState.h"
#include "ResourcesManager.h"
#include "MapManager.h"
#include "MenuManager.h"
#include "AnimationManager.h"
#include "Player.h"
#include "StrokeText.h"

class GameState : public IState
{
public:
	enum GameMode { INVALID, NORMAL, BATTLE, ANIMATION, ENDED };
	GameState(ResourcesManager &resourcesManager, const sf::Font &font, sf::String map);
	virtual ~GameState();

	virtual void pause();
	virtual void resume();
	virtual void handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events);
	virtual void update(const sf::Time &time);
	virtual void display(sf::RenderWindow &window);
	virtual bool ended() const;
	virtual IState *newState() const;
	virtual sf::Vector2f getViewSize() const;

	void movePlayerUnit();
	void changeTurn();
	void findTargets();
	void buyUnit(sf::Uint32 cost);
	void battleMode();
	void endGame();

private:
	void battle(const sf::Vector2i &tilePos);
	void addPlayer();
	void spawnUnit(Player *player, IUnit *unit, sf::Vector2u position);
	void setupBuildings(const std::vector<std::vector<IBuilding *>> &buildings);
	void setupTeams();

	GameMode _gameMode;
	ResourcesManager &_resourcesManager;
	const sf::Font &_font;
	MapManager _mapManager;
	MenuManager _menuManager;
	AnimationManager _animationManager;
	bool _finished;
	bool _paused;
	sf::Vector2u _mapSize;
	sf::Vector2u _tileSize;
	sf::Vector2f _realMapSize;
	sf::Uint8 _playersNumber;
	std::vector<sf::String> _availablePlayersTeams;
	std::vector<sf::String> _playersTeams;
	std::vector<sf::String>::iterator _currentPlayer;
	std::map<sf::String, Player *> _players;
	sf::Vector2f _mousePosition;
	sf::Vector2u _tilePosition;
	StrokeText _hp;
	std::vector<IUnit *> _targets;
	sf::Uint32 _turns;
};

#endif // GAMESTATE_H_
