#include "Game.hpp"
#include "inireader.hpp"

#include <cstdio>
#include <cstdarg>
#include <cassert>

#include <fstream>
#include <sstream>

using namespace Conway;

void Game::handleEvents() {
	sf::Event event;

	while (window.pollEvent(event)) {
		// Toggle the cell on mouse click/down (for dragging)
		// LMB creates, RMB kills. If both are held do nothing.
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				// Do nothing.
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			board.addCell(
				pos.x / settings.cell_size,
				pos.y / settings.cell_size);
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			board.killCell(
				pos.x / settings.cell_size,
				pos.y / settings.cell_size);
		}

		// General events
		switch(event.type) {
		case sf::Event::Resized:
			window.setView(sf::View(sf::FloatRect(
				0.0,
				0.0,
				(float)event.size.width,
				(float)event.size.height)));
			break;
		case sf::Event::Closed:
			stop();
			break;

		case sf::Event::KeyPressed:
			switch(event.key.code) {

				// R fills the screen with random cells
			case sf::Keyboard::R:
				board = Board();
				for(unsigned x = 0; x < window.getSize().x / settings.cell_size; ++x) {
					for(unsigned y = 0;
						y < window.getSize().y / settings.cell_size;
						++y) {
						if (rand() & 1) {
							board.addCell(x, y);
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

void Game::render() {
	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(settings.background_color);
	window.clear();
	window.draw(background);

	sf::VertexArray cell_verts(sf::Quads, 4 * board.population());

	board.forEachLivingCell( [this, &cell_verts](const Cell& cell){
		auto pos = sf::Vector2f(1.0f * cell.x * settings.cell_size, 1.0f * cell.y * settings.cell_size);

		sf::Vertex vert(sf::Vector2f(), settings.cell_color);

		vert.position = pos;
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x,               pos.y + settings.cell_size);
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x + settings.cell_size, pos.y + settings.cell_size);
		cell_verts.append(vert);

		vert.position = sf::Vector2f(pos.x + settings.cell_size, pos.y);
		cell_verts.append(vert);
	});
	window.draw(cell_verts);

	window.display();
}

void Game::setup() {
	// Does the file already exist?
	if (!std::ifstream(settings_filename).is_open()) {
		settings.saveToFile(settings_filename);
	} else {
		settings.fromFile(settings_filename);
	}

	window.create(
		sf::VideoMode(settings.width, settings.height),
		settings.window_title);
}

void Game::start() {
	sf::Clock clock;

	setup();

	while (running) {
		settings.fromFile(settings_filename);

		clock.restart();
		while (clock.getElapsedTime() < sf::seconds(1.0f / settings.speed)) {
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

void Game::setResolution(unsigned height, unsigned width) {
	window.create(sf::VideoMode(width, height), settings.window_title);
}

void Game::setWindowTitle(const std::string& title) {
	settings.window_title = title;
	window.setTitle(title);
}

#ifdef _WIN32
#include <Windows.h>

void Game::errorMessage(const std::string& message) {
	MessageBox(
		NULL,
		message.c_str(),
		"Error",
		MB_OK | MB_ICONERROR);
	exit(1);
}
#else
#include <iostream>
// TODO: use OS X / Linux version of MessageBox, this might not always display
void Game::errorMessage(const std::string& message) {
	std::cerr << message << "\n";
	exit(1);
}
#endif
