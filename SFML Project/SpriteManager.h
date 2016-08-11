#pragma once
#ifndef _SPRITE_MANAGER_H_
#define _SPRITE_MANAGER_H_

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <map>

class SpriteManager {
	private:
		std::map<int, sf::Sprite> _sprite;

		sf::Texture _other, 
					_terrain, 
					_enemy;

		int _otherLines[2],
			_enemyLines[2],
			_terrainLines[2];

		int _id;

	public:
		SpriteManager(void);

			void loadTextures(const char*, const char*, const char*);

			const sf::Sprite getSprite(const int id) const;
			const int getID() const;

			const int getDownTerrain() const;
			const int getUpTerrain() const;
			const int getDownEnemy() const;
			const int getUpEnemy() const;
			const int getDownOther() const;
			const int getUpOther() const;

		~SpriteManager();
};

#endif