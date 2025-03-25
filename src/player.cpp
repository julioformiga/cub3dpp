#include "player.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

constexpr float TURN_SPEED = 200.0f;
constexpr float MOVE_SPEED = 100.0f;

void Player::draw(sf::RenderTarget &target) {
	sf::CircleShape circle(8.0f);
	circle.setOrigin({circle.getRadius(), circle.getRadius()});
	circle.setPosition(position);
	circle.setFillColor(sf::Color::Red);

	sf::RectangleShape line(sf::Vector2f(24.0f, 2.0f));
	line.setPosition(position);
	line.setRotation(sf::Angle(sf::degrees(angle)));
	line.setFillColor(sf::Color::Red);

	target.draw(line);
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
		float radians = angle * M_PI / 180.0f;

		position.x += std::cos(radians) * MOVE_SPEED * deltaTime;
		position.y += std::sin(radians) * MOVE_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
		float radians = angle * M_PI / 180.0f;

		position.x -= std::cos(radians) * MOVE_SPEED * deltaTime;
		position.y -= std::sin(radians) * MOVE_SPEED * deltaTime;
	}
}
