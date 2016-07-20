#ifndef APPLICATION_H_
#define APPLICATION_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

#include "IState.h"
#include "MainMenuState.h"
#include "ResourcesManager.h"

class Application
{
public:
	Application(sf::Vector2u winSize = sf::Vector2u(256, 240));
	virtual ~Application();

	void run();

private:
	void resize(sf::Vector2f winSize, sf::Vector2f viewSize);
	void pushState(IState *state);
	void popState();

	sf::RenderWindow _window;
	sf::Font _font;
	sf::View _view;
	sf::Vector2f _winSize;
	std::vector<IState *> _states;
	ResourcesManager _resourcesManager;
};

#endif // APPLICATION_H_