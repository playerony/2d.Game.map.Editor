#pragma once
#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Windows.h>
#include <deque>
#include <string>

#include "SpriteManager.h"
#include "LoadScreen.h"
#include "Block.h"

using namespace std;
using namespace sf;

class FileManager {
	private:
		RenderWindow& _window;

		bool _encryption;

		Thread _thread;

		LoadScreen* _loadScreen;

	public:
		FileManager(RenderWindow&);

			void saveToFile(string, deque<Block*>&, Vector2f);

			deque<Block*> loadFromFile(const char*, int*, int*);
			deque<string> getListOfFiles(const char*) const;

			bool isEmpty(const char*) const;
			
			void setEncryption(bool);

			void loader();

		~FileManager(void);
};

#endif