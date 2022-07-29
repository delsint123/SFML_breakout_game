/* --------------------------------------------------------
 *    File: moveball.cpp
 *  Author: Delphine Sintamour
 *   Class: COP 2001, Spring 2022
 * Purpose: make the ball move
 * -------------------------------------------------------- */

#include <SFML/Graphics.hpp>

// Global Constants
// --------------------------------------------------------

// window properties
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::Black;

// drawing properties
const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::Yellow;

// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float SPEED_X = WINDOW_WIDTH / SPEED_TIME;    // speed horizontally
const float SPEED_Y = WINDOW_HEIGHT / SPEED_TIME;   // span  vertically

// Type definitions
// --------------------------------------------------------

// <direction enum here>
enum Direction {
    Exit = -1,
    None,
    Left,
    Up,
    Right,
    Down
};

// ball properties
struct Ball {
    // <define structure properties here>
    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf::Color color;
};

// Function declarations (prototypes)
// --------------------------------------------------------
Direction processInput() ;
void update(Direction &input, Ball &ball, float delta);
void render(sf::RenderWindow &window, Ball ball, float delta);

/**
 * The main application
 * @return OS status message (0=Success)
 */
int main() {

    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ("Hello SFML"));
    window.clear(WINDOW_COLOR);

    // declare a ball variable and populate it in the center of the window
    // <initialize the ball structure properties here>
    Ball theBall = {
            10.0,
            WINDOW_WIDTH/2.0,
            WINDOW_HEIGHT/2.0,
            0.0,
            0.0,
            BALL_COLOR,
    };


    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;

    bool gameOver = false;
    while (!gameOver)
    {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;

        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }

        // Process user input
        // ------------------------------------------------
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time

                    update(userInput, theBall, delta);

            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        // ------------------------------------------------
        render(window, theBall, delta);

    } // end main game loop

    // game ending, close the graphics window

    window.close();
    return 0;   // return success to the OS
} // end main

/**
 * convert user keyboard input into recognized integer values
 * for left, up, right, down
 * @return Direction - user input (default no-input=None, quit=Exit)
 */
Direction processInput() {
    Direction input = None;  // no input

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }

    return input;
} // end getUserInput

/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 */
void update(Direction &input, Ball &ball, float delta) {

    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            case Left: // Left
                ball.velocityX -= SPEED_X;
                break;
            case Up: // Up
                ball.velocityY -= SPEED_Y;
                break;
            case Right: // Right
                ball.velocityX += SPEED_X;
                break;
            case Down: // Down
                ball.velocityY += SPEED_Y;
        }
        // clear input
        input = None;
    }

    // adjust the location of the ball for speed * time
    ball.coordinateX += ball.velocityX*delta;
    ball.coordinateY += ball.velocityY*delta;

} // end update

/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 */
void render(sf::RenderWindow &window, Ball ball, float delta) {
    // Render drawing objects
    // ------------------------------------------------

    // clear the window with the background color
    window.clear(WINDOW_COLOR);

    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;
    circle.setFillColor(BALL_COLOR);
    circle.setRadius(ball.radius);

    // set screen coordinates relative to the center of the circle
    circle.setOrigin(ball.radius, ball.radius);

    // calculate current drawing location relative to speed and frame-time
    float xPosition = ball.coordinateX + ball.velocityX*delta;
    float yPosition = ball.coordinateY + ball.velocityY*delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    // show the new window
    // ------------------------------------------------
    window.display();
} // end render
