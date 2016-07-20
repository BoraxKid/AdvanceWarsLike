#include "Application.h"

Application::Application(sf::Vector2u winSize)
	: _window(sf::VideoMode(winSize.x, winSize.y), "Advance Wars Like"), _view(sf::Vector2f(0, 0), sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y))), _resourcesManager()
{
	this->_window.setVerticalSyncEnabled(true);
	this->_window.setView(this->_view);
	this->_states.push_back(new GameState(this->_resourcesManager));
	this->resize(sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)), this->_states.back()->getViewSize());
}

Application::~Application()
{
	std::vector<IState *>::iterator iter = this->_states.begin();
	std::vector<IState *>::iterator iter2 = this->_states.end();

	while (iter != iter2)
	{
		delete (*iter);
		++iter;
	}
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
			else if (e.type == sf::Event::Resized)
				this->resize(sf::Vector2f(static_cast<float>(e.size.width), static_cast<float>(e.size.height)), this->_states.back()->getViewSize());
			else
				events.push(e);
		}
		this->_states.back()->handleEvents(this->_window, events);
		this->_states.back()->update(clock.restart());
		this->_window.clear(sf::Color::Cyan);
		this->_states.back()->display(this->_window);
		this->_window.display();
		if (this->_states.back()->ended())
		{
			delete (this->_states.back());
			this->_states.erase(this->_states.end() - 1);
		}
	}
	if (this->_window.isOpen())
		this->_window.close();
}

void Application::resize(sf::Vector2f winSize, sf::Vector2f viewSize)
{
	this->_view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(winSize.x, winSize.y));
	this->_view.setCenter(viewSize.x / 2, viewSize.y / 2);
	float a = viewSize.x / winSize.x;
	float b = viewSize.y / winSize.y;
	this->_view.zoom((a > b) ? a : b);
	this->_window.setView(this->_view);
}
