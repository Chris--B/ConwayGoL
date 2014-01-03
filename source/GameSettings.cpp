#include "GameSettings.h"
#include "INIReader.h"

#include <cassert>

using namespace Conway;

GameSettings GameSettings::fromFile(const std::string& filename) {
	GameSettings settings;
	INIReader reader(filename);
	assert(reader.ParseError() >= 0);
		//can't load the file!

	settings.resolution_width = reader.GetInteger("window", "width", 800);
	settings.resolution_height = reader.GetInteger("window", "height", 800);
	settings.window_title = reader.Get("window", "title", "Conway's Game of Life");
	settings.fullscreen = reader.GetBoolean("window", "fullscreen", false);

	sf::Uint8 red, green, blue;

	// Colors default to black
	// TODO: How should too large of numbers be handled? Right now it just wraps around. e.g. 260 is read as 260 % 255 = 5
	red = reader.GetInteger("background color", "red", 0) & 0xff;
	green = reader.GetInteger("background color", "green", 0) & 0xff;
	blue = reader.GetInteger("background color", "blue", 0) & 0xff;
	settings.background_color = sf::Color(red, green, blue);

	red = reader.GetInteger("cell color", "red", 0) & 0xff;
	green = reader.GetInteger("cell color", "green", 0) & 0xff;
	blue = reader.GetInteger("cell color", "blue", 0) & 0xff;
	settings.cell_color = sf::Color(red, green, blue);

	settings.grid_width = reader.GetInteger("simulation", "width", 25);
	settings.grid_height = reader.GetInteger("simulation", "height", 25);
	settings.speed = reader.GetInteger("simulation", "speed", 15);
	settings.start_paused = reader.GetBoolean("simulation", "start_paused", false);

	return settings;
}
