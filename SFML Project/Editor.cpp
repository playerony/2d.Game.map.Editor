#include "Editor.h"

Editor::Editor(RenderWindow& window, const int width, const int height, char* path) 
	: _window(window), _SCREEN_WIDTH(width), _SCREEN_HEIGHT(height) {

	_sizeBlock = 63;
	_sizeX = 100;
	_sizeY = 18;
	_id = 1;
	_minID = _maxID = _id;
	_fps = _frame = 0;

	_add = false;
	_draw = false;
	_move = false;
	_mode = false;
	_fill = false;
	_area = false;
	_box = false;
	_lines = true;
	_delete = false;
	_addBlock = false;
	_press = false;
	_clear = false;
	
	_hud = new Hud(_window);
	_fileManager = new FileManager(_window);
	_exit = new Exit(_window);
	_manager = new SpriteManager();

		if (!_font.loadFromFile("fonts/font.ttf")) {
			MessageBox(NULL, "Font not fount!", "Editor.cpp", NULL);
			return;
		}

		if (!_cur.loadFromFile("graphics/cursor_1.png")) {
			MessageBox(NULL, "Graphics of cursor not found!", "Menu.cpp", NULL);
			return;
		}

		if (path != NULL) 
			_segment = _fileManager->loadFromFile(path, &_sizeX, &_sizeY);

	_clock.restart();

	line[0].setSize(Vector2f(_SCREEN_WIDTH * 2, 1));
	line[1].setSize(Vector2f(1, _sizeY*_sizeBlock));

	_cursor.setTexture(_cur);

	restartView();
	runEditor();
}

void Editor::runEditor() {
	while (_state != END) {
		switch (_state) {
			case EDITOR:
				showEditor();
				break;

			case INSTRUCTION: {
				Instruction* instruction = new Instruction(_window);
				_state = EDITOR;

				delete instruction;
			}break;

			case CLOSE:
				if (_exit->exitMenu())
					_state = SAVE;

				else
					_state = EDITOR;
				break;

			case SAVE:
				_exit->saveMenu(_segment, _sizeX, _sizeY);

				_state = END;
				break;

			case FAST_SAVE:
				_fileManager->saveToFile("fastSave_000000000", _segment, Vector2f(_sizeX, _sizeY));

				_state = EDITOR;
				break;

			default:
				_state = END;
				break;
		}
	}
}

