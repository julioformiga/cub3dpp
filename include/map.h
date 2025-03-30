#ifndef _MAP_H_
#define _MAP_H_

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Map {
public:
	Map(float cellSize, int width, int height);
	Map(float cellSize, const std::string &filename);

	void loadMap(const std::filesystem::path &path);
	void draw(sf::RenderTarget &target);
	void drawMiniMap(sf::RenderTarget &target);

	const std::vector<std::vector<sf::Color>> &getGrid() const;
	float getCellSize() const;
private:
	std::vector<std::vector<sf::Color>> grid;
	float cellSize;
};

#endif
