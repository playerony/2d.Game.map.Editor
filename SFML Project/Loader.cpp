#include "Loader.h"

Loader::Loader(RenderWindow& window) 
	: _window(window) {

	_SCREEN_WIDTH = window.getSize().x;
	_SCREEN_HEIGHT = window.getSize().y;

		if (!_font.loadFromFile("fonts/font.ttf")) {
			MessageBox(NULL, "Font not found", "Menu.cpp", NULL);
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

	_window.setMouseCursorVisible(false);

	_element = 10;

	init();
}

void Loader::init() {
	_manager = new FileManager(_window);

		for (int i = 0; i < 2; i++) {
			_shape[i].setRadius(15);
			_shape[i].setPointCount(3);
			_shape[i].setFillColor(Color::Black);
		}

	_shape[1].setRotation(180);

	_shape[0].setPosition(_SCREEN_WIDTH / 2 - _shape[0].getGlobalBounds().width / 2, _SCREEN_HEIGHT / 4);
	_shape[1].setPosition(_shape[0].getPosition().x + _shape[1].getGlobalBounds().width, _shape[0].getPosition().y + _SCREEN_HEIGHT / 2 + 25 + _shape[1].getGlobalBounds().width);
}

void Loader::runLoader(string* name) {
	deque<Text> texts;
	deque<string> names = _manager->getListOfFiles("./maps/");

	const int count = names.size();

		if (count <= 0)
			_state = NONE;
		else if (count <= 10 && count > 0)
			_state = LESS;
		else
			_state = MORE;

	switch (_state) {
		case NONE:
			showNone();
			*name = "";
			break;

		case LESS:
			*name = showLess(names);
			break;

		case MORE:
			*name = showMore(names);
			break;

		default:

			break;
	}
}

void Loader::showNone() {
	Text title("*.png files not found in ./maps/*", _font, 80);
	title.setFont(_font);
	title.setColor(Color::Black);
	title.setCharacterSize(_SCREEN_HEIGHT / 9);
	title.setPosition(_SCREEN_WIDTH / 2 - title.getGlobalBounds().width / 2,
					  _SCREEN_HEIGHT / 2 - title.getGlobalBounds().height / 2);

	bool run = true;

		while (run) {
			_mouse = Vector2f(Mouse::getPosition());
			Event event;

				while (_window.pollEvent(event)) 
					if (event.type == Event::Closed ||
					   (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) ||
					   (event.type == Event::KeyPressed && event.key.code == Keyboard::Return))
						run = false;

			updateBackground();

			_cursor.setPosition(_mouse);

			_window.clear();

			_window.setView(_window.getDefaultView());
			_window.draw(_background);
			_window.draw(title);
			_window.draw(_cursor);

			_window.display();
		}
}

string Loader::showLess(deque<string> names) {
	deque<Text> texts;
	names = _manager->getListOfFiles("./maps/");

	const int count = names.size();

	Text title("Files", _font, 80);
	title.setFont(_font);
	title.setColor(Color::Black);
	title.setCharacterSize(_SCREEN_HEIGHT / 9);
	title.setPosition(_SCREEN_WIDTH / 2 - title.getGlobalBounds().width / 2, 20);

	Text text;

		for (int i = 0; i < count; i++) {
			text.setFont(_font);
			text.setCharacterSize(30);

			text.setString(names[i]);
			text.setPosition(_SCREEN_WIDTH / 2 - text.getGlobalBounds().width / 2, 
				            (_SCREEN_HEIGHT / 3.5) + i * (_SCREEN_HEIGHT / 20));

			texts.push_back(text);
		}

	bool run = true;
	int index = 0;

	while (run) {
		_mouse = Vector2f(Mouse::getPosition());
		Event event;

		while (_window.pollEvent(event)) {
			if (event.type == Event::Closed ||
				(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) 
				run = false;

				for (int i = 0; i < count; i++)
					if (texts[i].getGlobalBounds().contains(_mouse) &&
						event.type == Event::MouseButtonPressed &&
						event.key.code == Mouse::Left) {

						if (_manager->isEmpty(names[index].c_str())) {
							MessageBox(NULL, "File is empty!", "Menu.cpp", NULL);
							return "";
						}

						else {
							run = false;
							return names[index];
						}
					}
			}

			for (int i = 0; i < count; i++)
				if (texts[i].getGlobalBounds().contains(_mouse))
					texts[i].setColor(Color::Red);
				else
					texts[i].setColor(Color::Black);

		updateBackground();

		_cursor.setPosition(_mouse);

		_window.clear();
		_window.setView(_window.getDefaultView());
		_window.draw(_background);
		_window.draw(title);

			for (int i = 0; i < count; i++)
				_window.draw(texts[i]);

		_window.draw(_cursor);

		_window.display();
	}

	texts.clear();

	return "";
}

string Loader::showMore(deque<string> names) {
	deque<Text> texts;
	names = _manager->getListOfFiles("./maps/");

	const int count = names.size();

	Text title("Files", _font, 80);
	title.setFont(_font);
	title.setColor(Color::Black);
	title.setCharacterSize(_SCREEN_HEIGHT / 9);
	title.setPosition(_SCREEN_WIDTH / 2 - title.getGlobalBounds().width / 2, _SCREEN_HEIGHT / 20);

	Text text;

		for (int i = 0; i < count; i++) {
			text.setFont(_font);
			text.setCharacterSize(30);

			text.setString(names[i]);
			text.setPosition(_SCREEN_WIDTH / 2 - text.getGlobalBounds().width / 2, (_SCREEN_HEIGHT / 3.5) + i * (_SCREEN_HEIGHT / 20));
			texts.push_back(text);
		}

	bool run = true;
	int index = 0;
	int value = 0;

	while (run) {
		_mouse = Vector2f(Mouse::getPosition());
		Event event;

		while (_window.pollEvent(event)) {
			if (event.type == Event::Closed ||
				(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) 
				run = false;

				int value = count;
				if (count > 10)
					value = 10;

				for (int i = 0; i < value; i++)
					if (texts[i].getGlobalBounds().contains(_mouse) &&
						event.type == Event::MouseButtonPressed &&
						event.key.code == Mouse::Left) {
						if (_element >= 10)
							index = i + (_element - 10);

						else if (i + (_element - 10) <= 0)
							index = i;

						if (_manager->isEmpty(names[index].c_str())) {
							MessageBox(NULL, "File is empty!", "Menu.cpp", NULL);
							return "";
						}

						else {
							run = false;
							return names[index];
						}
					}
			}

			if ((_shape[0].getGlobalBounds().contains(_mouse) && 
				event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) ||
				(event.type == Event::KeyPressed && event.key.code == Keyboard::Up)) {
				if (_element < count) {
					Text t = texts[0];
					for (int i = 0; i < texts.size() - 1; i++)
						texts[i] = texts[i + 1];

					texts[texts.size() - 1] = t;
					for (int i = 0; i < texts.size(); i++)
						texts[i].setPosition(_SCREEN_WIDTH / 2 - text.getGlobalBounds().width / 2, 
							                (_SCREEN_HEIGHT / 3.5) + i * (_SCREEN_HEIGHT / 20));

					_element++;
					value--;
				}
			}

			else if ((_shape[1].getGlobalBounds().contains(_mouse) &&
				event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) ||
				(event.type == Event::KeyPressed && event.key.code == Keyboard::Down)) {
				if (_element > 10) {
					Text t = texts[texts.size() - 1];
					for (int j = texts.size() - 1; j > 0; j--)
						texts[j] = texts[j - 1];

					texts[0] = t;
					for (int i = texts.size() - 1; i >= 0; i--)
						texts[i].setPosition(_SCREEN_WIDTH / 2 - text.getGlobalBounds().width / 2, (_SCREEN_HEIGHT / 3.5) + i * (_SCREEN_HEIGHT / 20));

					_element--;
					value++;
				}
			}

			for (int i = 0; i < 10; i++)
				if (texts[i].getGlobalBounds().contains(_mouse))
					texts[i].setColor(Color::Red);
				else
					texts[i].setColor(Color::Black);

		updateBackground();

		_cursor.setPosition(_mouse);

		_window.clear();
		_window.setView(_window.getDefaultView());
		_window.draw(_background);
		_window.draw(title);

			for (int i = 0; i < 2; i++)
				if (_shape[i].getGlobalBounds().contains(_mouse))
					_shape[i].setFillColor(Color::Red);
				else
			_shape[i].setFillColor(Color::Black);

			for (int i = 0; i < 10; i++)
				_window.draw(texts[i]);

			for (int i = 0; i < 2; i++)
				_window.draw(_shape[i]);

		_window.draw(_cursor);

		_window.display();
	}

	texts.clear();

	return "";
}

void Loader::updateBackground() {
	_background.setPosition(Vector2f((_mouse.x - _SCREEN_WIDTH / 2 - (_SCREEN_WIDTH / 2)) / 30,
									 (_mouse.y - _SCREEN_WIDTH - (_SCREEN_HEIGHT / 2)) / 30));

}

Loader::~Loader() {
	delete[] _shape;
	delete _manager;
}