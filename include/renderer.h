#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cstddef>
#include <cmath>
#include <string>
#include "map.h"
#include "player.h"

constexpr float SCREEN_W = 960.0f;
constexpr float SCREEN_H = 540.0f;

class Renderer {
public:
	// Renderer(): wallSprite(wallTexture) {}
	void init();
	void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);
private:
	sf::Texture wallTexture;
	sf::Texture skyTexture;
	sf::Texture floorTexture;
	// sf::Sprite wallSprite;
};

#endif // _RENDERER_H_
