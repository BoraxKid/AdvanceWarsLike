#include "AnimationManager.h"

AnimationManager::AnimationManager(ResourcesManager &resourcesManager, MapManager &mapManager, GameState *gameState, const sf::Font &font, const sf::Vector2f &size)
	: _resourcesManager(resourcesManager), _font(font)
{
	this->_animations.push_back(new NewTurnAnimation(font, "invalid", 0, size));
	this->_animations.push_back(new MovementAnimation(resourcesManager, mapManager));
	this->_animations.push_back(new AttackMovementAnimation(resourcesManager, mapManager, gameState));
	this->_animations.push_back(new WinAnimation(resourcesManager, gameState, font, size));
	this->_currentAnimation = this->_animations.end();
}

AnimationManager::~AnimationManager()
{
	std::vector<IAnimation *>::iterator iter = this->_animations.begin();
	std::vector<IAnimation *>::iterator iter2 = this->_animations.end();

	while (iter != iter2)
	{
		delete (*iter);
		++iter;
	}
	this->_animations.clear();
}

void AnimationManager::play(AnimationManager::Infos infos)
{
	this->_currentAnimation = this->_animations.begin() + infos.type;
	if (infos.type == NEWTURN)
		reinterpret_cast<NewTurnAnimation *>((*this->_currentAnimation))->changeText(infos.player, infos.turns);
	if (infos.type == MOVEUNIT)
		reinterpret_cast<MovementAnimation *>((*this->_currentAnimation))->changePath(infos.playerPtr);
	if (infos.type == ATTACKMOVEUNIT)
		reinterpret_cast<AttackMovementAnimation *>((*this->_currentAnimation))->changePath(infos.playerPtr);
	if (infos.type == WINNING)
		reinterpret_cast<WinAnimation *>((*this->_currentAnimation))->setWinner(infos.player, infos.turns);
}

bool AnimationManager::update(const sf::Time &elapsedTime)
{
	if (!(*this->_currentAnimation)->update(elapsedTime))
		this->_currentAnimation = this->_animations.end();
	if (this->_currentAnimation == this->_animations.end())
		return (false);
	return (true);
}

void AnimationManager::draw(sf::RenderWindow &window) const
{
	if (this->_currentAnimation != this->_animations.end())
		window.draw(**this->_currentAnimation);
}