void Editor::showEditor() {
	_maxID = _manager->getID() - 1;

	Vector2f center(_SCREEN_WIDTH / 2, _SCREEN_HEIGHT / 2);

		while (_state == EDITOR) {
			_pixelPos = Mouse::getPosition(_window);
			_mouse = _window.mapPixelToCoords(_pixelPos);

			Event event;
			while (_window.pollEvent(event)) {
				switch (event.type) {
					case Event::KeyPressed:
						if (event.key.code == Keyboard::Escape) 
							_state = CLOSE;

						else if (event.key.code == Keyboard::LControl)
							_move = true;

						else if (event.key.code == Keyboard::A && _fill)
							_add = true;

						else if (event.key.code == Keyboard::F1)
							_state = INSTRUCTION;

						else if (event.key.code == Keyboard::F5)
							_state = FAST_SAVE;

						else if (event.key.code == Keyboard::LAlt) {
							_fill = !_fill;

							if (!_fill)
								_area = false;

							else {
								_selectBox[0].x = _selectPos[0] * _sizeBlock;
								_selectBox[0].y = _selectPos[1] * _sizeBlock;
							}
						}

						else if (event.key.code == Keyboard::Space && _fill && !_hud->getMode())
							_press = true;

						else if (event.key.code == Keyboard::Up && _fill)
							_moveArea = UP;

						else if (event.key.code == Keyboard::Down && _fill)
							_moveArea = DOWN;

						else if (event.key.code == Keyboard::Left && _fill)
							_moveArea = LEFT;

						else if (event.key.code == Keyboard::Right && _fill)
							_moveArea = RIGHT;
					
						else if (event.key.code == Keyboard::N)
							_lines = !_lines;

						else if (event.key.code == Keyboard::B)
							restartView();

						else if (event.key.code == Keyboard::Delete && _delete && !_hud->getMode()) {
							int index = checkIn(_deletePos[0], _deletePos[1]);

							if (index != -1)
								_segment.erase(_segment.begin() + index);
						}

						else if (event.key.code == Keyboard::Delete && !_clear && _fill && !_hud->getMode())
							_clear = true;

						else if (event.key.code == Keyboard::Delete && _hud->getMode() && _part.size() > 0) {
							_part.erase(_part.begin() + _id);

							if (_hud->getMode() && _part.size() > 0) {
								_maxID--;
								_id--;
							}

							else
								_maxID = 0;
						}

						break;

					case Event::KeyReleased:
						if (event.key.code == Keyboard::LControl)
							_move = false;
						break;

					case Event::MouseButtonPressed:
						if (event.key.code == Mouse::Left && !_move && !_fill && !_hud->getMode() && _box)
							_draw = true;
						
						else if (event.key.code == Mouse::Left && _hud->getMode() && _box)
							_addBlock = true;

						else if (event.key.code == Mouse::Right && _delete) {
							int index = checkIn(_deletePos[0], _deletePos[1]);

							if (index != -1)
								_segment.erase(_segment.begin() + index);
						}

						else if (event.key.code == Mouse::Left && _fill) 
							_selectBox[0] = Vector2f(_mouse.x, _mouse.y);

						if(_hud->events(event, &_maxID, &_minID, _manager))
							_id = _minID;

						if (_hud->getMode() && _part.size() > 0)
							_maxID = _part.size() - 1;

						_hud->sizeEvent(event, &_sizeX, &_sizeY);
						break;

					case Event::MouseButtonReleased:
						if (event.key.code == Mouse::Left && !_move) 
							_draw = false;
						break;

					case Event::MouseWheelScrolled:
						if (event.mouseWheelScroll.delta > 0.7 && !_move) 
							_scroll = GORA;
						
						else if (event.mouseWheelScroll.delta < -0.7 && !_move)
							_scroll = DOL;
					
						else if (event.mouseWheelScroll.delta > 0.7 && _move) {
							if (_window.getView().getSize().x / _SCREEN_WIDTH > 1) {
								_view = _window.getView();
								_view.zoom(0.9f);
								_window.setView(_view);

								line[0].setOutlineThickness(1);
								line[1].setOutlineThickness(1);
							}
						}

						else if (event.mouseWheelScroll.delta < -0.7 && _move) 
							if (_window.getView().getSize().x / _SCREEN_WIDTH < 1.9) {
								_view = _window.getView();
								_view.zoom(1.1f);
								_window.setView(_view);

								float t = _window.getView().getCenter().x / _window.getDefaultView().getCenter().x;
								line[0].setOutlineThickness(t);
								line[1].setOutlineThickness(t);
							}

						_hud->sizeEvent(event, &_sizeX, &_sizeY);
						break;
						}
					}

					update();

					_frame++;

					if (_clock.getElapsedTime().asMilliseconds() > 999) {
						_fps = _frame;
						_frame = 0;
						_clock.restart();
					}

					boxCollision(_hud);

				_cursor.setPosition(_mouse);

				_window.clear(Color(224, 255, 255));
				_window.setVerticalSyncEnabled(true);
				_window.setView(_view);

				drawScene();
				drawSegment();

				drawFillBox();

				_hud->draw(_sizeX, _sizeY, _part.size());
				_hud->drawInformation(_fps, _minID, _maxID);

				_window.draw(_cursor);

				_window.display();
		}
}

