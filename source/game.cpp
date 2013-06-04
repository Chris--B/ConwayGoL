#include "game.h"

#include <cstdio>
#include <cstdarg>

using namespace Conway;

bool Game::isRunning() const {
	return window.isOpen();
}

void Game::addCell(int x, int y) {
	board.cells.insert(Cell(x, y));
}

void Game::handleEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch(event.type) {
		case sf::Event::Closed:
			stop();
			break;
		case sf::Event::KeyPressed:
			switch(event.key.code) {
			case sf::Keyboard::Q:
				stop();
				break;
			case sf::Keyboard::Space:
				paused = !paused;
				break;
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

	window.clear();
	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(sf::Color::White);

	window.draw(background);

	for(const auto cell : board.cells) {
		drawCell(cell);
	}

	window.display();
}

void Game::setResolution(unsigned height, unsigned width) {
	videomode = sf::VideoMode(height, width);
	cellsize = sf::Vector2f(height / board.getHeight(), width / board.getWidth());
}
void Game::setTitle(std::string title) {
	windowtitle = title;
}

void Game::start() {

	sf::Clock clock;
	const sf::Time delay = sf::seconds(1.0f / 15); // 15 generations per second

	window.create(videomode, windowtitle);

	while (isRunning()) {
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

void Game::stop() {
	window.close();
}

void Game::drawCell(const Cell& cell) {
	auto box = sf::RectangleShape(cellsize);
	auto pos = sf::Vector2f(cell.x * cellsize.x, cell.y * cellsize.y);
	box.setPosition(pos);
	box.setFillColor(sf::Color::Black);
	window.draw(box);
}

void Game::setBoardSize(int width, int height) {
	board.width = width;
	board.height = height;
	cellsize = sf::Vector2f(videomode.height / height, videomode.width / width);
}