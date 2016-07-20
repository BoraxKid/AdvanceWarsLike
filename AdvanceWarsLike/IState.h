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
	virtual void handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events) = 0;
	virtual void update(const sf::Time &time) = 0;
	virtual void display(sf::RenderWindow &window) = 0;
	virtual bool ended() const = 0;
	virtual IState *newState() const = 0;
	virtual sf::Vector2f getViewSize() const = 0;
};

#endif // ISATE_H_
