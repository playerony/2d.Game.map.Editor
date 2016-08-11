#include "Block.h"

Block::Block(const int x, const int y, const int id)
	: _x(x), _y(y), _id(id) {

};

const int Block::getX() const {
	return this->_x;
}

const int Block::getY() const {
	return this->_y;
}

const int Block::getID() const {
	return this->_id;
}

void Block::setX(const int xPos) {
	this->_x = xPos;
}

void Block::setY(const int yPos) {
	this->_y = yPos;
}

Block::~Block(void) {

}