void Editor::update() {
	if (_move) {
		Mouse::setPosition(Vector2i(_SCREEN_WIDTH / 2, _SCREEN_HEIGHT / 2), _window);

		_view.setCenter(_mouse);
	}

	scrollEvents();

	if (_press && !_hud->getMode()) {
		for (int i = 0; i < _sizeY; i++)
			for (int j = _render[0]; j < _render[1]; j++)
				if (checkPosition(j, i) == -1 && _square.contains(j*_sizeBlock, i*_sizeBlock))
					_segment.push_back(new Block(j, i, _id));

		_press = false;
		_fill = false;
		_area = false;
	}

	if (_addBlock && _hud->getMode() && _minID < _part.size()) {
		for (int i = 0; i < _part[_id].size(); i++)
			if(_part[_id][i]->getY() + _boxPos[1] < _sizeY)
				_segment.push_back(new Block(_part[_id][i]->getX() + _boxPos[0], _part[_id][i]->getY() + _boxPos[1], _part[_id][i]->getID()));

		_addBlock = false;
	}

	if (_add && !_hud->getMode() && _part.size() < 10) {
		int maxX = 0, maxY = 0;
		int minX = _sizeX, minY = _sizeY;

		for (int i = 0; i <= _sizeY; i++)
			for (int j = _render[0]; j <= _render[1]; j++)
				if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock)) {
					if (j > maxX)
						maxX = j;

					else if (j < minX)
						minX = j;

					if (i > maxY)
						maxY = i;

					else if (i < minY)
						minY = i;
				}

		if (minX != 0 || maxX != 0 || minY != 0 || maxY != 0) {
			deque<Block*> volat;

			for (int i = 0; i < _sizeY; i++)
				for (int j = _render[0]; j < _render[1]; j++)
					if (checkPosition(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock))
						volat.push_back(new Block(j - minX, i - minY, checkPosition(j, i)));


			_part.push_back(volat);

			volat.clear();

			_add = false;
			_maxID = _part.size();
		}
	}

	if (_clear) {
		for (int i = 0; i < _sizeY; i++)
			for (int j = _render[0]; j < _render[1]; j++)
				if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock)) {
					int index = checkIn(j, i);

					if (index != -1)
						_segment.erase(_segment.begin() + index);
				}

		_clear = false;
		_fill = false;
		_area = false;
	}

	moveBlocks();

	if (_draw && !_hud->getMode() && (_lastPos[0] != _boxPos[0] || _lastPos[1] != _boxPos[1]) && _box) {

		_segment.push_back(new Block(_boxPos[0], _boxPos[1], _id));

		_lastPos[0] = _boxPos[0];
		_lastPos[1] = _boxPos[1];
	}
}

void Editor::moveBlocks() {
	if (_fill && !_hud->getMode())
		switch (_moveArea) {
			case UP: {
				bool check = true;

				for (int i = 0; i < _sizeY; i++)
					for (int j = _render[0]; j < _render[1]; j++)
						if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock) && i - 1 < 0) {
							check = false;
							break;
						}

				if (check)
					for (int i = 0; i < _sizeY; i++)
						for (int j = _render[0]; j < _render[1]; j++)
							if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock))
								_segment[checkIn(j, i)]->setY(_segment[checkIn(j, i)]->getY() - 1);

				_moveArea = NOONE;
			}break;

			case DOWN: {
				bool check = true;

				for (int i = _sizeY; i >= 0; i--)
					for (int j = _render[0]; j < _render[1]; j++)
						if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock) && i + 1 > _sizeY - 1) {
							check = false;
							break;
						}

				if(check)
					for (int i = _sizeY; i >= 0; i--)
						for (int j = _render[0]; j < _render[1]; j++)
							if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock))
								_segment[checkIn(j, i)]->setY(_segment[checkIn(j, i)]->getY() + 1);

				_moveArea = NOONE;
			}break;

			case LEFT: {
				bool check = true;

				for (int i = 0; i < _sizeY; i++)
					for (int j = _render[0]; j < _render[1]; j++)
						if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock) && (j - 1 < 0 || j - 1 < _render[0])) {
							check = false;
							break;
						}

				if(check)
					for (int i = 0; i < _sizeY; i++)
						for (int j = _render[0]; j < _render[1]; j++)
							if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock))
								_segment[checkIn(j, i)]->setX(_segment[checkIn(j, i)]->getX() - 1);

				_moveArea = NOONE;
			}break;

			case RIGHT: {
				bool check = true;

				for (int i = 0; i < _sizeY; i++)
					for (int j = _render[1]; j >= _render[0]; j--)
						if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock) && (j + 1 > _sizeX || j + 1 > _render[1] - 1)) {
							check = false;
							break;
						}

				if(check)
					for (int i = 0; i < _sizeY; i++)
						for (int j = _render[1]; j >= _render[0]; j--)
							if (checkIn(j, i) != -1 && _square.contains(j*_sizeBlock, i*_sizeBlock))
								_segment[checkIn(j, i)]->setX(_segment[checkIn(j, i)]->getX() + 1);

				_moveArea = NOONE;
			}break;

			default:

				break;
		}
}

