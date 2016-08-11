#pragma once
#include "Hud.h"

Hud::Hud(RenderWindow& window)
	: _window(window), _visible(false) {

	_SCREEN_WIDTH = _window.getSize().x;
	_SCREEN_HEIGHT = _window.getSize().y;

		if (!_font.loadFromFile("fonts/arial.ttf")) {
			MessageBox(NULL, "Font not found!", "Hud.cpp", NULL);
			return;
		}

	_mode = false;
	_event = BUTTON_4;

	init();
	setBorder();
}

void Hud::init() {
	_rectangle.setFillColor(Color(255, 255, 255));
	_rectangle.setSize(sf::Vector2f(_SCREEN_WIDTH, _SCREEN_HEIGHT / 6));
	_rectangle.setPosition(0, _SCREEN_HEIGHT - _rectangle.getSize().y);
	_rectangle.setOutlineThickness(2);
	_rectangle.setOutlineColor(Color::Black);

	Vector2f pos[5];

		for (int i = 0; i < 5; i++)
			pos[i] = Vector2f(_rectangle.getPosition().x + ((_SCREEN_WIDTH / 7) * i + _SCREEN_WIDTH / 15),
							  _rectangle.getPosition().y + (_rectangle.getGlobalBounds().height / 2) - (40 / 2));

		_button.push_back(new Button(_window, Vector2f(_SCREEN_WIDTH / 11, _SCREEN_HEIGHT / 19), pos[0], "Other"));
		_button.push_back(new Button(_window, Vector2f(_SCREEN_WIDTH / 11, _SCREEN_HEIGHT / 19), pos[1], "World Blocks"));
		_button.push_back(new Button(_window, Vector2f(_SCREEN_WIDTH / 11, _SCREEN_HEIGHT / 19), pos[2], "Enemies"));
		_button.push_back(new Button(_window, Vector2f(_SCREEN_WIDTH / 11, _SCREEN_HEIGHT / 19), pos[3], "All"));
		_button.push_back(new Button(_window, Vector2f(_SCREEN_WIDTH / 11, _SCREEN_HEIGHT / 19), pos[4], "Segments"));

	initTriangles();

		_shape[0].setPosition(Vector2f(_button[_button.size() - 1]->getRectangle().getPosition().x + _button[_button.size() - 1]->getRectangle().getGlobalBounds().width + _SCREEN_WIDTH / 12,
									   _button[_button.size() - 1]->getRectangle().getPosition().y - 20));

		_shape[1].setPosition(Vector2f(_button[_button.size() - 1]->getRectangle().getPosition().x + _button[_button.size() - 1]->getRectangle().getGlobalBounds().width + _SCREEN_WIDTH / 12 + 30,
									   _button[_button.size() - 1]->getRectangle().getPosition().y + _button[_button.size() - 1]->getRectangle().getGlobalBounds().height + 20));

		_shape[2].setPosition(Vector2f(_button[_button.size() - 1]->getRectangle().getPosition().x + _button[_button.size() - 1]->getRectangle().getGlobalBounds().width + _SCREEN_WIDTH / 6,
									   _button[_button.size() - 1]->getRectangle().getPosition().y - 20));

		_shape[3].setPosition(Vector2f(_button[_button.size() - 1]->getRectangle().getPosition().x + _button[_button.size() - 1]->getRectangle().getGlobalBounds().width + _SCREEN_WIDTH / 6 + 30,
									   _button[_button.size() - 1]->getRectangle().getPosition().y + _button[_button.size() - 1]->getRectangle().getGlobalBounds().height + 20));

		for (int i = 0; i < 3; i++) {
			_text[i].setCharacterSize(20);
			_text[i].setColor(Color::Black);

			_text[i].setFont(_font);
		}

	_text[0].setPosition(_shape[0].getPosition().x - 5, _shape[0].getPosition().y + 25);
	_text[1].setPosition(_shape[2].getPosition().x - 5, _shape[2].getPosition().y + 25);

	_text[2].setCharacterSize(15);
	_text[2].setColor(Color::White);
	_text[2].setPosition(_button.at(4)->getRectangle().getPosition().x, _button.at(4)->getRectangle().getPosition().y);

		for (int i = 0; i < 3; i++) {
			_information[i].setCharacterSize(20);
			_information[i].setColor(Color::Black);

			_information[i].setFont(_font);

			_information[i].setPosition(50 + (i * 200), 0);
		}

	_view = _window.getView();
}

bool Hud::events(Event event, int* max, int* min, SpriteManager* manager) {
	Vector2f mouse(Mouse::getPosition(_window));

	for (int i = 0; i < _button.size(); i++)
		if (event.key.code == Mouse::Left && _button[i]->getRectangle().getGlobalBounds().contains(mouse) && _visible) {
			switch (i) {
				case 0:
					*max = manager->getUpOther();
					*min = manager->getDownOther();

					_mode = false;
					_event = BUTTON_1;
					break;

				case 1:
					*max = manager->getUpTerrain();
					*min = manager->getDownTerrain();

					_mode = false;
					_event = BUTTON_2;
					break;

				case 2:
					*max = manager->getUpEnemy();
					*min = manager->getDownEnemy();

					_mode = false;
					_event = BUTTON_3;
					break;

				case 3:
					*max = manager->getUpOther();
					*min = manager->getDownTerrain();

					_mode = false;
					_event = BUTTON_4;
					break;

				case 4:
					*max = 0;
					*min = 0;

					_mode = true;
					_event = BUTTON_5;
					break;

				default:

					break;
			}
			setBorder();
			return true;
		}
	return false;
}

