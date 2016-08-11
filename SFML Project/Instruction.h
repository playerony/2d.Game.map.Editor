#pragma once
#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace sf;

class Instruction {
	private:
		int _SCREEN_WIDTH, _SCREEN_HEIGHT;

		RenderWindow& _window;
		Vector2f _mouse;
		Font _font;

		Texture _bg, _cur;
		Sprite _background, _cursor;

		enum State{PAGE_1, PAGE_2, PAGE_3, END};
		State _state;

	public:
		Instruction(RenderWindow&);

			void runInstruction();

			void updateBackground();

			void showFirstPage();
			void showSecondPage();
			void showThirdPage();

		~Instruction();
};

#endif // !_INSTRUCTION_H_

