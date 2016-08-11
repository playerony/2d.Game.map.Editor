#include "FileManager.h"

FileManager::FileManager(RenderWindow& window)
: _window(window), _encryption(false), _thread(&FileManager::loader, this) {

	_loadScreen = new LoadScreen(_window);
}

deque<string> FileManager::getListOfFiles(const char* fileName) const{
	deque<string> names;

		if (fileName != NULL) {
			char search_path[200];
			sprintf(search_path, "%s/*.png", fileName);

			WIN32_FIND_DATA fd;
			HANDLE hFind = FindFirstFile(search_path, &fd);

				if (hFind != INVALID_HANDLE_VALUE) {
					do {
						if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
							names.push_back(fd.cFileName);
					} while (FindNextFile(hFind, &fd));

					FindClose(hFind);
				}
		}

	return names;
}

bool FileManager::isEmpty(const char* fileName) const {
	if (fileName != NULL) {
		string filePath = "maps/" + string(fileName);

		FILE* file = fopen(filePath.c_str(), "r");

			if (file != NULL) {
				char phrase[16];

				while (fgets(phrase, 16, file)) 
					if (strlen(phrase) > 1)
						return false;
			}

			else {
				MessageBox(NULL, "Cannot check this file!", "FileManager.cpp", NULL);
				return true;
			}

		fclose(file);

		return true;
	}

	return false;
}

void FileManager::saveToFile(string path, deque<Block*>& segment, Vector2f size) {
	if (path.size() != NULL && size.x > 0 && size.y > 0) {
		_thread.launch();
		_window.setActive(false);

		string finalPath = "./maps/" + path + ".png";

		FILE* file = fopen(finalPath.c_str(), "w");
		int** table = NULL;

		deque<Block*> trees;

		if (file != NULL && path.size() > 4) {
			table = (int**)calloc(size.x, sizeof(int*));

			for (int i = 0; i < size.x; i++)
				table[i] = (int*)calloc(size.y, sizeof(int));

				if (table != NULL) {
					for (int i = 0; i < segment.size(); i++) {

						if (segment[i]->getID() == 20) {
							table[segment[i]->getX()][segment[i]->getY()] = 0;
							trees.push_back(new Block(segment[i]->getX(), segment[i]->getY() + 2, segment[i]->getID()));
						}

						else if (segment[i]->getID() == 21) {
							table[segment[i]->getX()][segment[i]->getY()] = 0;
							trees.push_back(new Block(segment[i]->getX(), segment[i]->getY() + 2, segment[i]->getID()));
						}

						else
							table[segment[i]->getX()][segment[i]->getY()] = segment[i]->getID();
					}

					for (int p = 0; p < trees.size(); p++) 
						table[trees[p]->getX()][trees[p]->getY()] = trees[p]->getID();

					if (_encryption) {
						fprintf(file, "%d %d %d\n", (int)size.x * (int)size.y, ((int)size.x * (int)size.y) * (int)size.y, 1);
						int code = size.x * size.y;

						for (int i = 0; i < size.y; i++) {
							for (int j = 0; j < size.x; j++) 
								fprintf(file, "%d %d ", (73 * table[j][i]) + ((code + 11) * 83), (83 * table[j][i]) + ((code + 191) * 93));
							fprintf(file, "\n");
						}
					}

					else {
						fprintf(file, "%d %d %d\n", (int)size.x, (int)size.y, 0);

						for (int i = 0; i < size.y; i++) {
							for (int j = 0; j < size.x; j++)
								fprintf(file, "%d ", table[j][i]);
							fprintf(file, "\n");
						}
					}

					MessageBox(NULL, "Saving succesfull!", finalPath.c_str(), NULL);

					for (int i = 0; i < size.x; i++) free(table[i]);
					free(table);
				}

				else {
					MessageBox(NULL, "Cannot allocate requirement memory!", "FileManager.cpp", NULL);

					for (int i = 0; i < size.x; i++) free(table[i]);
					free(table);
				}
		}

		else
			MessageBox(NULL, "Some problem with save!", "FileManager.cpp", NULL);

		fclose(file);

		_window.setActive(true);
	}
}

deque<Block*> FileManager::loadFromFile(const char* path, int* sizeX, int* sizeY) {
	deque<Block*> segment;

	if (path != NULL) {
		_thread.launch();
		_window.setActive(false);

		char filePath[64] = "maps/";
		strcat(filePath, path);

		printf("%s\n", filePath);

		FILE* file = fopen(filePath, "r");
		unsigned int id[2] = { 0, 0 };
		int width = 0, height = 0;
		int check = 0;

		if (file != NULL) {
			fscanf(file, "%d %d %d", &width, &height, &check);

			if (width > 0 && height > 0 && check == 1) {

				height = height / width;
				width /= height;

				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width * 2; j += 2) {
						fscanf(file, "%d %d ", &id[0], &id[1]);

						int code1 = ((width * height) + 11) * 83;
						int code2 = ((width * height) + 191) * 93;

						id[0] -= code1;
						id[0] /= 73;

						id[1] -= code2;
						id[1] /= 83;

						if (id[0] == id[1]) {
							if (id[0] != 0 && id[0] != 20 && id[0] != 21)
								segment.push_back(new Block(j / 2, i, id[0]));

							else if (id[0] == 20 || id[0] == 21)
								segment.push_back(new Block(j / 2, i - 2, id[0]));
						}

						else
						{
							segment.clear();
							MessageBox(NULL, "Map was edit not by the editor", "Editor.cpp", NULL);
							break;
						}
					}
				}
			}

			else if (width > 0 && height > 0 && check == 0) {
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						fscanf(file, "%d ", &id[0]);

						if (id[0] != 0 && id[0] != 20 && id[0] != 21)
							segment.push_back(new Block(j, i, id[0]));

						else if (id[0] == 20 || id[0] == 21)
							segment.push_back(new Block(j, i - 2, id[0]));

						else if (id[0] < 0) {
							segment.clear();
							MessageBox(NULL, "Map was edit not by the editor", "Editor.cpp", NULL);
							break;
						}
					}
				}
			}

			else if(width < 0 || height < 0)
				MessageBox(NULL, "Wrong size of the map", "Editor.cpp", NULL);

			else
				MessageBox(NULL, "Map not found", "Editor.cpp", NULL);
		}
	
		fclose(file);

		*sizeX = width;
		*sizeY = height;
	}

	_window.setActive(true);

	return segment;
}

void FileManager::loader() {
	_window.setActive(true);
	_loadScreen->showLoadScreen();
	_window.setActive(false);
}

void FileManager::setEncryption(bool encryption) {
	this->_encryption = encryption;
}

FileManager::~FileManager(void) {
	delete _loadScreen;
}