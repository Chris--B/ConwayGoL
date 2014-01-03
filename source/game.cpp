#include "game.h"

#include <cstdio>
#include <cstdarg>

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

void Game::render() {
	window.clear();

	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(backgroundColor);
	window.draw(background);

	board.forEachCell( [this](Cell cell){
		drawCell(cell);
	});

	window.display();
}

void Game::setResolution(unsigned height, unsigned width) {
	videomode = sf::VideoMode(height, width);
	cellsize = sf::Vector2f(1.0f * height / board.getHeight(), 1.0f * width / board.getWidth());
}

void Game::start() {

	sf::Clock clock;
	const sf::Time delay = sf::seconds(1.0f / generations_per_sec); 

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

void Game::drawCell(const Cell& cell) {
	auto box = sf::RectangleShape(cellsize);
	auto pos = sf::Vector2f(cell.x * cellsize.x, cell.y * cellsize.y);
	box.setPosition(pos);
	box.setFillColor(cellColor);
	window.draw(box);
}

void Game::setBoardSize(int width, int height) {
	board.setWidth(width);
	board.setHeight(height);
	cellsize = sf::Vector2f(1.0f * videomode.height / height, 1.0f * videomode.width / width);
	

}