void Hud::sizeEvent(Event event, int* sizeX, int* sizeY) {
	Vector2f mouse(Mouse::getPosition(_window));

		for (int i = 0; i < 4; i++)
			if (event.key.code == Mouse::Left && _shape[i].getGlobalBounds().contains(mouse) && _visible) {
				switch (i) {
					case 0:
						if ((*sizeX + 10) <= 10000)
							*sizeX = *sizeX + 10;
						break;

					case 1:
						if ((*sizeX - 10) >= 100)
							*sizeX = *sizeX - 10;
						break;

					case 2:
						if ((*sizeY + 1) <= 20)
							*sizeY = *sizeY + 1;
						break;

					case 3:
						if ((*sizeY - 1) >= 1)
							*sizeY = *sizeY - 1;
						break;

					default:

						break;
				}
		}

		if (event.mouseWheelScroll.delta > 0.7 && _shape[0].getGlobalBounds().contains(mouse))
			if ((*sizeX + 10) <= 10000)
				*sizeX = *sizeX + 10;

		else if (event.mouseWheelScroll.delta < -0.7 && _shape[1].getGlobalBounds().contains(mouse))
			if ((*sizeX - 10) >= 100)
				*sizeX = *sizeX - 10;


		if (event.mouseWheelScroll.delta > 0.7 && _text[0].getGlobalBounds().contains(mouse))
			if ((*sizeX + 10) <= 10000)
				*sizeX = *sizeX + 10;

		else if (event.mouseWheelScroll.delta < -0.7 && _text[0].getGlobalBounds().contains(mouse))
			if ((*sizeX - 10) >= 100)
				*sizeX = *sizeX - 10;
}

void Hud::draw(const int sizeX, const int sizeY, const int segments) {
	update();

	Vector2f mouse(Mouse::getPosition(_window));

		if (_visible) {

			if(_visible)
				_window.setView(_view);

			_text[0].setString("X: " + to_string(sizeX));
			_text[1].setString("Y: " + to_string(sizeY));
			_text[2].setString(to_string(segments) + "/10");
		
				for (int i = 0; i < _button.size(); i++)
					if (_button[i]->getRectangle().getGlobalBounds().contains(mouse))
							_button[i]->setColor(Color(5, 113, 220));
						else
							_button[i]->setColor(Color(0, 102, 204));

				for (int i = 0; i < 4; i++)
					if (_shape[i].getGlobalBounds().contains(mouse))
						_shape[i].setFillColor(Color(51, 153, 255));
					else
						_shape[i].setFillColor(Color(0, 102, 204));
					
			_window.draw(_rectangle);

			for (int i = 0; i < _button.size(); i++)
				_button[i]->draw();

			for (int i = 0; i < 4; i++)
				_window.draw(_shape[i]);

			for (int i = 0; i < 3; i++)
				_window.draw(_text[i]);
		}
}

void Hud::drawInformation(const int fps, const int min, const int max) {
	if (_visible) {
		_information[0].setString("FPS: " + to_string(fps * 2));
		_information[1].setString("MIN ID: " + to_string(min));
		_information[2].setString("MAX ID: " + to_string(max));

		for (int i = 0; i < 3; i++)
			_window.draw(_information[i]);
	}
}

void Hud::initTriangles() {
	for (int i = 0; i < 4; i++) {
		_shape[i].setRadius(15);
		_shape[i].setPointCount(3);
		_shape[i].setFillColor(Color(0, 102, 204));

		_shape[i].setOutlineThickness(1);
		_shape[i].setOutlineColor(Color::Black);

		if ((i + 1) % 2 == 0)
			_shape[i].setRotation(180);
	}
}

void Hud::update() {
	isVisible();

	/*
	if (_visible) {
		if(_view.getCenter().y - _SCREEN_HEIGHT / 2 > 0)
			_view.move(0, 1);
	}
	else {
		if (_view.getCenter().y + _SCREEN_HEIGHT / 2 - _rectangle.getGlobalBounds().height < _SCREEN_HEIGHT - 50) {
			_view.move(0, 1);
		}
	}
	*/
}

void Hud::setBorder() {
	switch (_event){
		case BUTTON_1:
			_button[0]->setBorder(true);

			for (int i = 1; i < 5; i++)
				_button[i]->setBorder(false);
			break;

		case BUTTON_2:
			_button[1]->setBorder(true);

			for (int i = 0; i < 5; i++)
				if (i != 1)
					_button[i]->setBorder(false);
			break;

		case BUTTON_3:
			_button[2]->setBorder(true);

			for (int i = 0; i < 5; i++)
				if (i != 2)
					_button[i]->setBorder(false);
			break;

		case BUTTON_4:
			_button[3]->setBorder(true);

			for (int i = 0; i < 5; i++)
				if (i != 3)
					_button[i]->setBorder(false);
			break;

		case BUTTON_5:
			_button[4]->setBorder(true);

			for (int i = 0; i < 4; i++)
				_button[i]->setBorder(false);
			break;

		default:

			break;
	}
}

void Hud::isVisible() {
	Vector2f mouse(Mouse::getPosition(_window));

		if (mouse.y > (_SCREEN_HEIGHT - _rectangle.getGlobalBounds().height + 100))
			_visible = true;

		if (mouse.y < (_SCREEN_HEIGHT - _rectangle.getGlobalBounds().height))
			_visible = false;
}

bool Hud::getVisible() const {
	return this->_visible;
}

bool Hud::getMode() const{
	return this->_mode;
}

void Hud::setVisible(bool visible) {
	this->_visible = visible;
}

const RectangleShape Hud::getRectangle() const {
	return this->_rectangle;
}

Hud::~Hud() {
	_button.clear();
	delete[] _text;
}