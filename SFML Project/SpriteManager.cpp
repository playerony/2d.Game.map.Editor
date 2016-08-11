#include "SpriteManager.h"

SpriteManager::SpriteManager() 
	: _id(1) {

	loadTextures("graphics/other.png", "graphics/graphics.png", "graphics/enemy.png");
}

void SpriteManager::loadTextures(const char* other, const char* terrain, const char* enemy) {
	sf::Sprite sprite;

	//////////////////////////////////////

		if (!_terrain.loadFromFile(terrain)) {
			MessageBox(NULL, "Texture of terrain not found", "SpriteManager.cpp", NULL);
			return;
		}

	sprite.setTexture(_terrain);
	sprite.setScale(sf::Vector2f(0.5, 0.5));
	_terrainLines[0] = _id;

		for (int i = 0; i < 7; i++) {
			sprite.setTextureRect(sf::IntRect(i * 128, 0, 128, 128));

			this->_sprite[_id++] = sprite;
		}

		for (int i = 0; i < 2; i++) {
			sprite.setTextureRect(sf::IntRect(i * 128, 128, 128, 128));

			this->_sprite[_id++] = sprite;
		}

	_terrainLines[1] = _id - 1;
	_otherLines[0] = _id;

	sprite.setTextureRect(sf::IntRect(256, 128, 128, 128));
	this->_sprite[_id++] = sprite;

	sprite.setTextureRect(sf::IntRect(384, 128, 256, 128));
	this->_sprite[_id++] = sprite;

	sprite.setTextureRect(sf::IntRect(640, 128, 256, 128));
	this->_sprite[_id++] = sprite;

		for (int i = 0; i < 6; i++) {
			sprite.setTextureRect(sf::IntRect(i * 128, 256, 128, 128));

			this->_sprite[_id++] = sprite;
		}

	sprite.setTextureRect(sf::IntRect(0, 768, 256, 128));
	this->_sprite[_id++] = sprite;

	sprite.setTextureRect(sf::IntRect(0, 384, 384, 384));
	this->_sprite[_id++] = sprite;

	sprite.setTextureRect(sf::IntRect(384, 384, 384, 384));
	this->_sprite[_id++] = sprite;

	_otherLines[1] = _id - 1;

	//////////////////////////////////////

	/*
		if (!_other.loadFromFile(other)) {
			MessageBox(NULL, "Texture of other not found", "SpriteManager.cpp", NULL);
			return;
		}

	sprite.setTexture(_other);
	sprite.setScale(1, 1);

		for (int i = 0; i < 2; i++) {
			sprite.setTextureRect(sf::IntRect(i * 64, 0, 64, 64));

			this->_sprite[_id++] = sprite;
		}
		*/

	//////////////////////////////////////

	sprite.setScale(sf::Vector2f(1, 1));

		if (!_enemy.loadFromFile(enemy)) {
			MessageBox(NULL, "Texture of enemy not found", "SpriteManager.cpp", NULL);
			return;
		}

	sprite.setTexture(_enemy);
	sprite.setScale(sf::Vector2f(0.5, 0.5));

	_enemyLines[0] = _id;

		for (int i = 0; i < 2; i++) {
			sprite.setTextureRect(sf::IntRect(i * 256, 0, 256, 128));

			this->_sprite[_id++] = sprite;
		}

	_enemyLines[1] = _id - 1;

	//////////////////////////////////////
}

const sf::Sprite SpriteManager::getSprite(const int id) const {
	return this->_sprite.at(id);
}

const int SpriteManager::getID() const {
	return this->_id;
}

const int SpriteManager::getDownOther() const {
	return this->_otherLines[0];
}

const int SpriteManager::getUpOther() const {
	return this->_otherLines[1];
}

const int SpriteManager::getDownTerrain() const {
	return this->_terrainLines[0];
}

const int SpriteManager::getUpTerrain() const {
	return this->_terrainLines[1];
}

const int SpriteManager::getDownEnemy() const {
	return this->_enemyLines[0];
}

const int SpriteManager::getUpEnemy() const {
	return this->_enemyLines[1];
}

SpriteManager::~SpriteManager() {
	_sprite.clear();

	delete[] _otherLines;
	delete[] _enemyLines;
	delete[] _terrainLines;
}