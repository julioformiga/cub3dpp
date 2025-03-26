#include "map.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

Map::Map(float cellSize, int width, int height)
	: grid(height, std::vector(width, sf::Color::Black)), cellSize(cellSize) {};

Map::Map(float cellSize, const std::string &filename): cellSize(cellSize) {
	sf::Image image;
	if (!image.loadFromFile(filename)) {
		std::cerr << "Failed to load image";
		return;
	}
	grid = std::vector(image.getSize().y, std::vector(image.getSize().x, sf::Color::Black));

	for (unsigned int y = 0; y < image.getSize().y; y++) {
		for (unsigned int x = 0; x < image.getSize().x; x++) {
			grid[y][x] = image.getPixel({x, y});
		}
	}
};

void Map::loadMap(const std::filesystem::path &path) {
	std::ifstream in{path, std::ios::in | std::ios::binary};
	if (!in.is_open())
		std::cerr << "Failed to open file \"" << path << "\" for input";

	size_t w, h;
	in.read(reinterpret_cast<char *>(&w), sizeof(w));
	in.read(reinterpret_cast<char *>(&h), sizeof(h));

}

void Map::draw(sf::RenderTarget &target) {
	if (grid.empty())
		return;

	sf::RectangleShape background({grid[0].size() * cellSize, grid.size() * cellSize});
	background.setFillColor(sf::Color::Green);
	target.draw(background);

	sf::RectangleShape cell(sf::Vector2(cellSize * 0.95f, cellSize * 0.95f));

	for (size_t y = 0; y < grid.size(); y++) {
		for (size_t x = 0; x < grid[y].size(); x++) {
			cell.setFillColor(grid[y][x]);
			cell.setPosition(sf::Vector2f(x, y) * cellSize +
							 sf::Vector2f(cellSize * 0.025f, cellSize * 0.025f));
			target.draw(cell);
		}
	}
}

// void Map::drawMiniMap(sf::RenderTarget &target) {
// 	if (grid.empty()) {
// 		return;
// 	}
//
// 	sf::RectangleShape background({grid[0].size() * cellSize, grid.size() * cellSize});
// 	background.setFillColor(sf::Color::Green);
// 	target.draw(background);
//
// 	sf::RectangleShape cell(sf::Vector2(cellSize * 0.95f, cellSize * 0.95f));
//
// 	for (size_t y = 0; y < grid.size(); y++) {
// 		for (size_t x = 0; x < grid[y].size(); x++) {
// 			if (grid[y][x] == sf::Color::Black) {
// 				cell.setFillColor(sf::Color::Black);
// 			} else if (grid[y][x] == 1) {
// 				cell.setFillColor(sf::Color::White);
// 			}
// 			cell.setPosition(sf::Vector2f(x, y) * cellSize + sf::Vector2f(cellSize * 0.025f, cellSize * 0.025f));
// 			target.draw(cell);
// 		}
// 	}
	// }

const std::vector<std::vector<sf::Color>> &Map::getGrid() const {
	return grid;
}

float Map::getCellSize() const {
	return cellSize;
}
