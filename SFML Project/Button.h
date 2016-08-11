#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace sf;

class Button {
	private:
		const char* _title;

		RenderWindow& _window;
		RectangleShape _rectangle;

		Text _text;
		Font _font;

		Vector2f _size,
				 _position;

	public:
		Button(RenderWindow&, Vector2f, Vector2f, const char*);

			void init();
			void draw() const;

			void setColor(Color);
			void setBorder(bool);

			const RectangleShape getRectangle() const;

		~Button(void);
};

#endif