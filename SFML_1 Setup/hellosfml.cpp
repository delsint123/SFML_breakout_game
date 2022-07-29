/* --------------------------------------------------------
 *    File: hellosfml.cpp
 *  Author: Delphine Sintamour
 *   Class: COP 2001, Spring 2022
 * Purpose: demo SFML
 * -------------------------------------------------------- */

#include <SFML/Graphics.hpp>

// Global Defines
// --------------------------------------------------------
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

const sf::Color WINDOW_COLOR = sf::Color::Black;
const sf::Color BALL_COLOR = sf::Color::Yellow;

int main() {

    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hello SFML");
    window.clear(WINDOW_COLOR);

    sf::CircleShape ball;
    ball.setRadius(15);
    ball.setOrigin(10.0, 10.0);
    ball.setPosition(WINDOW_WIDTH / 3.0, WINDOW_HEIGHT / 2.0);
    ball.setFillColor(BALL_COLOR);

    while (window.isOpen())
    {
        // Process user input
        // ------------------------------------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Render drawing objects
        // ------------------------------------------------
        window.clear(WINDOW_COLOR);
        window.draw(ball);
        window.display();
    }

    return 0;
}
