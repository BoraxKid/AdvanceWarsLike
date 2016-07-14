#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#pragma once

#include "IState.h"
#include "ResourcesManager.h"
#include "MapManager.h"
#include "Player.h"
#include "Menu.h"

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

private:
	void spawnUnit(Player &player, IUnit *unit, sf::Vector2u position);
	void resetMovementMap();
	void calculateMovement(const sf::Vector2i &tilePos, sf::Uint8 movement = 5);
	void dump();

	ResourcesManager &_resourcesManager;
	MapManager _mapManager;
	sf::Vector2u _mapSize;
	Player _player;
	sf::Vector2f _mousePosition;
	sf::Font _font;
	Menu _menu;
	std::vector<std::vector<sf::Int32>> _movement;
	IUnit *_selectedUnit;
};

#endif // GAMESTATE_H_
