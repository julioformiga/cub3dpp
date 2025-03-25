#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>
#include "map.h"
#include "player.h"

class Renderer {
public:
	void drawRays(sf::RenderTarget &target, const Player &player,
			   const Map &map);
};

#endif // _RENDERER_H_
