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
	Application();
	virtual ~Application();

	void run();

private:
	sf::RenderWindow _window;
	std::vector<IState *> _states;
	ResourcesManager _resourcesManager;
};

#endif // APPLICATION_H_