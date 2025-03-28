#include "renderer.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>

constexpr size_t MAX_RAYCASTING_DEPTH = 64;
constexpr size_t NUM_RAYS = 600;
constexpr float PLAYER_FOV = 60.0f;
constexpr float COLUMN_WIDTH = SCREEN_W / (float)NUM_RAYS;

struct Ray {
	sf::Vector2f hitPosition;
	sf::Vector2u mapPosition;
	float distance;
	bool hit;
	bool isHitVertical;
};


void Renderer::init() {
	if (!wallTexture.loadFromFile("./assets/texturewall.png")) {
		std::cerr << "Failed to load texture" << std::endl;
	}
	if (wallTexture.getSize().x != wallTexture.getSize().y) {
		std::cerr << "ERROR: Texture is not square" << std::endl;
	}
	wallSprite = sf::Sprite(wallTexture);
}

void Renderer::draw3dView(sf::RenderTarget &target, const Player &player, const Map &map) {
	sf::RectangleShape column {sf::Vector2f(1.0f, 1.0f)};
	sf::RectangleShape rectangle({SCREEN_W, SCREEN_H / 2.0f});
	rectangle.setFillColor(sf::Color(100, 170, 250));
	target.draw(rectangle);

	rectangle.setPosition({0, (int)(SCREEN_H / 2.0f)});
	rectangle.setFillColor(sf::Color(50, 50, 50));
	target.draw(rectangle);

	// const sf::Color fogColor = sf::Color(100, 170, 250);
	// const float maxRenderDistance = MAX_RAYCASTING_DEPTH * map.getCellSize();
	// const float maxFogDistance = maxRenderDistance / 4.0f;

	float radians = player.angle * M_PI / 180.0f;
	sf::Vector2f direction{
		std::cos(radians),
		std::sin(radians)
	};
	sf::Vector2f plane{-direction.y, direction.x};

	sf::VertexArray walls{sf::PrimitiveType::Lines};

	for (size_t i = 0; i < SCREEN_W; i++) {
		float cameraX = 2.0f * i / SCREEN_W - 1.0f;
		sf::Vector2f rayPos = player.position / map.getCellSize();
		sf::Vector2f rayDir = direction + plane * cameraX;

		sf::Vector2f deltaDist{
			std::abs(1.0f / rayDir.x),
			std::abs(1.0f / rayDir.y)
		};
		sf::Vector2i mapPos{rayPos};
		sf::Vector2i step;
		sf::Vector2f sideDist;
		if (rayDir.x < 0) {
			step.x = -1;
			sideDist.x = (rayPos.x - mapPos.x) * deltaDist.x;
		} else {
			step.x = 1;
			sideDist.x = (mapPos.x + 1.0f - rayPos.x) * deltaDist.x;
		}
		if (rayDir.y < 0) {
			step.y = -1;
			sideDist.y = (rayPos.y - mapPos.y) * deltaDist.y;
		} else {
			step.y = 1;
			sideDist.y = (mapPos.y + 1.0f - rayPos.y) * deltaDist.y;
		}
		bool didHit{}, isHitVertical{};
		size_t depth = 0;
		while (!didHit && depth < MAX_RAYCASTING_DEPTH) {
			if (sideDist.x < sideDist.y) {
				sideDist.x += deltaDist.x;
				mapPos.x += step.x;
				isHitVertical = false;
			} else {
				sideDist.y += deltaDist.y;
				mapPos.y += step.y;
				isHitVertical = true;
			}
			int x = mapPos.x;
			int y = mapPos.y;
			const auto &grid = map.getGrid();
			if (y >= 0 && y < (int)grid.size() && x >= 0 &&
				x < (int)grid[y].size() &&
				grid[y][x] != sf::Color::Black) {
			  didHit = true;
			}
			depth++;
		}
		float perpWallDist =
			isHitVertical ? sideDist.y - deltaDist.y : sideDist.x - deltaDist.x;
		float wallHeight = SCREEN_H / perpWallDist;

		float wallStart = (-wallHeight + SCREEN_H) / 2.0f;
		float wallEnd = (wallHeight + SCREEN_H) / 2.0f;

		sf::Vertex vertex;
		vertex.position = sf::Vector2f((float)i, wallStart);
		walls.append(vertex);

		vertex.position = sf::Vector2f((float)i, wallEnd);
		walls.append(vertex);

	}
	target.draw(walls);
}