void Editor::drawSegment() {
	if (_part.size() > 0 && _hud->getMode() && !_fill)
		for (int i = 0; i < _part[_id].size(); i++) {
			Sprite sprite = _manager->getSprite(_part[_id][i]->getID());
			sprite.setPosition((_part[_id][i]->getX() + _boxPos[0]) * _sizeBlock, (_part[_id][i]->getY() + _boxPos[1]) * _sizeBlock);

			_window.draw(sprite);
		}
}

void Editor::drawScene() {
	drawLines();

		for (int i = 0; i < _segment.size(); i++) 
			if (_segment[i]->getX() * _sizeBlock > _view.getCenter().x - _SCREEN_WIDTH &&
				_segment[i]->getX() * _sizeBlock < _view.getCenter().x + _SCREEN_WIDTH) 
			{
				Sprite sprite = _manager->getSprite(_segment[i]->getID());
				sprite.setPosition(_segment[i]->getX() * _sizeBlock, _segment[i]->getY() * _sizeBlock);

				_window.draw(sprite);
			}

		if (_box && !_hud->getMode()) {
			Sprite sprite = _manager->getSprite(_id);
			sprite.setPosition(_boxPos[0] * _sizeBlock, _boxPos[1] * _sizeBlock);

			_window.draw(sprite);
		}
}

void Editor::drawLines() {
	if (_lines) {

		_render[0] = ((_window.getView().getCenter().x) / _sizeBlock) - (_SCREEN_WIDTH / _sizeBlock);
		_render[1] = ((_window.getView().getCenter().x) / _sizeBlock) - (_SCREEN_WIDTH / _sizeBlock) + ((_SCREEN_WIDTH/_sizeBlock) * 2);

		int value = ((_window.getView().getCenter().x)) - (_SCREEN_WIDTH);

			if (_render[1] > _sizeX) {
				_render[0] = _sizeX - ((_SCREEN_WIDTH * 2) / _sizeBlock);
				_render[1] = _sizeX;
			}

			else if (_render[0] < 0) {
				_render[0] = 0;
				_render[1] = ((_SCREEN_WIDTH * 2) / _sizeBlock);
			}
		
				for (int i = _render[0]; i <= _render[1]; i++) {
					line[1].setFillColor(Color(0, 0, 0));
					line[1].setPosition(i*_sizeBlock, 0);
					line[1].setOutlineColor(Color(0, 0, 0));

					_window.draw(line[1]);
				}

				for (int i = 0; i <= _sizeY; i++) {
					line[0].setFillColor(Color(0, 0, 0));

					if (value < 0) value = 0;
					else if (value + (_SCREEN_WIDTH*2) > _sizeX*_sizeBlock) value = (_sizeX*_sizeBlock) - (_SCREEN_WIDTH*2);

					line[0].setPosition(value, i*_sizeBlock);
					line[0].setOutlineColor(Color(0, 0, 0));

					_window.draw(line[0]);
				}
		}
}

