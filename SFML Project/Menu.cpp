#include "Menu.h"

Menu::Menu(RenderWindow& window) 
: _window(window) {

	_SCREEN_WIDTH = window.getSize().x;
	_SCREEN_HEIGHT = window.getSize().y;

		if (!_font.loadFromFile("fonts/font.ttf")) {
			MessageBox(NULL, "Font not fountd", "Menu.cpp", NULL);
			return;
		}

		if (!_bg.loadFromFile("graphics/BG.png")) {
			MessageBox(NULL, "Graphics of background not found!", "Menu.cpp", NULL);
			return;
		}

		if (!_face.loadFromFile("graphics/deal2.png")) {
			MessageBox(NULL, "Graphics of 'deal with it' not found!", "Menu.cpp", NULL);
			return;
		}

		if (!_cur.loadFromFile("graphics/cursor.png")) {
			MessageBox(NULL, "Graphics of cursor not found!", "Menu.cpp", NULL);
			return;
		}

	_background.setTexture(_bg);
	_background.setScale(Vector2f(float(_SCREEN_WIDTH) / 911, float(_SCREEN_WIDTH) / 911));

	_deal.setTexture(_face);
	_deal.setPosition(_SCREEN_WIDTH / 13, _SCREEN_HEIGHT);
	_deal.setScale(Vector2f(0.5, 0.5));

	_cursor.setTexture(_cur);

	_showFace = false;
	_rand = 2;

	_loader = new Loader(_window);
	_fileManager = new FileManager(_window);

	_clock.restart();

	_state = MENU;
}

void Menu::runMenu() {
	while (_state != END) 
		switch (_state) {
			case MENU:
				showMenu();
				break;

			case NEW:
				newMap();
				break;

			case LOAD: {
				string name = "";
				_loader->runLoader(&name);

					if (name != "") {
						Editor* editor = new Editor(_window, _SCREEN_WIDTH, _SCREEN_HEIGHT, (char*)name.c_str());

						_state = MENU;
					}

					else 
						_state = MENU;

				}break;

			case INSTRUCTION: 
				Instruction* instruction = new Instruction(_window);
				_state = MENU;
				break;

			_showFace = false;
		}
}

void Menu::showMenu() {
	Text title("Map editor", _font, _SCREEN_HEIGHT / 10);
	title.setPosition(_SCREEN_WIDTH / 2 - title.getGlobalBounds().width / 2, (_SCREEN_HEIGHT / 22) * (_SCREEN_WIDTH / _SCREEN_HEIGHT));
	title.setColor(Color::Black);

	const int count = 4;
	Text text[count];

	String str[] = { "Create a new map", "Load exist map", "Instruction",  "Exit" };
	for (int i = 0; i < count; i++) {
		text[i].setFont(_font);
		text[i].setCharacterSize(_SCREEN_HEIGHT / 12);

		text[i].setString(str[i]);
		text[i].setPosition(_SCREEN_WIDTH / 2 - text[i].getGlobalBounds().width / 2, (_SCREEN_HEIGHT / 4) + (i * (_SCREEN_HEIGHT / 6.4)));
	}


		while (_state == MENU) {
			_mouse = Vector2f(Mouse::getPosition());
			Event event;

			while (_window.pollEvent(event)) 
				if (event.type == Event::Closed ||
					(event.type == Event::KeyPressed &&
					event.key.code == Keyboard::Escape)) 
					_state = END;

				else if (text[0].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) 
					_state = NEW;

				else if (text[1].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) 
					_state = LOAD;

				else if (text[2].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left)
					_state = INSTRUCTION;

				else if (text[3].getGlobalBounds().contains(_mouse) &&
					event.type == Event::MouseButtonPressed &&
					event.key.code == Mouse::Left) 
					_state = END;

					for (int i = 0; i < count; i++) 
						if (text[i].getGlobalBounds().contains(_mouse)) 
							text[i].setColor(Color::Red);
						else 
							text[i].setColor(Color::Black);

					if (_clock.getElapsedTime() > seconds(_rand)) {
						_showFace = !_showFace;

						_rand = rand() % 10 + 10;
						_clock.restart();
					}

				updateBackground();

			_cursor.setPosition(_mouse);

			_window.clear();
			_window.setView(_window.getDefaultView());

			_window.draw(_background);
			_window.draw(title);

				for (int i = 0; i < count; i++)
					_window.draw(text[i]);

			_window.draw(_deal);
			_window.draw(_cursor);

			_window.display();
		}
}

void Menu::updateBackground() {
	_background.setPosition(Vector2f((_mouse.x - _SCREEN_WIDTH / 2 - (_SCREEN_WIDTH / 2)) / 30,
		                             (_mouse.y - _SCREEN_WIDTH - (_SCREEN_HEIGHT / 2)) / 30));

		if (_showFace && _deal.getPosition().y > _SCREEN_HEIGHT - _deal.getGlobalBounds().height)
				_deal.move(0, -10);

		else if (!_showFace && _deal.getPosition().y < _SCREEN_HEIGHT)
				_deal.move(0, 0.5);
}

void Menu::newMap() {
	Editor* editor = new Editor(_window, _SCREEN_WIDTH, _SCREEN_HEIGHT, NULL);

	_state = MENU;
}

Menu::~Menu(void) {
	delete _fileManager;
	delete _loader;
}