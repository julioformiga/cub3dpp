#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics/RenderTarget.hpp>

class Player {
public:
	void draw(sf::RenderTarget &target);
	void update(float deltaTime);

	sf::Vector2f position;
	float angle;
};

#endif
