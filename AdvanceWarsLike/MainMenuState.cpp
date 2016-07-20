#include "MainMenuState.h"
#include "GameState.h"

MainMenuState::MainMenuState(ResourcesManager &resourcesManager, const sf::Font &font, const sf::Vector2f &winSize)
	: _resourcesManager(resourcesManager), _font(font), _size(winSize), _menuManager(_font, winSize), _paused(false), _quit(false), _gameTitle("Tanks!", _font, 48)
{
	this->_menuManager.createMenusMainMenuState(this, resourcesManager);
	this->_menuManager.openMainMenu();
	this->_gameTitle.setColor(sf::Color::White);
	this->_gameTitle.setOutlineColor(sf::Color::Black);
	this->_gameTitle.setOutlineThickness(2);
	this->_gameTitle.setTextPosition((winSize.x - this->_gameTitle.getTextLocalBounds().width) / 2, 0);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::pause()
{
	this->_paused = true;
	this->_state = nullptr;
}

void MainMenuState::resume()
{
	this->_paused = false;
	this->_menuManager.openMainMenu();
}

void MainMenuState::handleEvents(sf::RenderWindow &window, std::queue<sf::Event> &events)
{
	sf::Event e;

	while (!events.empty() && !this->_paused)
	{
		e = events.front();
		if (e.type == sf::Event::MouseMoved)
		{
			this->_mousePosition = window.mapPixelToCoords(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));
			this->_menuManager.mouseMoved(this->_mousePosition);
		}
		else if (e.type == sf::Event::MouseButtonPressed)
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				if (this->_menuManager.mouseMoved(this->_mousePosition))
				{
					this->_menuManager.click(this->_mousePosition);
					this->_menuManager.mouseMoved(this->_mousePosition);
				}
			}
		}
		events.pop();
	}
}

void MainMenuState::update(const sf::Time &time)
{
}

void MainMenuState::display(sf::RenderWindow &window)
{
	window.draw(this->_gameTitle);
	this->_menuManager.draw(window);
}

bool MainMenuState::ended() const
{
	return (this->_quit);
}

IState *MainMenuState::newState() const
{
	return (this->_state);
}

sf::Vector2f MainMenuState::getViewSize() const
{
	return (this->_size);
}

void MainMenuState::configPlayer(sf::Uint8 number)
{
	this->_state = new GameState(this->_resourcesManager, this->_font, "./resources/map_" + std::to_string(number) + "p.tmx");
}

void MainMenuState::quit()
{
	this->_quit = true;
}
