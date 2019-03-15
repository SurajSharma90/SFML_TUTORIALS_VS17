#include<iostream>
#include<sstream>
#include<vector>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"

int main()
{
	//Init game
	float gridSizeF = 100.f;
	unsigned gridSizeU = static_cast<unsigned>(gridSizeF);
	float dt = 0.f;
	sf::Clock dtClock;
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;
	sf::Font font;
	font.loadFromFile("Fonts/Dosis-Light.ttf");
	sf::Text text;
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
	text.setFont(font);
	text.setPosition(20.f, 20.f);
	text.setString("TEST");

	//Init window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
	window.setFramerateLimit(120);
	
	//Init window
	sf::View view;
	view.setSize(1920.f, 1080.f);
	view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
	float viewSpeed = 200.f;

	window.setView(view);

	//Init game elements
	sf::RectangleShape shape(sf::Vector2f(gridSizeF, gridSizeF));

	const int mapSize = 1000;

	std::vector<std::vector<sf::RectangleShape>> tileMap;

	tileMap.resize(mapSize, std::vector<sf::RectangleShape>());

	for (int x = 0; x < mapSize; x++)
	{
		tileMap[x].resize(mapSize, sf::RectangleShape());
		for (int y = 0; y < mapSize; y++)
		{
			tileMap[x][y].setSize(sf::Vector2f(gridSizeF, gridSizeF));
			tileMap[x][y].setFillColor(sf::Color::White);
			tileMap[x][y].setOutlineThickness(1.f);
			tileMap[x][y].setOutlineColor(sf::Color::Black);
			tileMap[x][y].setPosition(x * gridSizeF, y * gridSizeF);
		}
	}

	int fromX = 0;
	int toX = 0;
	int fromY = 0;
	int toY = 0;

	sf::RectangleShape tileSelector(sf::Vector2f(gridSizeF, gridSizeF));
	tileSelector.setFillColor(sf::Color::Transparent);
	tileSelector.setOutlineThickness(1.f);
	tileSelector.setOutlineColor(sf::Color::Green);

	while (window.isOpen())
	{
		//Update dt
		dt = dtClock.restart().asSeconds();

		//Update mouse positions
		mousePosScreen = sf::Mouse::getPosition();
		mousePosWindow = sf::Mouse::getPosition(window);
		window.setView(view);
		mousePosView = window.mapPixelToCoords(mousePosWindow);
		if(mousePosView.x >= 0.f)
			mousePosGrid.x = mousePosView.x / gridSizeU;
		if (mousePosView.y >= 0.f)
			mousePosGrid.y = mousePosView.y / gridSizeU;
		window.setView(window.getDefaultView());

		//Update game elements
		tileSelector.setPosition(mousePosGrid.x * gridSizeF, mousePosGrid.y * gridSizeF);

		//Update UI
		std::stringstream ss;
		ss << "Screen: " << mousePosScreen.x << " " << mousePosScreen.y << "\n"
			<< "Window: " << mousePosWindow.x << " " << mousePosWindow.y << "\n"
			<< "View: " << mousePosView.x << " " << mousePosView.y << "\n"
			<< "Grid: " << mousePosGrid.x << " " << mousePosGrid.y << "\n";

		text.setString(ss.str());

		//Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Update
		//Update input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Left
		{
			view.move(-viewSpeed * dt, 0.f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Right
		{
			view.move(viewSpeed * dt, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) //Up
		{
			view.move(0.f, -viewSpeed * dt);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Down
		{
			view.move(0.f, viewSpeed * dt);
		}

		//Render
		window.clear();
		
		//Render game elements
		window.setView(view);

		window.draw(shape);
		
		fromX = view.getCenter().x / gridSizeF - 5;
		toX = view.getCenter().x / gridSizeF + 6;
		fromY = view.getCenter().y / gridSizeF - 5;
		toY = view.getCenter().y / gridSizeF + 6;

		if (fromX < 0)
			fromX = 0;
		else if (fromX >= mapSize)
			fromX = mapSize - 1;

		if (toX < 0)
			toX = 0;
		else if (toX >= mapSize)
			toX = mapSize - 1;

		if (fromY < 0)
			fromY = 0;
		else if (fromY >= mapSize)
			fromY = mapSize - 1;

		if (toY < 0)
			toY = 0;
		else if (toY >= mapSize)
			toY = mapSize - 1;

		for (int x = fromX; x < toX; x++)
		{
			for (int y = fromY; y < toY; y++)
			{
				window.draw(tileMap[x][y]);
			}
		}

		window.draw(tileSelector);

		//Render ui
		window.setView(window.getDefaultView());	

		window.draw(text);

		//Done drawing
		window.display();
	}

	return 0;
}