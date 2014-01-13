#pragma once

#include "board.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Conway {
	class Game {
	public:
		// [window]
		static const int LAUNCH_HEIGHT_DEFAULT;
		static const int LAUNCH_WIDTH_DEFAULT;
		static const std::string WINDOW_TITLE_DEFAULT;

		//[background color]
		static const sf::Color BACKGROUND_COLOR_DEFAULT;

		//[cell color]
		static const sf::Color CELL_COLOR_DEFAULT;

		//[simulation]
		static const int PIXELS_PER_CELL_DEFAULT;
		static const int SPEED_DEFAULT;

		Game();

		bool isRunning() const { return running; }

		void errorMessage(const std::string& message);

		void handleEvents();

		void loadSettings(const std::string& filename);

		void render();
		void start();
		void stop();

		void saveSettings(const std::string& filename);

		void setResolution(unsigned height, unsigned width);
		void setSettingsFile(const std::string& filename) { settings_filename = filename; }
		void setWindowTitle(const std::string& title);

		void addCell(int x, int y) { board.addCell(Cell(x, y)); }

	private:
		Board board;

		bool paused;
		bool running;
		int cell_size;
		int speed;

		sf::Color background_color;
		sf::Color cell_color;

		sf::RenderWindow window;

		std::string settings_filename;
		std::string window_title;      //SFML doesn't provide a getter, so we have to keep track of the title manually.

	};
} // namespace Conway
