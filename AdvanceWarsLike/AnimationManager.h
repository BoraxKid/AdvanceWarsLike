#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_
#pragma once

#include "ResourcesManager.h"
#include "Animation.h"

class AnimationManager
{
public:
	enum Type
	{
		NEWTURN = 0,
		MOVEUNIT = 1,
		ATTACKMOVEUNIT = 2,
		WINNING = 3
	};
	struct Infos
	{
		Infos(AnimationManager::Type ctype, const sf::String &cplayer, const sf::Uint32 &cturns)
			: type(ctype), player(cplayer), turns(cturns)
		{}
		Infos(AnimationManager::Type ctype, Player *cplayerPtr)
			: type(ctype), playerPtr(cplayerPtr)
		{}
		AnimationManager::Type type = NEWTURN;
		const sf::String &player = "";
		const sf::Uint32 &turns = 0;
		Player *playerPtr = nullptr;
	};
	AnimationManager(ResourcesManager &resourcesManager, MapManager &mapManager, GameState *gameState, const sf::Font &font, const sf::Vector2f &size);
	virtual ~AnimationManager();

	void play(AnimationManager::Infos infos);
	bool update(const sf::Time &elapsedTime);
	void draw(sf::RenderWindow &window) const;

private:
	ResourcesManager &_resourcesManager;
	const sf::Font &_font;
	std::vector<IAnimation *> _animations;
	std::vector<IAnimation *>::iterator _currentAnimation;
};

#endif // ANIMATIONMANAGER_H_
