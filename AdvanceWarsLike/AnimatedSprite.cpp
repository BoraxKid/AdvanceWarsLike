#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const sf::Time frameTime)
	: _texture(0), _frameTime(frameTime)
{
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite &cpy)
	: _texture(cpy._texture), _frames(cpy._frames)
{
}

AnimatedSprite &AnimatedSprite::operator=(const AnimatedSprite &cpy)
{
	if (this != &cpy)
	{
		this->_texture = cpy._texture;
		this->_frames = cpy._frames;
		this->_sprites = cpy._sprites;
	}
	return (*this);
}

AnimatedSprite::~AnimatedSprite()
{
	this->_frames.clear();
}

void AnimatedSprite::update(const sf::Time &elapsed)
{
	this->_elapsedTime += elapsed;
	if (this->_elapsedTime >= this->_frameTime)
	{
		this->_elapsedTime = sf::microseconds(this->_elapsedTime.asMicroseconds() % this->_frameTime.asMicroseconds());
		if (this->_currentFrame + 1 != this->_sprites.end())
			++this->_currentFrame;
		else
			this->_currentFrame = this->_sprites.begin();
	}
}

void AnimatedSprite::setTexture(const sf::Texture *texture)
{
	this->_texture = texture;
	std::vector<sf::Sprite>::iterator iter = this->_sprites.begin();
	while (iter != this->_sprites.end())
	{
		iter->setTexture(*this->_texture);
		++iter;
	}
}

void AnimatedSprite::addFrame(sf::IntRect rect)
{
	this->_frames.push_back(rect);
	if (!this->_sprites.empty())
		this->_sprites.push_back(sf::Sprite(this->_sprites.back()));
	else
	{
		this->_sprites.push_back(sf::Sprite());
		if (this->_texture)
			this->_sprites.back().setTexture(*this->_texture);
	}
	this->_sprites.back().setTextureRect(rect);
	this->_currentFrame = this->_sprites.end() - 1;
}

void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(*this->_currentFrame, states);
}
