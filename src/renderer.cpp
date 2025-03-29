#include "renderer.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>

constexpr size_t MAX_RAYCASTING_DEPTH = 28;
constexpr float CAMERA_Z = 0.5f * SCREEN_H;
// constexpr size_t NUM_RAYS = 600;
// constexpr float PLAYER_FOV = 60.0f;
// constexpr float COLUMN_WIDTH = SCREEN_W / (float)NUM_RAYS;

struct Ray {
	sf::Vector2f hitPosition;
	sf::Vector2u mapPosition;
	float distance;
	bool hit;
	bool isHitVertical;
};

void Renderer::init() {
	if (!floorTexture.loadFromFile("./assets/texture_floor.png")) {
		std::cerr << "Failed to load texture" << std::endl;
	}
	if (floorTexture.getSize().x != floorTexture.getSize().y) {
		std::cerr << "ERROR: Texture is not square" << std::endl;
	}
	if (!wallTexture.loadFromFile("./assets/texture_wall.png")) {
		std::cerr << "Failed to load texture" << std::endl;
	}
	if (wallTexture.getSize().x != wallTexture.getSize().y) {
		std::cerr << "ERROR: Texture is not square" << std::endl;
	}
	// wallSprite = sf::Sprite(wallTexture);
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
	sf::Vector2f plane{-direction.y, direction.x * 0.66f};

	sf::Vector2f position = player.position / map.getCellSize();

	sf::VertexArray floorPixels{sf::PrimitiveType::Points};

	for (size_t y = SCREEN_H / 2; y < SCREEN_H; y++) {
		sf::Vector2f rayDirLeft{direction - plane}, rayDirRight{direction + plane};
		float rowDistance = CAMERA_Z / ((float)y - SCREEN_H / 2);
		sf::Vector2f floorStep = rowDistance * (rayDirRight - rayDirLeft) / SCREEN_W;
		sf::Vector2f floor = position + rowDistance * rayDirLeft;

		for (size_t x = 0; x < SCREEN_W; x++) {
			sf::Vector2i cell{floor};
			float textureSize = floorTexture.getSize().x;
			sf::Vector2f textCoords{textureSize * (floor - (sf::Vector2f)cell)};
			floorPixels.append({sf::Vector2f(x, y), sf::Color::White, textCoords});
			floor += floorStep;
		}
	}

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

		if (didHit) {
			float perpWallDist =
				isHitVertical ? sideDist.y - deltaDist.y : sideDist.x - deltaDist.x;
			float wallHeight = SCREEN_H / perpWallDist;

			float wallStart = (-wallHeight + SCREEN_H) / 2.0f;
			float wallEnd = (wallHeight + SCREEN_H) / 2.0f;

			float textureSize = wallTexture.getSize().x;

			float wallX = isHitVertical ? rayPos.x + perpWallDist * rayDir.x
										: rayPos.y + perpWallDist * rayDir.y;

			wallX -= std::floor(wallX);

			float textureX = wallX * textureSize;
			float brightness = 1.0f - (perpWallDist / (float)MAX_RAYCASTING_DEPTH);

			if (isHitVertical)
				brightness *= 0.7f;

			sf::Color color = sf::Color(255 * brightness, 255 * brightness, 255 * brightness);
			walls.append({sf::Vector2f((float)i, wallStart), color, sf::Vector2f(textureX, 0.0f)});
			walls.append({sf::Vector2f((float)i, wallEnd),   color, sf::Vector2f(textureX, textureSize)});
		}
	}
	sf::RenderStates states{&floorTexture};
	target.draw(floorPixels, states);
	states.texture = &wallTexture;
	target.draw(walls, states);
}
