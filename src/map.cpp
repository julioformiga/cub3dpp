#include "map.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

Map::Map(float cellSize, int width, int height)
	: grid(height, std::vector(width, 0)), cellSize(cellSize) {};

Map::Map(float cellSize, std::vector<std::vector<int>> grid)
	: grid(grid), cellSize(cellSize) {};

void Map::draw(sf::RenderTarget &target) {
	if (grid.empty()) {
		return;
	}

	sf::RectangleShape background({grid[0].size() * cellSize, grid.size() * cellSize});
	background.setFillColor(sf::Color::Green);
	target.draw(background);

	sf::RectangleShape cell(sf::Vector2(cellSize * 0.95f, cellSize * 0.95f));

	for (size_t y = 0; y < grid.size(); y++) {
		for (size_t x = 0; x < grid[y].size(); x++) {
			if (grid[y][x] == 0) {
				cell.setFillColor(sf::Color::Black);
			} else if (grid[y][x] == 1) {
				cell.setFillColor(sf::Color::White);
			}
			cell.setPosition(sf::Vector2f(x, y) * cellSize + sf::Vector2f(cellSize * 0.025f, cellSize * 0.025f));
			target.draw(cell);
		}
	}
}

const std::vector<std::vector<int>> &Map::getGrid() const {
	return grid;
}

float Map::getCellSize() const {
	return cellSize;
}
