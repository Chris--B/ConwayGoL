#include "game.h"
#include "INIReader.h"

#include <cstdio>
#include <cstdarg>
#include <cassert>

#include <fstream>
#include <sstream>

using namespace Conway;

// [window]
const int Game::LAUNCH_HEIGHT_DEFAULT = 780;
const int Game::LAUNCH_WIDTH_DEFAULT = 1240;
const std::string Game::WINDOW_TITLE_DEFAULT = "Conway's Game of Life";

//[background color]
const sf::Color Game::BACKGROUND_COLOR_DEFAULT = sf::Color(25, 25, 25);

//[cell color]
const sf::Color Game::CELL_COLOR_DEFAULT = sf::Color(255, 0, 255);

//[simulation]
const int Game::PIXELS_PER_CELL_DEFAULT = 10;
const int Game::SPEED_DEFAULT = 10;

Game::Game() :
	paused(false),
	running(true),
	cell_size(PIXELS_PER_CELL_DEFAULT),
	speed(SPEED_DEFAULT),
	background_color(BACKGROUND_COLOR_DEFAULT),
	cell_color(CELL_COLOR_DEFAULT),
	settings_filename("Conway.ini"),
	window_title(WINDOW_TITLE_DEFAULT) {

}

void Game::handleEvents() {
	sf::Event event;

	while (window.pollEvent(event)) {
		// Toggle the cell on mouse click/down (for dragging)
		// LMB creates, RMB kills. If both are held do nothing.
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				// do nothing
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			auto pos = sf::Mouse::getPosition(window);
			board.addCell(Cell(pos.x / cell_size, pos.y / cell_size));
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			auto pos = sf::Mouse::getPosition(window);
			board.killCell(Cell(pos.x / cell_size, pos.y / cell_size));
		}

		// General events
		switch(event.type) {
		case sf::Event::Resized:
			window.setView(sf::View(sf::FloatRect(0.0, 0.0, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::Closed:
			stop();
			break;

		case sf::Event::KeyPressed:
			switch(event.key.code) {

				// R fills the screen with random cells
			case sf::Keyboard::R:
				board = Board();
				for(unsigned x = 0; x < window.getSize().x / cell_size; ++x) {
					for(unsigned y = 0; y < window.getSize().y / cell_size; ++y) {
						if (rand() & 1) {
							board.addCell(Cell(x, y));
						}
					}
				}
				break;

				// Q quits
			case sf::Keyboard::Q:
				stop();
				break;

				// C clears
			case sf::Keyboard::C:
				board = Board();
				break;

				// Space toggles pause
			case sf::Keyboard::Space:
				paused = !paused;
				break;

				// Right arrow key advances one generation at a time when paused
			case sf::Keyboard::Right:
				if (paused) {
					board.advanceBoard();
				}
				break;
			}
		}
	}
}

void Game::loadSettings(const std::string& filename) {
	INIReader reader(filename);
	if (reader.ParseError() < 0) {
		std::stringstream ss;
		ss << "There was an error while parsing " << filename << ".";
		errorMessage(ss.str());
	}

	if (!window.isOpen()) {
		auto height = reader.GetInteger("window", "launch height", LAUNCH_HEIGHT_DEFAULT);
		auto width = reader.GetInteger("window", "launch width", LAUNCH_WIDTH_DEFAULT);
		setResolution(height, width);
	}

	cell_size = reader.GetInteger("simulation", "pixels per cell", PIXELS_PER_CELL_DEFAULT);

	speed = reader.GetInteger("simulation", "speed", SPEED_DEFAULT);

	setWindowTitle(reader.Get("window", "title", WINDOW_TITLE_DEFAULT));

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
		background_color = sf::Color(red, green, blue);
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
		cell_color = sf::Color(red, green, blue);
	}
}

void Game::render() {
	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(background_color);
	window.clear();
	window.draw(background);

	sf::VertexArray cell_verts(sf::Quads, 4 * board.population());

	board.forEachCell( [this, &cell_verts](const Cell& cell){
		auto pos = sf::Vector2f(1.0f * cell.x * cell_size, 1.0f * cell.y * cell_size);

		sf::Vertex vert(sf::Vector2f(), cell_color);

		vert.position = pos;
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x,               pos.y + cell_size);
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x + cell_size, pos.y + cell_size);
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x + cell_size, pos.y);
		cell_verts.append(vert);
	});
	window.draw(cell_verts);

	window.display();
}

void Game::start() {

	sf::Clock clock;

	// Does the file already exist?
	if (!std::ifstream(settings_filename).is_open()) {
		saveSettings(settings_filename);
	}

	while (isRunning()) {
		loadSettings(settings_filename);

		clock.restart();
		while (clock.getElapsedTime() < sf::seconds(1.0f / speed)) {
			handleEvents();
			render();
		}

		if (!paused) {
			board.advanceBoard();
		}
	}
}

void Game::stop() {
	window.close();
	running = false;
}

void Game::saveSettings(const std::string& filename) {

	auto launch_height = window.getSize().y ? window.getSize().y : LAUNCH_HEIGHT_DEFAULT;
	auto launch_width = window.getSize().x ? window.getSize().x : LAUNCH_WIDTH_DEFAULT;

	std::ofstream fi(filename);
	if (!fi) {
		std::stringstream ss;
		ss << "Unable to open " << filename << "!";
		errorMessage(ss.str());
	}

	fi << "; Edit this file to change settings, or delete it to reset them.\n";

	fi << "[window]\n";
	fi << "launch height = " << launch_height << "\n";
	fi << "launch width = "  << launch_width << "\n";
	fi << "\n";

	fi << "[background color]\n";
	fi << "\n";
	fi << "red = "   << (int)background_color.r << "\n";
	fi << "green = " << (int)background_color.g << "\n";
	fi << "blue = "  << (int)background_color.b << "\n";
	fi << "\n";

	fi << "[cell color]\n";
	fi << "\n";
	fi << "red = "   << (int)cell_color.r << "\n";
	fi << "green = " << (int)cell_color.g << "\n";
	fi << "blue = "  << (int)cell_color.b << "\n";
	fi << "\n";

	fi << "[simulation]\n";
	fi << "pixels per cell = " << cell_size << "\n";
	fi << "speed = " << speed << "\n";
	fi << "\n";

}

void Game::setResolution(unsigned height, unsigned width) {
	window.create(sf::VideoMode(width, height), window_title);
}

void Game::setWindowTitle(const std::string& title) {
	window_title = title;
	window.setTitle(title);
}

#ifdef WIN32
#include <Windows.h>

void Game::errorMessage(const std::string& message) {
	MessageBox(NULL, message.c_str(), window_title.c_str(),
		MB_OK | MB_ICONERROR);
	exit(1);
}
#else
// TODO: use OS X / Linux version of MessageBox, this might not always display
void Game::errorMessage(const std::string& message) {
	std::cerr << message << "\n";
	exit(1);
}
#endif