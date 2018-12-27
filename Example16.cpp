#include<iostream>
#include<cmath>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"

using namespace sf;

int main()
{
	const unsigned WINDOW_WIDTH = 800;
	const unsigned WINDOW_HEIGHT = 600;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
	window.setFramerateLimit(120);

	float dt;
	Clock dt_clock;

	const float gridSize = 50.f;

	Vector2i mousePosGrid;

	//Player
	const float movementSpeed = 400.f;
	Vector2f velocity;
	RectangleShape player;
	player.setFillColor(Color::Green);
	player.setSize(Vector2f(gridSize, gridSize));

	//Walls
	std::vector<RectangleShape> walls;
	
	RectangleShape wall;
	wall.setFillColor(Color::Red);
	wall.setSize(Vector2f(gridSize, gridSize));
	wall.setPosition(gridSize * 5, gridSize * 2);

	walls.push_back(wall);

	//Collision
	FloatRect nextPos;

	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();

		mousePosGrid.x = Mouse::getPosition(window).x / (int)gridSize;
		mousePosGrid.y = Mouse::getPosition(window).y / (int)gridSize;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Add walls
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			bool exists = false;
			for (size_t i = 0; i < walls.size() && !exists; i++)
			{
				if (walls[i].getPosition().x / (int)gridSize == mousePosGrid.x
					&& walls[i].getPosition().y / (int)gridSize == mousePosGrid.y)
				{
					exists = true;
				}
			}

			if (!exists)
			{
				wall.setPosition(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize);
				walls.push_back(wall);
			}
		}

		//Remove wall
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			bool exists = false;
			int index = -1;
			for (size_t i = 0; i < walls.size() && !exists; i++)
			{
				if (walls[i].getPosition().x / (int)gridSize == mousePosGrid.x
					&& walls[i].getPosition().y / (int)gridSize == mousePosGrid.y)
				{
					exists = true;
					index = i;
				}
			}

			if (exists)
			{
				walls.erase(walls.begin() + index);
			}
		}

		//Player movement
		velocity.y = 0.f;
		velocity.x = 0.f;

		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			velocity.y += -movementSpeed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			velocity.y += movementSpeed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			velocity.x += -movementSpeed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			velocity.x += movementSpeed * dt;
		}

		//Collision
		for (auto &wall : walls)
		{
			FloatRect playerBounds = player.getGlobalBounds();
			FloatRect wallBounds = wall.getGlobalBounds();

			nextPos = playerBounds;
			nextPos.left += velocity.x;
			nextPos.top += velocity.y;

			if (wallBounds.intersects(nextPos))
			{
				//Bottom collision
				if (playerBounds.top < wallBounds.top
					&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left
					)
				{
					velocity.y = 0.f;
					player.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
				}

				//Top collision
				else if (playerBounds.top > wallBounds.top
					&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left
					)
				{
					velocity.y = 0.f;
					player.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
				}

				//Right collision
				if (playerBounds.left < wallBounds.left
					&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top
					)
				{
					velocity.x = 0.f;
					player.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
				}
				
				//Left collision
				else if (playerBounds.left > wallBounds.left
					&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top
					)
				{
					velocity.x = 0.f;
					player.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
				}
			}
		}

		player.move(velocity);

		//Collision screen
		//Left collision
		if (player.getPosition().x < 0.f)
			player.setPosition(0.f, player.getPosition().y);
		//Top collision
		if (player.getPosition().y < 0.f)
			player.setPosition(player.getPosition().x, 0.f);
		//Right collision
		if (player.getPosition().x + player.getGlobalBounds().width > WINDOW_WIDTH)
			player.setPosition(WINDOW_WIDTH - player.getGlobalBounds().width, player.getPosition().y);
		//Bottom collision
		if (player.getPosition().y + player.getGlobalBounds().height > WINDOW_HEIGHT)
			player.setPosition(player.getPosition().x, WINDOW_HEIGHT - player.getGlobalBounds().height);

		//Render
		window.clear();

		window.draw(player);

		for (auto &i : walls)
		{
			window.draw(i);
		}

		window.display();
	}

	return 0;
}
