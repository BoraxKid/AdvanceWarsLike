#ifndef ISATE_H_
#define ISATE_H_
#pragma once

#include <SFML/Graphics.hpp>
#include <queue>

class IState
{
public:
	virtual ~IState() {}

	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void handleEvents(std::queue<sf::Event> &events) = 0;
	virtual void update(const sf::Time &time) = 0;
	virtual void display(sf::RenderWindow &window) = 0;
};

#endif // ISATE_H_