void Editor::drawFillBox() {
	if (_fill) {
		_selectBox[1] = Vector2f((_selectPos[0] + 1) * _sizeBlock, (_selectPos[1] + 1) * _sizeBlock);

		RectangleShape lines[4];

		// 0 -> top
		// 1 -> bot
		// 2 -> left
		// 3 -> right

		lines[0].setPosition(_selectBox[0]);
		lines[0].setSize(Vector2f((_selectBox[1].x - _selectBox[0].x), 6));

		lines[1].setPosition(_selectBox[0].x, _selectBox[1].y);
		lines[1].setSize(Vector2f((_selectBox[1].x - _selectBox[0].x), 6));

		lines[2].setPosition(_selectBox[0].x, _selectBox[0].y);
		lines[2].setSize(Vector2f(6, (_selectBox[1].y - _selectBox[0].y)));

		lines[3].setPosition(_selectBox[1].x, _selectBox[0].y);
		lines[3].setSize(Vector2f(6, (_selectBox[1].y - _selectBox[0].y)));

		_square = IntRect(_selectBox[0].x, _selectBox[0].y, lines[0].getSize().x, lines[2].getSize().y);

		RectangleShape rect;
		rect.setPosition(_selectBox[0].x, _selectBox[0].y);
		rect.setSize(Vector2f(lines[0].getSize().x, lines[2].getSize().y));
		rect.setFillColor(Color(255, 255, 255, 30));

			_window.draw(rect);

			for (int i = 0; i < 4; i++)
				lines[i].setFillColor(Color::Red);

			for (int i = 0; i < 4; i++)
				_window.draw(lines[i]);
	}
}

void Editor::boxCollision(Hud* hud) {
	for (int i = _render[0]; i <= _render[1]; i++)
		for (int j = 0; j < _sizeY; j++) {
			IntRect rect(i*_sizeBlock, j*_sizeBlock, _sizeBlock, _sizeBlock);

				if (rect.contains(_mouse.x, _mouse.y) && checkPosition(i, j) == -1 && _sizeX != i) {
					_box = true;
					_delete = false;

					_boxPos[0] = i;
					_boxPos[1] = j;

					_selectPos[0] = i;
					_selectPos[1] = j;
				}

				else if (rect.contains(_mouse.x, _mouse.y) && checkPosition(i, j) != -1) {
					_box = false;
					_delete = true;

					_deletePos[0] = i;
					_deletePos[1] = j;

					_selectPos[0] = i;
					_selectPos[1] = j;
				}

				else if (_mouse.x > _sizeX*_sizeBlock || _mouse.y > _sizeY*_sizeBlock || _mouse.x < 0 || _mouse.y < 0 ||
					    (hud->getRectangle().getGlobalBounds().contains(_mouse) && hud->getVisible()) || _fill) {
					_draw = false;
					_box = false;
					_delete = false;
				}	
		}
}

void Editor::scrollEvents() {
	if (_maxID != _minID) 
		switch (_scroll) {
			case GORA:
				_id++;
				if (_id >= _maxID) 
					_id = _minID;

				_scroll = NEUTRAL;
				break;

			case DOL:
				_id--;
				if (_id <= _minID) 
					_id = _maxID;

				_scroll = NEUTRAL;
				break;
		}
}

void Editor::restartView() {
	_view.setCenter(Vector2f(_SCREEN_WIDTH / 2, _SCREEN_HEIGHT / 2));
	_view.setSize(Vector2f(_SCREEN_WIDTH, _SCREEN_HEIGHT));

	line[0].setOutlineThickness(1);
	line[1].setOutlineThickness(1);
}

int Editor::checkPosition(const int x, const int y) const {
	for (unsigned int i = 0; i < _segment.size(); i++)
		if (_segment[i]->getX() == x && _segment[i]->getY() == y)
			return _segment[i]->getID();
	return -1;
}

int Editor::checkIn(const int x, const int y) const {
	for (unsigned int i = 0; i < _segment.size(); i++)
		if (_segment[i]->getX() == x && _segment[i]->getY() == y)
			return i;
	return -1;
}

bool Editor::checkIndex(const int id) const {
	for (unsigned int i = 0; i < _segment.size(); i++)
		if (_segment[i]->getID() == id)
			return false;
	return true;
}

Editor::~Editor(void) {
	_segment.clear();
	delete _fileManager;
}