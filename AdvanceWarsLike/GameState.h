#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#pragma once

#include "IState.h"
#include "ResourcesManager.h"
#include "MapManager.h"
#include "Player.h"
#include "MenuManager.h"

class GameState : public IState
{
public:
	GameState(ResourcesManager &resourcesManager);
	virtual ~GameState();

	void pause();
	void resume();
	void handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events);
	void update(const sf::Time &time);
	void display(sf::RenderWindow &window);
	void changeTurn();

private:
	void addPlayer();
	void spawnUnit(Player *player, IUnit *unit, sf::Vector2u position);

	ResourcesManager &_resourcesManager;
	MapManager _mapManager;
	MenuManager _menuManager;
	sf::Vector2u _mapSize;
	sf::Uint8 _playersNumber;
	std::vector<Player *> _players;
	std::vector<Player *>::iterator _currentPlayer;
	sf::Vector2f _mousePosition;
	sf::Font _font;
};

#endif // GAMESTATE_H_
