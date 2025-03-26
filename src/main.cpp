#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "map.h"
#include "player.h"
#include "renderer.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({(int)SCREEN_W, (int)SCREEN_H}),
						 "Cub3D++", sf::Style::Close | sf::Style::Titlebar);

	Map map(48.0f, "assets/map.png");
	Player player;
	player.position = sf::Vector2f(300, 200);

	Renderer renderer;

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
		// map.draw(window);
		renderer.draw3dView(window, player, map);
		// renderer.drawRays(window, player, map);
		// player.draw(window);
		window.display();
	}

	return 0;
}
