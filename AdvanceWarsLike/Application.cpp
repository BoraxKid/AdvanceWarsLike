#include "Application.h"

Application::Application()
	: _window(sf::VideoMode(1280, 720), "Advance Wars Like"), _resourcesManager()
{
	this->_window.setVerticalSyncEnabled(true);
	this->_states.push_back(new GameState(this->_resourcesManager));
}

Application::~Application()
{
	this->_states.clear();
}

void Application::run()
{
	std::queue<sf::Event> events;
	sf::Event e;
	sf::Clock clock;

	while (this->_window.isOpen() && !this->_states.empty())
	{
		while (this->_window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				this->_window.close();
			else
				events.push(e);
		}
		this->_states.back()->handleEvents(events);
		this->_states.back()->update(clock.restart());
		this->_window.clear(sf::Color::Cyan);
		this->_states.back()->display(this->_window);
		this->_window.display();
	}
}
