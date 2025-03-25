#include "renderer.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

constexpr size_t MAX_RAYCASTING_STEPS = 16;
constexpr float PLAYER_FOV = 90.0f;

struct Ray {
	sf::Vector2f hitPosition;
	float distance;
	bool hit;
};

static Ray castRay(sf::Vector2f start, float angleDegrees, const Map &map);

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
		vdof = MAX_RAYCASTING_STEPS;
	}

	const auto &grid = map.getGrid();
	for (; vdof < MAX_RAYCASTING_STEPS; vdof++) {
		int mapX = (int)(vRayPos.x / cellSize);
		int mapY = (int)(vRayPos.y / cellSize);

		if (static_cast<size_t>(mapY) < grid.size() &&
		    static_cast<size_t>(mapX) < grid[mapY].size() &&
		    grid[mapY][mapX]) {
			hit = true;
			vdist = std::sqrt(
				(vRayPos.x - start.x) * (vRayPos.x - start.x) +
				(vRayPos.y - start.y) * (vRayPos.y - start.y)
			);
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
		hdof = MAX_RAYCASTING_STEPS;
	}

	for (; hdof < MAX_RAYCASTING_STEPS; hdof++) {
		int mapX = (int)(hRayPos.x / cellSize);
		int mapY = (int)(hRayPos.y / cellSize);

		if (static_cast<size_t>(mapY) < grid.size() &&
		    static_cast<size_t>(mapX) < grid[mapY].size() &&
		    grid[mapY][mapX]) {
			hit = true;
			hdist = std::sqrt(
				(hRayPos.x - start.x) * (hRayPos.x - start.x) +
				(hRayPos.y - start.y) * (hRayPos.y - start.y)
			);
			break;
		}

		hRayPos += offset;
	}
	return Ray { hdist < vdist ? hRayPos : vRayPos, std::min(hdist, vdist), hit };
};

