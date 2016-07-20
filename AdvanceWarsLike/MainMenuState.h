#ifndef MAINMENUSTATE_H_
#define MAINMENUSTATE_H_
#pragma once

#include <Windows.h>

#include "IState.h"
#include "ResourcesManager.h"
#include "MenuManager.h"
#include "StrokeText.h"

class MainMenuState : public IState
{
public:
	MainMenuState(ResourcesManager &resourcesManager, const sf::Font &font, const sf::Vector2f &winSize);
	virtual ~MainMenuState();

	virtual void pause();
	virtual void resume();
	virtual void handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events);
	virtual void update(const sf::Time &time);
	virtual void display(sf::RenderWindow &window);
	virtual bool ended() const;
	virtual IState *newState() const;
	virtual sf::Vector2f getViewSize() const;

	void configPlayer(sf::Uint8 number);
	void quit();

private:
	ResourcesManager &_resourcesManager;
	const sf::Font &_font;
	MenuManager _menuManager;
	sf::Vector2f _size;
	bool _paused;
	bool _quit;
	sf::Vector2f _mousePosition;
	StrokeText _gameTitle;
	IState *_state;
};

#endif // MAINMENUSTATE_H_
