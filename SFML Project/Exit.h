#pragma once
#ifndef  _EXIT_H_
#define  _EXIT_H_

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <deque>

#include "FileManager.h"
#include "Block.h"

using namespace sf;

class Exit {
	private:
		int _SCREEN_WIDTH, _SCREEN_HEIGHT;
		bool _show;

		RenderWindow& _window;
		CircleShape _shape[2];

		Font _font;
		Vector2f _mouse;

		Texture _bg, _district, _bird, _cur;
		Sprite _background, _mark, _area, _cursor;

		Clock _clock;

		FileManager* _fileManager;

	public:
		Exit(RenderWindow&);

			void init();

			bool exitMenu();
			void saveMenu(deque<Block*>, const size_t, const size_t);

			void updateBackground();
			
		~Exit();
};

#endif // ! _EXIT_H_
