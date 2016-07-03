#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#pragma once

#include "IState.h"
#include "ResourcesManager.h"
#include "MapManager.h"
#include "Player.h"

class GameState : public IState
{
public:
	GameState(ResourcesManager &resourcesManager);
	virtual ~GameState();

	void pause();
	void resume();
	void handleEvents(std::queue<sf::Event> &events);
	void update(const sf::Time &time);
	void display(sf::RenderWindow &window);

private:
	void spawnUnit(Player &player, IUnit *unit);

	ResourcesManager &_resourcesManager;
	MapManager _mapManager;
	Player _player;
};

#endif // GAMESTATE_H_