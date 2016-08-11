#include "LoadScreen.h"

LoadScreen::LoadScreen(RenderWindow& window) 
	: _window(window) {

	_SCREEN_WIDTH = _window.getSize().x;
	_SCREEN_HEIGHT = _window.getSize().y;

		if (!_bg.loadFromFile("graphics/logo.jpg")) {
			MessageBox(NULL, "Graphics of background not found!", "LoadScreen.cpp", NULL);
			return;
		}

	_background.setTexture(_bg);
	_background.setScale(Vector2f(float(_SCREEN_WIDTH) / 1200, float(_SCREEN_WIDTH) / 1200));
	_background.setPosition(-(_background.getGlobalBounds().width / 20), -(_background.getGlobalBounds().height / 15));
}

void LoadScreen::showLoadScreen() const {
	_window.clear();

	_window.draw(_background);

	_window.display();
}

LoadScreen::~LoadScreen() {
	
}