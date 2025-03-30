#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

constexpr float PLAYER_TURN_SPEED = 100.0f;
constexpr float PLAYER_MOVE_SPEED = 200.0f;

class Player {
public:
	void draw(sf::RenderTarget &target);
	void update(float deltaTime);

	sf::Vector2f position;
	float angle;
};

#endif
