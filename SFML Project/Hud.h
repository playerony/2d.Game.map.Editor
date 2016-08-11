#pragma once
#ifndef _HUD_H_
#define _HUD_H_

#include <SFML/Graphics.hpp>
#include <deque>
#include <math.h>

#include "SpriteManager.h"
#include "Button.h"
#include "Block.h"

using namespace sf;
using namespace std;

class Hud {
	private:
		int _SCREEN_WIDTH, _SCREEN_HEIGHT;

		deque<Button*> _button;

		RenderWindow& _window;
		CircleShape _shape[4];
		RectangleShape _rectangle;

		View _view;

		Text _text[3], _information[3];
		Font _font;

		bool _visible,
			 _mode;

		enum State {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5};
		State _event;

	public:
		Hud(RenderWindow&);

			void init();
			void initTriangles();
			bool events(Event, int*, int*, SpriteManager* manager);
			void sizeEvent(Event, int*, int*);
			void draw(const int, const int, const int);
			void drawInformation(const int, const int, const int);

			void setVisible(bool);
			bool getVisible() const;
			bool getMode() const;

			void setBorder();

			void update();

			void isVisible();

			const RectangleShape getRectangle() const;

		~Hud();
};

#endif
