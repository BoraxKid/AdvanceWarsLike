#ifndef APPLICATION_H_
#define APPLICATION_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

#include "IState.h"
#include "GameState.h"
#include "ResourcesManager.h"

class Application
{
public:
	Application(sf::Vector2u winSize = sf::Vector2u(256, 240));
	virtual ~Application();

	void run();

private:
	void resize(sf::Vector2f winSize, sf::Vector2f viewSize);

	sf::RenderWindow _window;
	sf::View _view;
	std::vector<IState *> _states;
	ResourcesManager _resourcesManager;
};

#endif // APPLICATION_H_