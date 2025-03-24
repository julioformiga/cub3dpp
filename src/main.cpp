#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "map.h"
#include "player.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800,600}), "Cub3D++");

	std::vector<std::vector<int>> grid = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 1, 0, 0, 1},
		{1, 0, 1, 0, 1, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 1, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1}
	};

	Map map(32.0f, grid);
	Player player;
	player.position = sf::Vector2f(50, 50);

	sf::Clock gameClock;
	while (window.isOpen())
	{
		float deltaTime = gameClock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
				window.close();
			} else if (const auto* keyPressed
					= event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape
						|| keyPressed->scancode == sf::Keyboard::Scancode::Q)
					window.close();
			}
        }

		player.update(deltaTime);
		window.clear();
		map.draw(window);
		player.draw(window);
		window.display();
	}

	return 0;
}
