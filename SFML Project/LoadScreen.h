#pragma once
#ifndef _LOAD_SCREEN_H_
#define _LOAD_SCREEN_H_

#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace sf;

class LoadScreen {
	private:
		int _SCREEN_WIDTH, _SCREEN_HEIGHT;

		RenderWindow& _window;

		Texture _bg;
		Sprite _background;

	public:
		LoadScreen(RenderWindow&);

			void showLoadScreen() const;

		~LoadScreen();
};

#endif