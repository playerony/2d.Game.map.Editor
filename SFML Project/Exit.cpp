#include "Exit.h"

Exit::Exit(RenderWindow& window)
	: _window(window), _show(false){

	_SCREEN_WIDTH = _window.getSize().x;
	_SCREEN_HEIGHT = _window.getSize().y;

		if (!_font.loadFromFile("fonts/font.ttf")) {
			MessageBox(NULL, "Font not found", "Menu.cpp", NULL);
			return;
		}

		if (!_bg.loadFromFile("graphics/BG.png")) {
			MessageBox(NULL, "Graphics of background not found!", "Menu.cpp", NULL);
			return;
		}

		if (!_district.loadFromFile("graphics/area.png")) {
			MessageBox(NULL, "Graphics of background not found!", "Menu.cpp", NULL);
			return;
		}

		if (!_bird.loadFromFile("graphics/mark.png")) {
			MessageBox(NULL, "Graphics of background not found!", "Menu.cpp", NULL);
			return;
		}

		if (!_cur.loadFromFile("graphics/cursor.png")) {
			MessageBox(NULL, "Graphics of cursor not found!", "Menu.cpp", NULL);
			return;
		}

	_background.setTexture(_bg);
	_mark.setTexture(_bird);
	_area.setTexture(_district);
	_cursor.setTexture(_cur);

	_background.setScale(Vector2f(float(_SCREEN_WIDTH) / 911, float(_SCREEN_WIDTH) / 911));
	_mark.setScale(Vector2f(0.5, 0.5));
	_area.setScale(Vector2f(0.5, 0.5));

	init();
}

void Exit::init() {
	_clock.restart();

	_fileManager = new FileManager(_window);
}

bool Exit::exitMenu() {
	Text title("Are you sure to leave the editor ?", _font, 80);
	title.setPosition(_SCREEN_WIDTH / 2 - title.getGlobalBounds().width / 2, 70);
	title.setColor(Color::Black);

	const int count = 2;
	Text text[count];

	String str[] = { "Yes", "No" };

	for (int i = 0; i < count; i++) {
		text[i].setFont(_font);
		text[i].setCharacterSize(65);

		text[i].setString(str[i]);
		text[i].setPosition(_SCREEN_WIDTH / 2 - text[i].getGlobalBounds().width / 2 - text[i].getGlobalBounds().width - 50 + ((_SCREEN_WIDTH / 5) * i), _SCREEN_HEIGHT - (_SCREEN_HEIGHT / 3));
	}

	bool run = true;

		while (run) {
			_mouse = Vector2f(Mouse::getPosition());
			Event event;

			while (_window.pollEvent(event)) {
				if (event.type == Event::Closed ||
					(event.type == Event::KeyPressed &&
						event.key.code == Keyboard::Escape)) {
					return false;
				}

				else if (text[0].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) {
					return true;
				}

				else if (text[1].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) {
					return false;
				}
			}

			for (int i = 0; i < count; i++)
				if (text[i].getGlobalBounds().contains(_mouse))
					text[i].setColor(Color::Red);
				else
					text[i].setColor(Color::Black);

			updateBackground();
			_cursor.setPosition(_mouse);

			_window.clear();

			_window.setView(_window.getDefaultView());

			_window.draw(_background);
			_window.draw(title);

			for (int i = 0; i < count; i++)
				_window.draw(text[i]);

			_window.draw(_cursor);

			_window.display();
		}	
}

