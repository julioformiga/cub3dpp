#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>
#include "map.h"
#include "player.h"

constexpr float SCREEN_W = 1200.0f;
constexpr float SCREEN_H = 675.0f;

class Renderer {
public:
	Renderer(): wallSprite(wallTexture) {}
	void init();
	void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);
	void drawRays(sf::RenderTarget &target, const Player &player, const Map &map);
private:
	sf::Texture wallTexture;
	sf::Sprite wallSprite;
};

#endif // _RENDERER_H_
