#include "Instruction.h"

Instruction::Instruction(RenderWindow& window)
: _window(window) {

	_SCREEN_WIDTH = _window.getSize().x;
	_SCREEN_HEIGHT = _window.getSize().y;

		if (!_font.loadFromFile("fonts/font.ttf")) {
			MessageBox(NULL, "Font not fountd", "Menu.cpp", NULL);
			return;
		}

		if (!_bg.loadFromFile("graphics/BG.png")) {
			MessageBox(NULL, "Graphics of background not found!", "Menu.cpp", NULL);
			return;
		}

		if (!_cur.loadFromFile("graphics/cursor.png")) {
			MessageBox(NULL, "Graphics of cursor not found!", "Menu.cpp", NULL);
			return;
		}

	_background.setTexture(_bg);
	_background.setScale(Vector2f(float(_SCREEN_WIDTH) / 911, float(_SCREEN_WIDTH) / 911));

	_cursor.setTexture(_cur);

	_state = PAGE_1;
	runInstruction();
}

void Instruction::runInstruction() {
	while (_state != END) {
		switch (_state) {
			case PAGE_1:
				showFirstPage();
				break;

			case PAGE_2:
				showSecondPage();
				break;

			case PAGE_3:
				showThirdPage();
				break;

			default:
				_state = END;
				break;
		}
	}
}

void Instruction::showFirstPage() {
	Texture txt;

		if (!txt.loadFromFile("graphics/instruction_1.png")) {
			MessageBox(NULL, "Instruction not fount!", "Menu.cpp", NULL);
			return;
		}

	Sprite texture;
	texture.setTexture(txt);
	texture.setScale(_SCREEN_WIDTH / texture.getGlobalBounds().width, _SCREEN_HEIGHT / texture.getGlobalBounds().height);
	texture.setPosition(_SCREEN_WIDTH / 2 - texture.getGlobalBounds().width / 2, _SCREEN_HEIGHT / 2 - texture.getGlobalBounds().height / 2);

	while (_state == PAGE_1) {
		_mouse = Vector2f(Mouse::getPosition());
		Event event;

			while (_window.pollEvent(event))
				if (event.type == Event::Closed ||
					(event.type == Event::KeyPressed &&
						event.key.code == Keyboard::Escape))
					_state = END;

				else if (_mouse.x >= _SCREEN_WIDTH / 2 &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = PAGE_2;

				else if (_mouse.x < _SCREEN_WIDTH / 2 &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = END;

		updateBackground();
		_cursor.setPosition(_mouse);

		_window.clear(Color::White);
		_window.setView(_window.getDefaultView());
		_window.setVerticalSyncEnabled(true);

		_window.draw(_background);
		_window.draw(texture);

		_window.draw(_cursor);

		_window.display();
	}
}

void Instruction::showSecondPage() {
	Texture txt;

		if (!txt.loadFromFile("graphics/instruction_2.png")) {
			MessageBox(NULL, "Instruction_2 not found!", "Instruction.cpp", NULL);
			return;
		}

	Sprite texture;
	texture.setTexture(txt);
	texture.setScale(_SCREEN_WIDTH / texture.getGlobalBounds().width, _SCREEN_HEIGHT / texture.getGlobalBounds().height);
	texture.setPosition(_SCREEN_WIDTH / 2 - texture.getGlobalBounds().width / 2, _SCREEN_HEIGHT / 2 - texture.getGlobalBounds().height / 2);

	while (_state == PAGE_2) {
		_mouse = Vector2f(Mouse::getPosition());
		Event event;

			while (_window.pollEvent(event))
				if (event.type == Event::Closed ||
					(event.type == Event::KeyPressed &&
						event.key.code == Keyboard::Escape))
					_state = END;

				else if (_mouse.x >= _SCREEN_WIDTH / 2 &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = PAGE_3;

				else if (_mouse.x < _SCREEN_WIDTH / 2 &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = PAGE_1;

		updateBackground();
		_cursor.setPosition(_mouse);

		_window.clear(Color::White);
		_window.setView(_window.getDefaultView());
		_window.setVerticalSyncEnabled(true);

		_window.draw(_background);
		_window.draw(texture);

		_window.draw(_cursor);

		_window.display();
	}
}

void Instruction::showThirdPage() {
	Texture txt;

		if (!txt.loadFromFile("graphics/instruction_3.png")) {
			MessageBox(NULL, "Instruction_3 not found!", "Instruction.cpp", NULL);
			return;
		}

	Sprite texture;
	texture.setTexture(txt);
	texture.setScale(_SCREEN_WIDTH / texture.getGlobalBounds().width, _SCREEN_HEIGHT / texture.getGlobalBounds().height);
	texture.setPosition(_SCREEN_WIDTH / 2 - texture.getGlobalBounds().width / 2, _SCREEN_HEIGHT / 2 - texture.getGlobalBounds().height / 2);

	while (_state == PAGE_3) {
		_mouse = Vector2f(Mouse::getPosition());
		Event event;

			while (_window.pollEvent(event))
				if (event.type == Event::Closed ||
					(event.type == Event::KeyPressed &&
						event.key.code == Keyboard::Escape))
					_state = END;

				else if (_mouse.x >= _SCREEN_WIDTH / 2 &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = END;

				else if (_mouse.x < _SCREEN_WIDTH / 2 &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = PAGE_2;

			updateBackground();
			_cursor.setPosition(_mouse);

			_window.clear(Color::White);
			_window.setView(_window.getDefaultView());
			_window.setVerticalSyncEnabled(true);

			_window.draw(_background);
			_window.draw(texture);

			_window.draw(_cursor);

			_window.display();
	}
}

void Instruction::updateBackground() {
	_background.setPosition(Vector2f((_mouse.x - _SCREEN_WIDTH / 2 - (_SCREEN_WIDTH / 2)) / 30,
								     (_mouse.y - _SCREEN_WIDTH - (_SCREEN_HEIGHT / 2)) / 30));
}

Instruction::~Instruction() {

}