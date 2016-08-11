#pragma once
#ifndef _LOADER_H_
#define _LOADER_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "FileManager.h"
#include "LoadScreen.h"

using namespace sf;

class Loader{
	private:
		int _SCREEN_WIDTH, _SCREEN_HEIGHT;
		int _element;

		RenderWindow& _window;
		CircleShape _shape[2];

		Font _font;
		Vector2f _mouse;

		Texture _bg, _cur;
		Sprite _background, _cursor;

		FileManager* _manager;

		enum State { NONE, LESS, MORE, END};
		State _state;

	public:
		Loader(RenderWindow& _window);

			void init();
			void runLoader(string*);

			void showNone();
			string showLess(deque<string>);
			string showMore(deque<string>);

			void updateBackground();

		~Loader();
};

#endif