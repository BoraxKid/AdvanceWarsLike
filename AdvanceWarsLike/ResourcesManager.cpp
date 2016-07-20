#include "ResourcesManager.h"

ResourcesManager::ResourcesManager()
{
	this->loadSprites("resources/tiles.xml");
	this->loadSprites("resources/units.xml");
}

ResourcesManager::~ResourcesManager()
{
}

void ResourcesManager::update(const sf::Time &elapsed)
{
	std::map<sf::String, AnimatedSprite>::iterator iter = this->_sprites.begin();
	std::map<sf::String, AnimatedSprite>::iterator iter2 = this->_sprites.end();

	while (iter != iter2)
	{
		iter->second.update(elapsed);
		++iter;
	}
}

void ResourcesManager::loadTexture(sf::String textureName)
{
	if (this->_textures.find(textureName) == this->_textures.end() && !this->_textures[textureName].loadFromFile("resources/" + textureName))
		std::cerr << "Texture " << textureName.toAnsiString() << " couldn't be loaded" << std::endl;
}

void ResourcesManager::loadSprites(const char *file)
{
	pugi::xml_document doc;
	pugi::xml_node type;
	pugi::xml_node data;
	pugi::xml_node elem;
	sf::Uint16 id;
	sf::String textureName;
	sf::String spriteName;

	doc.load_file(file);
	type = doc.child("type");
	while (type)
	{
		id = type.attribute("id").as_uint();
		textureName = type.attribute("texture").as_string();
		this->loadTexture(textureName);
		data = type.first_child();
		while (data)
		{
			spriteName = data.first_attribute().as_string();
			this->_sprites[spriteName].setTexture(&this->_textures.at(textureName));
			elem = data.first_child();
			while (elem)
			{
				this->_sprites[spriteName].addFrame(sf::IntRect(elem.attribute("x").as_uint(), elem.attribute("y").as_uint(), elem.attribute("width").as_uint(), elem.attribute("height").as_uint()));
				if (spriteName.find("move_left") != sf::String::InvalidPos)
				{
					this->_sprites.at(spriteName).setScale(static_cast<float>(-1), static_cast<float>(1));
					this->_sprites.at(spriteName).setOrigin(static_cast<float>(elem.attribute("width").as_uint()), static_cast<float>(0));
				}
				elem = elem.next_sibling();
			}
			data = data.next_sibling();
		}
		type = type.next_sibling();
	}
}

AnimatedSprite &ResourcesManager::at(const sf::String &spriteName)
{
	return (this->_sprites.at(spriteName));
}
