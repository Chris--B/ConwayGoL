#include "game.h"
#include "INIReader.h"

#include <cstdio>
#include <cstdarg>
#include <cassert>

#include <fstream>

using namespace Conway;

void Game::handleEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch(event.type) {
		case sf::Event::Closed:
			stop();
			break;
		case sf::Event::KeyPressed:
			switch(event.key.code) {
				// Q quits
			case sf::Keyboard::Q:
				stop();
				break;
			case sf::Keyboard::Space:
				// Space toggles pause
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
            inifile << ";Auto-generated empty file.\n";
            inifile.close();
        } else {
            // Could not open it. Bad things are about to happen....
            assert("Could not open settings file, bad things happened instead.");
        }
	}
	INIReader reader(filename);
    
	int width, height;

	height = reader.GetInteger("window", "height", 800);
	width = reader.GetInteger("window", "width", 800);
	setResolution(height, width);

	height = reader.GetInteger("simulation", "height", 25);
	width = reader.GetInteger("simulation", "width", 25);
	setBoardSize(height, width);

	speed = reader.GetInteger("simulation", "speed", 15);

	setWindowTitle(reader.Get("window", "title", "Conway's Game of Life"));

	// sf::Color uses 8bit ints, we'll use the extra values for error detection of sorts
	// This is messier than I'd like, maybe there's a work around somewhere?
	int red, green, blue;

	// Colors default to black
	// TODO: How should too large of numbers be handled?

	// If a color isn't specificed, assume 0
	// UNLESS they're all 0, then we want it to be white
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
	window.clear();

	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(color_background);
	window.draw(background);

	board.forEachCell( [this](Cell cell){
		auto box = sf::RectangleShape(cell_size);
		auto pos = sf::Vector2f(cell.x * cell_size.x, cell.y * cell_size.y);
		box.setPosition(pos);
		box.setFillColor(color_cell);
		window.draw(box);
	});

	window.display();
}

void Game::start() {

	sf::Clock clock;
	const sf::Time delay = sf::seconds(1.0f / speed); 

	window.create(video_mode, window_title);

	while (isRunning()) {
		loadSettings("conway.ini");
		clock.restart();
		while (clock.getElapsedTime() < delay) {
			handleEvents();
			render();
		}
		if (!paused) {
			board.advanceBoard();
		}
	}
}

void Game::setBoardSize(int height, int width) {
	board.setHeight(height);
	board.setWidth(width);
	cell_size = sf::Vector2f(1.0f * video_mode.height / height, 1.0f * video_mode.width / width);
}

void Game::setResolution(unsigned height, unsigned width) {
	video_mode = sf::VideoMode(height, width);
	cell_size = sf::Vector2f(1.0f * height / board.getHeight(), 1.0f * width / board.getWidth());
}
