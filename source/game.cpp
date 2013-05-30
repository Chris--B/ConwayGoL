#include "game.h"
#include "cell.h"

#include <cstdio>
#include <cstdarg>

using namespace Conway;

int Game::countLivingNeighbors(const Cell& cell) const {
	auto living = 0;
	auto neighbors = cell.getNeighbors();
	for(auto c : neighbors) {
		if (isCellAlive(c)) {
			++living;
		}
	}
	return living;
}

bool Game::isCellAlive(const Cell& cell) const {
	return cells.find(cell) != cells.end();
}

bool Game::isRunning() const {
	return window.isOpen();
}

void Game::addCell(const Cell& cell) {
	cells.insert(cell);
	log("Inserted cell @ %d, %d.", cell.x, cell.y);
}
void Game::addCell(int x, int y) {
	addCell(Cell(x, y));
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
			}
			break;
		}
	}
}

void Game::render() {

	window.clear();
	auto background = sf::RectangleShape(sf::Vector2f(window.getSize()));
	background.setFillColor(sf::Color::White);
	
	window.draw(background);
	for(auto cell : cells) {
		drawCell(cell);
	}
	window.display();
}

void Game::setResolution(unsigned height, unsigned width) {
	videomode = sf::VideoMode(height, width);
}
void Game::setTitle(std::string title) {
	windowtitle = title;
}

void Game::start() {

	sf::Clock clock;
	const sf::Time delay = sf::seconds(1.0 / 3);

	window.create(videomode, windowtitle);

	while (isRunning()) {
		clock.restart();
		while (clock.getElapsedTime() < delay) {
			handleEvents();
			render();
		}
		if (!paused) {
			int x = advanceBoard();
		}
	}
}

void Game::stop() {
	window.close();
}

int Game::advanceBoard(unsigned generations) {
	int living_count = 0;
	while (generations--) {
		std::unordered_set<Cell> next_gen;

		for(auto cell : cells) {
			log("Checking neighbors of existing cell %d, %d", cell.x, cell.y);
			for(auto neighbor : cell.getNeighbors()) {
				log("Found cell %d, %d", neighbor.x, neighbor.y);
				auto living_neighbors = countLivingNeighbors(neighbor);
				if (living_neighbors == 3 || (living_neighbors == 2 && isCellAlive(neighbor))) {
					next_gen.insert(neighbor);
					++living_count;
				}
			}
		}
		cells = next_gen;
	}
	return living_count;
}

void Game::log(std::string format, ...) const {
#ifdef NDEBUG
	// do nothing
#else
	va_list va;
	va_start(va, format);
	vfprintf(stdout, format.c_str(), va);
	printf("\n");
	va_end(va);
#endif
}

void Game::drawCell(const Cell& cell) {
	sf::RectangleShape square(sf::Vector2f(gridsize, gridsize));
	square.setPosition(cell.x * gridsize, cell.y * gridsize);
	square.setFillColor(sf::Color::Black);
	window.draw(square);
}
