#include "player.h"

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::H)) {
		angle -= PLAYER_TURN_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::L)) {
		angle += PLAYER_TURN_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::K)) {
		float radians = angle * M_PI / 180.0f;

		position.x += std::cos(radians) * PLAYER_MOVE_SPEED * deltaTime;
		position.y += std::sin(radians) * PLAYER_MOVE_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::J)) {
		float radians = angle * M_PI / 180.0f;

		position.x -= std::cos(radians) * PLAYER_MOVE_SPEED * deltaTime;
		position.y -= std::sin(radians) * PLAYER_MOVE_SPEED * deltaTime;
	}
}
