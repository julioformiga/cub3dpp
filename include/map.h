#ifndef _MAP_H_
#define _MAP_H_

#include <SFML/Graphics/RenderTarget.hpp>
#include <filesystem>
#include <string>
#include <vector>

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
