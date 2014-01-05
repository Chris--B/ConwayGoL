#include "game.h"
#include "INIReader.h"

#include <cstdio>
#include <cstdarg>
#include <cassert>

#include <fstream>

using namespace Conway;

static const std::string SETTINGS_FILE_DEFAULT = 
	";Default settings, feel free to edit."
	"\n;Delete this file to re-generate"
	"\n[window]"
	"\nlaunch height = 600"
	"\nlaunch width = 800"
	"\ntitle = Conway's Game of Life"
	"\n"
	"\n[background color]"
	"\nred = 25"
	"\ngreen = 25"
	"\nblue = 25"
	"\n"
	"\n[cell color]"
	"\nred = 255"
	"\ngreen = 0"
	"\nblue = 255"
	"\n"
	"\n[simulation]"
	"\npixels per cell = 10"
	"\nspeed = 10"
	"\n";

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
			board.addCell(Cell(pos.x / cell_size.x, pos.y / cell_size.y));
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			auto pos = sf::Mouse::getPosition(window);
			board.killCell(Cell(pos.x / cell_size.x, pos.y / cell_size.y));
		}

		// General events
		switch(event.type) {
		case sf::Event::Resized:
			window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			break;
		case sf::Event::Closed:
			stop();
			break;

		case sf::Event::KeyPressed:
			switch(event.key.code) {
			// R fills the screen with random cells
			case sf::Keyboard::R:
				board = Board();
				for(int x = 0; x < window.getSize().x / cell_size.x; ++x) {
					for(int y = 0; y < window.getSize().y / cell_size.y; ++y) {
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
    std::ifstream fi(filename);
	if (!fi) {
        // The file doesn't exist, we better try to create it.
        std::ofstream inifile(filename);
        if (inifile) {
            inifile << SETTINGS_FILE_DEFAULT;
            inifile.close();
        } else {
            // Could not open it. Bad things are about to happen....
            assert("Could not open settings file, bad things happened instead.");
        }
	}
    
	INIReader reader(filename);
    
	int width, height;

	if (!window.isOpen()) {
		height = reader.GetInteger("window", "launch height", 600);
		width = reader.GetInteger("window", "launch width", 800);
		setResolution(height, width);
	}

	auto length = reader.GetInteger("simulation", "pixels per cell", 10);
	cell_size = sf::Vector2f(length, length);

	speed = reader.GetInteger("simulation", "speed", 5);

	setWindowTitle(reader.Get("window", "title", "Conway's Game of Life"));

	// sf::Color uses 8bit ints, we'll use the extra values for error detection of sorts
	// This is messier than I'd like, maybe there's an easier way
	int red, green, blue;

	// Colors default to black
	// TODO: How should too large of numbers be handled?

	// If a color isn't specificed, assume 0
	// UNLESS none of them are given, then we want it to be white
	red = reader.GetInteger("background color", "red", -1);
	green = reader.GetInteger("background color", "green", -1);
	blue = reader.GetInteger("background color", "blue", -1);
	if (red == -1 && green == -1 && blue == -1) {
		red = green = blue = 255;
	} else {
		if (red == -1)	{ red = 0; }
		if (green == -1){ green = 0; }
		if (blue == -1)	{ blue = 0; }
	}
	color_background = sf::Color(red, green, blue);

	red = reader.GetInteger("cell color", "red", 0);
	green = reader.GetInteger("cell color", "green", 0);
	blue = reader.GetInteger("cell color", "blue", 0);
	color_cell = sf::Color(red, green, blue);
}

void Game::render() {
	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(color_background);
	window.clear();
	window.draw(background);

	sf::VertexArray cell_verts(sf::Quads, 4 * board.population());

	board.forEachCell( [this, &cell_verts](const Cell& cell){
		auto pos = sf::Vector2f(cell.x * cell_size.x, cell.y * cell_size.y);

		sf::Vertex vert(sf::Vector2f(), color_cell);

		vert.position = pos;
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x,               pos.y + cell_size.y);
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x + cell_size.x, pos.y + cell_size.y);
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x + cell_size.x, pos.y);
		cell_verts.append(vert);
	});
	window.draw(cell_verts);

	window.display();
}

void Game::start() {

	sf::Clock clock;
	loadSettings();

	while (isRunning()) {
		loadSettings();
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

void Game::setResolution(unsigned height, unsigned width) {
	window.create(sf::VideoMode(width, height), window_title);
}

void Game::setWindowTitle(const std::string& title) {
	window_title = title;
	window.setTitle(title);
}