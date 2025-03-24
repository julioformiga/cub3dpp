#include "player.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

constexpr float TURN_SPEED = 100.0f;
constexpr float MOVE_SPEED = 100.0f;

void Player::draw(sf::RenderTarget &target) {
	sf::CircleShape circle(5.0f);
	circle.setPosition(position);

	target.draw(circle);
}

void Player::update(float deltaTime) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
		angle -= TURN_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		angle += TURN_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
		float radians = angle * 3.14159f / 180.0f;

		position.x += std::cos(radians) * MOVE_SPEED * deltaTime;
		position.y += std::sin(radians) * MOVE_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
		float radians = angle * 3.14159f / 180.0f;

		position.x -= std::cos(radians) * MOVE_SPEED * deltaTime;
		position.y -= std::sin(radians) * MOVE_SPEED * deltaTime;
	}
}
