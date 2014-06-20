#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace Conway {

struct Settings {
	// Setting defaults
	// [window]
	static const int HEIGHT_DEFAULT;
	static const int WIDTH_DEFAULT;
	static const std::string WINDOW_TITLE_DEFAULT;

	// Colors need to be their own section to because they come in three parts.
	// [background color]
	static const sf::Color BACKGROUND_COLOR_DEFAULT;

	// [cell color]
	static const sf::Color CELL_COLOR_DEFAULT;

	// [simulation]
	static const int PIXELS_PER_CELL_DEFAULT;
	static const int SPEED_DEFAULT;

	// Actual values. Write to these directly.
	// [window]
	int height                 = HEIGHT_DEFAULT;
	int width                  = WIDTH_DEFAULT;

	// SFML doesn't provide a getter, so we have to keep track of
	//     the title ourselves
	std::string window_title   = WINDOW_TITLE_DEFAULT;

	// [background color]
	sf::Color background_color = BACKGROUND_COLOR_DEFAULT;

	// [cell color]
	sf::Color cell_color       = CELL_COLOR_DEFAULT;

	// [simulation]
	int cell_size              = PIXELS_PER_CELL_DEFAULT;
	int speed                  = SPEED_DEFAULT;


	void fromFile(const std::string& filename);

	// TODO: Make it preserve use-comments?
	void saveToFile(const std::string& filename);
};

} // namespace Conway
