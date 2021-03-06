#include "Application.h"

Application::Application(sf::Vector2u winSize)
	: _window(sf::VideoMode(winSize.x, winSize.y), "Advance Wars Like"), _winSize(sf::Vector2f(winSize)), _view(sf::Vector2f(0, 0), _winSize), _resourcesManager()
{
	if (!this->_font.loadFromFile("resources/uni0553-webfont.ttf"))
		std::cerr << "Can't load font in file " << __FILE__ << " at line " << __LINE__ << std::endl;
	const_cast<sf::Texture &>(this->_font.getTexture(8)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(10)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(16)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(24)).setSmooth(false);
	const_cast<sf::Texture &>(this->_font.getTexture(48)).setSmooth(false);
	this->_window.setVerticalSyncEnabled(true);
	this->_window.setView(this->_view);
	this->pushState(new MainMenuState(this->_resourcesManager, this->_font, this->_winSize));
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
		this->_window.clear(sf::Color(200, 200, 200));
		this->_states.back()->display(this->_window);
		this->_window.display();
		if (this->_states.back()->ended())
			this->popState();
		else if (this->_states.back()->newState() != nullptr)
			this->pushState(this->_states.back()->newState());
	}
	if (this->_window.isOpen())
		this->_window.close();
}

void Application::resize(sf::Vector2f winSize, sf::Vector2f viewSize)
{
	this->_winSize.x = winSize.x;
	this->_winSize.y = winSize.y;
	this->_view = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(winSize.x, winSize.y));
	this->_view.setCenter(viewSize.x / 2, viewSize.y / 2);
	float a = viewSize.x / winSize.x;
	float b = viewSize.y / winSize.y;
	this->_view.zoom((a > b) ? a : b);
	this->_window.setView(this->_view);
}

void Application::pushState(IState *state)
{
	if (!this->_states.empty())
		this->_states.back()->pause();
	this->_states.push_back(state);
	this->resize(sf::Vector2f(static_cast<float>(this->_winSize.x), static_cast<float>(this->_winSize.y)), this->_states.back()->getViewSize());
}

void Application::popState()
{
	if (!this->_states.empty())
	{
		delete (this->_states.back());
		this->_states.pop_back();
		if (!this->_states.empty())
			this->_states.back()->resume();
	}
}
