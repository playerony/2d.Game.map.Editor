#pragma once
#ifndef _MENU_H_
#define _MENU_H_
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <iostream>

#include "FileManager.h"
#include "Instruction.h"
#include "Loader.h"
#include "Editor.h"

using namespace std;
using namespace sf;

class Menu {
	private:
		int _SCREEN_WIDTH, _SCREEN_HEIGHT;
		int _rand;

		bool _showFace;

		RenderWindow& _window;
		Vector2f _mouse;
		Font _font;

		Texture _bg, _face, _cur;
		Sprite _background, _deal, _cursor;

		Clock _clock;

		Loader* _loader;
		FileManager* _fileManager;

		enum State { MENU, NEW, LOAD, INSTRUCTION, END };
		State _state;

	public:
		Menu(RenderWindow&);

			void runMenu();
			void showMenu();

			void updateBackground();

			void newMap();

		~Menu(void);
};

#endif // ! _MENU_H_
