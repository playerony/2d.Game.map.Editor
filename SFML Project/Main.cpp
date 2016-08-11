#include <SFML/Graphics.hpp>

#include "Menu.h"

int main()
{
	volatile int ANTYALIASING = 8;

	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTYALIASING;

	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	//sf::RenderWindow window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Map Editor", sf::Style::Fullscreen, settings);
	sf::RenderWindow window(VideoMode(1365, 768), "Map Editor", sf::Style::None, settings);

		window.setVerticalSyncEnabled(true);
		window.setMouseCursorVisible(false);

		Menu* menu = new Menu(window);
		menu->runMenu();

	return 0;
}