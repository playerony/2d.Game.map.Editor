#pragma once
#ifndef _EDITOR_H_
#define _EDITOR_H_
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <deque>

#include "SpriteManager.h"
#include "FileManager.h"
#include "Instruction.h"
#include "Block.h"
#include "Exit.h"
#include "Hud.h"

using namespace std;
using namespace sf;

class Editor {
	private:
		deque<Block*> _segment;
		deque< deque<Block*> > _part;

		RectangleShape line[2];
		IntRect _square;
		RenderWindow& _window;
		Clock _clock;
		View _view;
		Font _font;

		Vector2i _pixelPos;
		Vector2f _mouse,
				 _selectBox[2];

		Texture _cur;
		Sprite _cursor;

		Hud* _hud;
		FileManager* _fileManager;
		SpriteManager* _manager;
		Exit* _exit;

		int _SCREEN_WIDTH, _SCREEN_HEIGHT,
			_sizeBlock, _sizeX, _sizeY,
			_id,
			_fps,
			_frame,
			_minID,
			_maxID;

		int _boxPos[2],
			_deletePos[2],
			_selectPos[2],
			_lastPos[2],
			_render[2];

		bool _box,
			 _add,
			 _area,
			 _draw,
			 _mode,
			 _fill,
			 _press,
			 _addBlock,
			 _clear,
			 _move,
			 _save,
			 _lines,
		   	 _delete;

		enum State {EDITOR, INSTRUCTION, CLOSE, SAVE, FAST_SAVE, END};
		State _state;

		enum Scroll { GORA, DOL, NEUTRAL };
		Scroll _scroll;

		enum Move {UP, DOWN, LEFT, RIGHT , NOONE};
		Move _moveArea;

	public:
		Editor(RenderWindow&, const int, const int, char*);

			void runEditor();
			void showEditor();
			void update();

			void scrollEvents();
			void restartView();

			void drawLines();
			void drawSegment();
			void drawFillBox();
			void boxCollision(Hud*);
			void drawScene();

			void moveBlocks();

			int checkPosition(const int, const int) const;
			int checkIn(const int, const int) const;
			bool checkIndex(const int) const;

		~Editor(void);
};

#endif