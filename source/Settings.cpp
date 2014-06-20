#include "Settings.hpp"
#include "Game.hpp"
#include "inireader.hpp"

#include <fstream>
#include <sstream>

using namespace Conway;

// [window]
const int Settings::HEIGHT_DEFAULT                 = 780;
const int Settings::WIDTH_DEFAULT                  = 1240;
const std::string Settings::WINDOW_TITLE_DEFAULT   = "Conway's Game of Life";

// [background color]
const sf::Color Settings::BACKGROUND_COLOR_DEFAULT = sf::Color(25, 25, 25);

// [cell color]
const sf::Color Settings::CELL_COLOR_DEFAULT       = sf::Color(255, 0, 255);

// [simulation]
const int Settings::PIXELS_PER_CELL_DEFAULT        = 10;
const int Settings::SPEED_DEFAULT                  = 10;

void Settings::fromFile(const std::string& filename) {
	INIReader reader(filename);
	if (reader.ParseError() < 0) {
		std::stringstream ss;
		ss << "There was an error while parsing " << filename << ".";
		Game::errorMessage(ss.str());
	}

	height = reader.GetInteger("window", "launch height", HEIGHT_DEFAULT);
	width = reader.GetInteger("window", "launch width", WIDTH_DEFAULT);

	cell_size = reader.GetInteger("simulation", "pixels per cell",
		PIXELS_PER_CELL_DEFAULT);

	speed = reader.GetInteger("simulation", "speed", SPEED_DEFAULT);

	window_title = reader.Get("window", "title", WINDOW_TITLE_DEFAULT);

	// sf::Color uses 8bit ints, we'll use the extra values for error detection of sorts.
	// This is messier than I'd like, maybe there's an easier way?
	int red, green, blue;

	// TODO: How should too large of numbers be handled?

	// If a color isn't specificed, assume 0, UNLESS none of them are given, then we want to use the default color
	red = reader.GetInteger("background color", "red", -1);
	green = reader.GetInteger("background color", "green", -1);
	blue = reader.GetInteger("background color", "blue", -1);
	if (red == -1 && green == -1 && blue == -1) {
		background_color = BACKGROUND_COLOR_DEFAULT;
	} else {
		if (red == -1)	{ red = 0; }
		if (green == -1){ green = 0; }
		if (blue == -1)	{ blue = 0; }
		background_color = sf::Color(
			sf::Uint8(red), sf::Uint8(green), sf::Uint8(blue));
	}

	red = reader.GetInteger("cell color", "red", -1);
	green = reader.GetInteger("cell color", "green", -1);
	blue = reader.GetInteger("cell color", "blue", -1);
	if (red == -1 && green == -1 && blue == -1) {
		cell_color = CELL_COLOR_DEFAULT;
	} else {
		if (red == -1)	{ red = 0; }
		if (green == -1){ green = 0; }
		if (blue == -1)	{ blue = 0; }
		cell_color = sf::Color(
			sf::Uint8(red), sf::Uint8(green), sf::Uint8(blue));
	}
}

void Settings::saveToFile(const std::string& filename) {
	std::ofstream fi(filename);
	if (!fi) {
		std::stringstream ss;
		ss << "Unable to open file '" << filename << "'!";
		Game::errorMessage(ss.str());
	}

	fi << "; Edit this file to change settings, or delete it to reset them.\n";

	fi << "[window]\n";
	fi << "launch height = " << height << "\n";
	fi << "launch width = "  << width << "\n";
	fi << "\n";

	fi << "[background color]\n";
	fi << "red = "   << (int)background_color.r << "\n";
	fi << "green = " << (int)background_color.g << "\n";
	fi << "blue = "  << (int)background_color.b << "\n";
	fi << "\n";

	fi << "[cell color]\n";
	fi << "red = "   << (int)cell_color.r << "\n";
	fi << "green = " << (int)cell_color.g << "\n";
	fi << "blue = "  << (int)cell_color.b << "\n";
	fi << "\n";

	fi << "[simulation]\n";
	fi << "pixels per cell = " << cell_size << "\n";
	fi << "speed = " << speed << "\n";
	fi << "\n";
}
