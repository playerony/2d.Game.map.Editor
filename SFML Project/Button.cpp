#include "Button.h"

Button::Button(RenderWindow& window, Vector2f size, Vector2f position, const char* text)
	: _window(window), _size(size), _position(position), _title(text) {

		if (!_font.loadFromFile("fonts/font.ttf")) {
			MessageBox(NULL, "Font not found!", "Button.cpp", NULL);
			return;
		}

	init();
}

void Button::init() {
	_rectangle.setPosition(_position);
	_rectangle.setSize(_size);

	_text.setFont(_font);
	_text.setString(_title);
	_text.setCharacterSize(_rectangle.getSize().y / 2);

	_text.setPosition(_position.x + _rectangle.getGlobalBounds().width / 2 - _text.getGlobalBounds().width / 2,
					  _position.y + _rectangle.getGlobalBounds().height / 2 - _text.getGlobalBounds().height / 2);
}

void Button::draw() const {
	_window.draw(_rectangle);
	_window.draw(_text);
}

void Button::setColor(const Color color) {
	_rectangle.setFillColor(color);
}

void Button::setBorder(bool state) {
	_rectangle.setOutlineColor(Color::Black);

		if (state) 
			_rectangle.setOutlineThickness(2);
		else
			_rectangle.setOutlineThickness(0);
}

const RectangleShape Button::getRectangle() const {
	return this->_rectangle;
}

Button::~Button(void) {
	
}