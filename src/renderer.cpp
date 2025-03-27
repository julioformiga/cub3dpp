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

static Ray castRay(sf::Vector2f start, float angleDegrees, const Map &map);

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
	Ray ray;
	float angle = player.angle - PLAYER_FOV / 2.0f;
	float angleIncrement = PLAYER_FOV / (float)NUM_RAYS;
	float wallHeight;
	float wallOffset;
	float shade;
	float brightness;
	int textureX;
	const float maxRenderDistance = MAX_RAYCASTING_DEPTH * map.getCellSize();
	const float maxFogDistance = maxRenderDistance / 4.0f;
	const sf::Color fogColor = sf::Color(100, 170, 250);

	sf::RectangleShape column {sf::Vector2f(1.0f, 1.0f)};
	sf::RectangleShape rectangle({SCREEN_W, SCREEN_H / 2.0f});
	rectangle.setFillColor(sf::Color(100, 170, 250));
	target.draw(rectangle);

	rectangle.setPosition({0, (int)(SCREEN_H / 2.0f)});
	rectangle.setFillColor(sf::Color(50, 50, 50));
	target.draw(rectangle);


	for (size_t i = 0; i < NUM_RAYS; i++, angle += angleIncrement) {
		ray = castRay(player.position, angle, map);
		if (ray.hit) {
			ray.distance *= std::cos((player.angle - angle) * M_PI / 180.0f);
			wallHeight = (map.getCellSize() * SCREEN_H) / ray.distance;

			wallOffset = SCREEN_H / 2.0f - wallHeight / 2.0f;
			if (ray.isHitVertical) {
				textureX = ray.hitPosition.y - wallTexture.getSize().x *
					std::floor(ray.hitPosition.y / wallTexture.getSize().x);
			} else {
				textureX = wallTexture.getSize().x *
					std::ceil(ray.hitPosition.x / wallTexture.getSize().x) - ray.hitPosition.x;
			}
			wallSprite.setPosition({i * COLUMN_WIDTH, wallOffset});
			wallSprite.setTextureRect(
				sf::IntRect({textureX, 0},
							{
								static_cast<int>(wallTexture.getSize().x / map.getCellSize()),
								static_cast<int>(wallTexture.getSize().y)
							}
				)
			);
			wallSprite.setScale({COLUMN_WIDTH,
								wallHeight / wallTexture.getSize().y});

			if (wallHeight > SCREEN_H)
				wallHeight = SCREEN_H;

			brightness = 1.0f - (ray.distance / maxRenderDistance);
			if (brightness < 0.0f)
				brightness = 0.0f;

			shade = (ray.isHitVertical ? 0.8f : 1.0f) * brightness;

			float fogPercentage = (ray.distance / maxFogDistance);
			if (fogPercentage > 1.0f)
				fogPercentage = 1.0f;
			column.setPosition({i * COLUMN_WIDTH, wallOffset});
			column.setScale({COLUMN_WIDTH, wallHeight});
			column.setFillColor(sf::Color(fogColor.r, fogColor.g, fogColor.b, fogPercentage * 255));
			wallSprite.setColor(sf::Color(255 * shade, 255 * shade, 255 * shade));
			target.draw(wallSprite);
			target.draw(column);
		}
	}
}

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {

	Ray ray;

	for (float angle = player.angle - PLAYER_FOV / 2.0f; angle < player.angle + PLAYER_FOV; angle += 1.0f) {
		ray = castRay(player.position, angle, map);
		if (ray.hit) {
			sf::Vertex line[] = {
				sf::Vertex({player.position}),
				sf::Vertex({ray.hitPosition})
			};
			target.draw(line, 2, sf::PrimitiveType::Lines);
		}
	}
}

Ray castRay(sf::Vector2f start, float angleDegrees, const Map &map) {
	float angle = angleDegrees * M_PI / 180.0f;
	float vtan = -std::tan(angle);
	float htan = -1.0f / std::tan(angle);
	float cellSize = map.getCellSize();

	bool hit = false;

	size_t vdof = 0;
	size_t hdof = 0;
	float hdist = std::numeric_limits<float>::max();
	float vdist = std::numeric_limits<float>::max();

	sf::Vector2u vMapPos, hMapPos;
	sf::Vector2f vRayPos, hRayPos, offset;
	if (std::cos(angle) > 0.001f) {
		vRayPos.x = std::floor(start.x / cellSize) * cellSize + cellSize;
		vRayPos.y = (start.x - vRayPos.x) * vtan + start.y;
		offset.x = cellSize;
		offset.y = -offset.x * vtan;
	} else if (std::cos(angle) < -0.001f) {
		vRayPos.x = std::floor(start.x / cellSize) * cellSize - 0.01f;
		vRayPos.y = (start.x - vRayPos.x) * vtan + start.y;
		offset.x = -cellSize;
		offset.y = -offset.x * vtan;
	} else {
		vdof = MAX_RAYCASTING_DEPTH;
	}

	const auto &grid = map.getGrid();
	for (; vdof < MAX_RAYCASTING_DEPTH; vdof++) {
		int mapX = (int)(vRayPos.x / cellSize);
		int mapY = (int)(vRayPos.y / cellSize);

		if ((size_t)(mapY) < grid.size() &&
		    (size_t)mapX < grid[mapY].size() &&
						grid[mapY][mapX] != sf::Color::Black) {
			hit = true;
			vdist = std::sqrt(
				(vRayPos.x - start.x) * (vRayPos.x - start.x) +
				(vRayPos.y - start.y) * (vRayPos.y - start.y)
			);
			vMapPos = sf::Vector2u(mapX, mapY);
			break;
		}
		vRayPos += offset;
	}

	if (sin(angle) > 0.001f) {
		hRayPos.y = std::floor(start.y / cellSize) * cellSize + cellSize;
		hRayPos.x = (start.y - hRayPos.y) * htan + start.x;
		offset.y = cellSize;
		offset.x = -offset.y * htan;
	} else if (sin(angle) < -0.001f) {
		hRayPos.y = std::floor(start.y / cellSize) * cellSize - 0.01f;
		hRayPos.x = (start.y - hRayPos.y) * htan + start.x;
		offset.y = -cellSize;
		offset.x = -offset.y * htan;
	} else {
		hdof = MAX_RAYCASTING_DEPTH;
	}

	for (; hdof < MAX_RAYCASTING_DEPTH; hdof++) {
		int mapX = (int)(hRayPos.x / cellSize);
		int mapY = (int)(hRayPos.y / cellSize);

		if ((size_t)(mapY) < grid.size() &&
		    (size_t)(mapX) < grid[mapY].size() &&
						grid[mapY][mapX] != sf::Color::Black) {
			hit = true;
			hdist = std::sqrt(
				(hRayPos.x - start.x) * (hRayPos.x - start.x) +
				(hRayPos.y - start.y) * (hRayPos.y - start.y)
			);
			hMapPos = sf::Vector2u(mapX, mapY);
			break;
		}

		hRayPos += offset;
	}
	return Ray {
		hdist < vdist ? hRayPos : vRayPos,
		hdist < vdist ? hMapPos : vMapPos,
		std::min(hdist, vdist),
		hit,
		vdist < hdist
	};
};

