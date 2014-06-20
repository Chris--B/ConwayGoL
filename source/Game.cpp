#include "Game.hpp"
#include "inireader.hpp"

#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cmath>

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
			board.addCell(cellAtMouse(sf::Mouse::getPosition(window)));
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			board.killCell(cellAtMouse(sf::Mouse::getPosition(window)));
		}

		// General events
		switch(event.type) {
		case sf::Event::Resized:
			handleResize(event.size.width, event.size.height);
			break;

		case sf::Event::Closed:
			stop();
			break;

		case sf::Event::KeyPressed:
			switch(event.key.code) {

			case sf::Keyboard::F1: {
					sf::Image screen = window.capture();
					screen.saveToFile("Brian.png");
				}
				break;

			case sf::Keyboard::R: {
					// TODO: Put this in a function.
					// TODO: Make scaling coordinates easier.
					// R fills the screen with random cells
					board.clear();

					sf::View view = window.getView();
					sf::Vector2f halfsize = sf::Vector2f(
						view.getSize().x / 2,
						view.getSize().y / 2);
					sf::Vector2f center = view.getCenter();
					sf::Vector2f lower = center - halfsize;
					sf::Vector2f upper = center + halfsize;

					// We need to scale it into board coordinates.
					board.randomize(
						(int)lower.x / settings.cell_size,
						(int)lower.y / settings.cell_size,
						(int)upper.x / settings.cell_size,
						(int)upper.y / settings.cell_size);
				}
				break;

			case sf::Keyboard::Q:
				// Q quits
				stop();
				break;

			case sf::Keyboard::C:
				// C clears
				board.clear();
				break;

			case sf::Keyboard::Space:
				// Space toggles pause
				paused = !paused;
				break;

			// Arrowkeys move the view around.
			case sf::Keyboard::Right:
				moveView(20, 0);
				break;

			case sf::Keyboard::Left:
				moveView(-20, 0);
				break;

			case sf::Keyboard::Up:
				moveView(0, -20);
				break;

			case sf::Keyboard::Down:
				moveView(0, 20);
				break;
			}
		}
	}
}

void Game::render() {
	window.clear(settings.background_color);

	sf::VertexArray cell_verts(sf::Quads, 4 * board.population());

	board.forEachLivingCell( [this, &cell_verts](const Cell& cell) {
		sf::Vector2f pos = sf::Vector2f(
			1.0f * cell.x * settings.cell_size,
			1.0f * cell.y * settings.cell_size);

		sf::Vertex vert(sf::Vector2f(), settings.cell_color);

		// Top left
		vert.position = pos;
		cell_verts.append(vert);

		// Bottom left
		vert.position = sf::Vector2f(
			pos.x,
			pos.y + settings.cell_size);
		cell_verts.append(vert);

		// Bottom right
		vert.position = sf::Vector2f(
			pos.x + settings.cell_size,
			pos.y + settings.cell_size);
		cell_verts.append(vert);

		// Top right
		vert.position = sf::Vector2f(
			pos.x + settings.cell_size,
			pos.y);
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

Cell Game::cellAtMouse(sf::Vector2i pos) {
	sf::View view = window.getView();
	sf::Vector2f center = view.getCenter();
	return Cell(
		int(pos.x + center.x - (view.getSize().x / 2)) / settings.cell_size,
		int(pos.y + center.y - (view.getSize().y / 2)) / settings.cell_size
	);
}

void Game::handleResize(int width, int height) {
	const sf::View view = window.getView();

	sf::Vector2f halfsize = sf::Vector2f(
		view.getSize().x / 2,
		view.getSize().y / 2);

	// When resizing, it's important to keep the top left unchanged.
	// The view only stores center and size, so we need to
	//     re-calcualte the top left corner on each resize.
	sf::Vector2f topleft = view.getCenter() - halfsize;
	sf::Vector2f size = sf::Vector2f((float)width, (float)height);

	window.setView(sf::View(sf::FloatRect(topleft, size)));
}

void Game::moveView(float dx, float dy) {
	sf::View view = window.getView();
	view.move(dx, dy);
	window.setView(view);
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
