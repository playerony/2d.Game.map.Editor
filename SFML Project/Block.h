#pragma once
#ifndef _BLOCK_H_
#define _BLOCK_H_

class Block {
	private:
		int _x, _y;
		int	_id;

	public:
		Block(const int, const int, const int);

			const int getX() const;
			const int getY() const;
			const int getID() const;

			void setX(const int);
			void setY(const int);

		~Block(void);
};

#endif // !_BLOCK_H_