void Exit::saveMenu(deque<Block*> segment, const size_t _sizeX, const size_t _sizeY) {
	Text text[3];

	text[0].setFont(_font);
	text[0].setCharacterSize(_SCREEN_HEIGHT / 20);
	text[0].setColor(Color::Black);

	text[1].setFont(_font);
	text[1].setCharacterSize(_SCREEN_HEIGHT / 20);
	text[1].setColor(Color::Black);
	text[1].setString("Enter the file name: ");

	text[2].setFont(_font);
	text[2].setCharacterSize(_SCREEN_HEIGHT / 20);
	text[2].setColor(Color::Black);
	text[2].setString("Encrypt the file? ");


	RectangleShape shape;
	shape.setSize(Vector2f(_SCREEN_WIDTH / 3.9, _SCREEN_HEIGHT / 14));
	shape.setFillColor(Color::White);
	shape.setOutlineThickness(2);
	shape.setOutlineColor(Color::Black);

	RectangleShape line;
	line.setSize(Vector2f(2, _SCREEN_HEIGHT / 17.8));
	line.setFillColor(Color::Black);

	text[0].setPosition(_SCREEN_WIDTH / 2, (_SCREEN_HEIGHT / 2) - (_SCREEN_HEIGHT / 4));
	text[1].setPosition(text[0].getPosition().x - text[1].getGlobalBounds().width, text[0].getPosition().y);
	text[2].setPosition(text[0].getPosition().x - text[1].getGlobalBounds().width + _area.getGlobalBounds().width, 
						text[0].getPosition().y + _area.getGlobalBounds().height / 2 + text[2].getGlobalBounds().height * 2 - text[2].getGlobalBounds().height /2 + _SCREEN_HEIGHT / 38);

	shape.setPosition(text[0].getPosition());
	line.setPosition(shape.getPosition().x + 5, shape.getPosition().y + 5);

	_area.setPosition(text[0].getPosition().x - text[1].getGlobalBounds().width, text[0].getPosition().y + _area.getGlobalBounds().height + _SCREEN_HEIGHT / 38);
	_mark.setPosition(_area.getPosition().x - 20, _area.getPosition().y - 20);

	string string = "";

	bool draw = true;

	const int count = 2;
	Text txt[count];

	String str[] = { "Save", "Exit" };

		for (int i = 0; i < count; i++) {
			txt[i].setFont(_font);
			txt[i].setCharacterSize(_SCREEN_HEIGHT / 12);

			txt[i].setString(str[i]);
			txt[i].setPosition(_SCREEN_WIDTH / 2 - txt[i].getGlobalBounds().width / 2 - txt[i].getGlobalBounds().width - 25 + ((_SCREEN_WIDTH / 5.5) * i), _SCREEN_HEIGHT - (_SCREEN_HEIGHT / 3));
		}
	
	bool run = true;

		while (run)
		{
			_mouse = Vector2f(Mouse::getPosition());
			Event event;

			while (_window.pollEvent(event))
			{
				if (event.type == Event::Closed ||
					(event.type == Event::KeyPressed &&
						event.key.code == Keyboard::Escape)) {
					run = false;
				}

				else if (txt[0].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) {
					if (string.size() > 0) {
						_fileManager->setEncryption(_show);
						_fileManager->saveToFile(string, segment, Vector2f(_sizeX, _sizeY));
						run = false;
					}
					else
						run = true;
				}

				else if (txt[1].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) {
					run = false;
				}

				else if (_area.getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) {
					_show = !_show;
				}

				else if (event.type == Event::TextEntered)
				{
					sf::Uint32 unicode = event.text.unicode;

					if (unicode == 8 && string.size() > 0)
					{
						string.erase(string.size() - 1, 1);
						text[0].setString(string);

						line.setPosition(text[0].getPosition().x + text[0].getGlobalBounds().width + 4, line.getPosition().y);
					}

					else if (((unicode >= 48 && unicode <= 57) || (unicode >= 65 && unicode <= 90) ||
						(unicode >= 97 && unicode <= 122)) && string.size() < 15)
					{
						string += static_cast<char>(unicode);
						text[0].setString(string);

						line.setPosition(text[0].getPosition().x + text[0].getGlobalBounds().width + 4, line.getPosition().y);
					}
				}
			}

			if (_clock.getElapsedTime() > seconds(1.0) && string.size() < 15) {
				draw = !draw;
				_clock.restart();
			}

			else if (string.size() >= 15)
				draw = false;

			for (int i = 0; i < count; i++)
				if (txt[i].getGlobalBounds().contains(_mouse))
					txt[i].setColor(Color::Red);
				else
					txt[i].setColor(Color::Black);

			updateBackground();
			_cursor.setPosition(_mouse);

			_window.clear();
			_window.setMouseCursorVisible(false);

			_window.draw(_background);
			_window.draw(shape);
			_window.draw(text[1]);
			_window.draw(text[2]);
			_window.draw(text[0]);

			_window.draw(_area);

				if (_show)
					_window.draw(_mark);

				if (draw)
					_window.draw(line);

				for (int i = 0; i < count; i++)
					_window.draw(txt[i]);

			_window.draw(_cursor);

			_window.display();
		}
}

void Exit::updateBackground() {
	_background.setPosition(Vector2f((_mouse.x - _SCREEN_WIDTH / 2 - (_SCREEN_WIDTH / 2)) / 30,
									 (_mouse.y - _SCREEN_WIDTH - (_SCREEN_HEIGHT / 2)) / 30));
}

Exit::~Exit() {
	delete _fileManager;
	delete[] _shape;
}