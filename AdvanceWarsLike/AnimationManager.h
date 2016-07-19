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
		MOVEUNIT = 1
	};
	struct Infos
	{
		Infos(AnimationManager::Type ctype, const sf::String &cplayer, const sf::Uint32 &cturns)
			: type(ctype), player(cplayer), turns(cturns)
		{}
		Infos(AnimationManager::Type ctype, Player *cplayerPtr, void (Player::*cmovePtr)())
			: type(ctype), playerPtr(cplayerPtr), movePtr(cmovePtr)
		{}
		AnimationManager::Type type = NEWTURN;
		const sf::String &player = "";
		const sf::Uint32 &turns = 0;
		Player *playerPtr = nullptr;
		void (Player::*movePtr)() = nullptr;
	};
	AnimationManager(ResourcesManager &resourcesManager, MapManager &mapManager, const sf::Font &font, const sf::Vector2f &size);
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
