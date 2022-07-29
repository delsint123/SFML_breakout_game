/* --------------------------------------------------------
 *    File: collisionDetection.cpp
 *  Author: Delphine Sintamour
 *   Class: COP 2001, Spring 2022
 * Purpose: Apply collision detection to the moving ball program
 * -------------------------------------------------------- */

#include <SFML/Graphics.hpp>
#include <cmath>

// Global Constants

//block properties
float BLOCK_THICKNESS = 5.0;
sf::Color BLOCK_COLOR = sf::Color::Cyan;

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

enum Direction {
    Exit = -1,
    None,
    Left,
    Up,
    Right,
    Down
};

//structure for block properties
struct Block {
    float left;
    float top;
    float width;
    float height;
    sf::Color blockColor;
    sf::RectangleShape blockShape;
};

//border properties
struct Borders {
    Block leftBlock;
    Block topBlock;
    Block rightBlock;
    Block bottomBlock;
};

// ball properties
struct Ball {
    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf::Color color;
};

// Function declarations (prototypes)
Direction processInput() ;
void update(Direction &input, Ball &ball, float delta, Borders theBorders);
void render(sf::RenderWindow &window, Ball ball, float delta, Borders theBorder);
int getCollisionPoint(Ball *ptrBall, Block *ptrBlock);
bool collisionCheck(Ball *ptrBall, Block *ptrBlock);

/**
 * The main application
 * @return OS status message (0=Success)
 */
int main() {

    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ("Hello SFML"));
    window.clear(WINDOW_COLOR);

    //declare a variable of Borders and initialize
    Borders theBlocks;

    //initialize leftBlock
    theBlocks.leftBlock.left = 0.0;
    theBlocks.leftBlock.top = 0.0;
    theBlocks.leftBlock.width = BLOCK_THICKNESS;
    theBlocks.leftBlock.height = WINDOW_HEIGHT;
    theBlocks.leftBlock.blockColor = BLOCK_COLOR;
    theBlocks.leftBlock.blockShape.setSize(sf::Vector2(theBlocks.leftBlock.width, theBlocks.leftBlock.height));
    theBlocks.leftBlock.blockShape.setPosition(theBlocks.leftBlock.left, theBlocks.leftBlock.top);
    theBlocks.leftBlock.blockShape.setFillColor(theBlocks.leftBlock.blockColor);

    //initialize topBlock
    theBlocks.topBlock.left = 0.0;
    theBlocks.topBlock.top = 0.0;
    theBlocks.topBlock.width = WINDOW_WIDTH;
    theBlocks.topBlock.height = BLOCK_THICKNESS;
    theBlocks.topBlock.blockColor = BLOCK_COLOR;
    theBlocks.topBlock.blockShape.setSize(sf::Vector2(theBlocks.topBlock.width, theBlocks.topBlock.height));
    theBlocks.topBlock.blockShape.setPosition(theBlocks.topBlock.left, theBlocks.topBlock.top);
    theBlocks.topBlock.blockShape.setFillColor(theBlocks.topBlock.blockColor);


    //initialize rightBlock
    theBlocks.rightBlock.left = WINDOW_WIDTH-BLOCK_THICKNESS;
    theBlocks.rightBlock.top = 0.0;
    theBlocks.rightBlock.width = BLOCK_THICKNESS;
    theBlocks.rightBlock.height = WINDOW_HEIGHT;
    theBlocks.rightBlock.blockColor = BLOCK_COLOR;
    theBlocks.rightBlock.blockShape.setSize(sf::Vector2(theBlocks.rightBlock.width, theBlocks.rightBlock.height));
    theBlocks.rightBlock.blockShape.setPosition(theBlocks.rightBlock.left, theBlocks.rightBlock.top);
    theBlocks.rightBlock.blockShape.setFillColor(theBlocks.rightBlock.blockColor);


    //initialize bottomBlock
    theBlocks.bottomBlock.left = 0.0;
    theBlocks.bottomBlock.top = WINDOW_HEIGHT - BLOCK_THICKNESS;
    theBlocks.bottomBlock.width = WINDOW_WIDTH;
    theBlocks.bottomBlock.height = BLOCK_THICKNESS;
    theBlocks.bottomBlock.blockColor = BLOCK_COLOR;
    theBlocks.bottomBlock.blockShape.setSize(sf::Vector2(theBlocks.bottomBlock.width, theBlocks.bottomBlock.height));
    theBlocks.bottomBlock.blockShape.setPosition(theBlocks.bottomBlock.left, theBlocks.bottomBlock.top);
    theBlocks.bottomBlock.blockShape.setFillColor(theBlocks.bottomBlock.blockColor);

    // declare a ball variable and populate it in the center of the window
    Ball theBall;
    theBall.radius = 10.0;
    theBall.coordinateX = WINDOW_WIDTH/2.0;
    theBall.coordinateY = WINDOW_HEIGHT/2.0;
    theBall.velocityX = 0.0;
    theBall.velocityY = 0.0;
    theBall.color = BALL_COLOR;


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
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time

            update(userInput, theBall, delta, theBlocks);

            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        render(window, theBall, delta, theBlocks);

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
 * Checks if there is a collision with the ball
 * @param ptrBall
 * @param ptrBlock
 * @return - int cardinal heading between 1 and 360
 */
int getCollisionPoint(Ball *ptrBall, Block *ptrBlock){

    //initialize to 0 for no collision
    int cardinalHeading = 0;

    float checkPointX;
    float checkPointY;

    //find the x-coordinate for the check point
    if(ptrBall->coordinateX < ptrBlock->left) {
        checkPointX = ptrBlock->left;
    }
    else if(ptrBall->coordinateX > (ptrBlock->left + ptrBlock->width)) {
        checkPointX = (ptrBlock->left + ptrBlock->width);
    }
    else {
        checkPointX = ptrBall->coordinateX;
    }

    //find the y-coordinate for the check point
    if(ptrBall->coordinateY < ptrBlock->top) {
        checkPointY = ptrBlock->top;
    }
    else if(ptrBall->coordinateY > (ptrBlock->top + ptrBlock->height)) {
        checkPointY = (ptrBlock->top + ptrBlock->height);
    }
    else {
        checkPointY = ptrBall->coordinateY;
    }

    //difference values
    float diffX;
    float diffY;

    diffX = checkPointX - ptrBall->coordinateX;
    diffY = (WINDOW_HEIGHT - checkPointY) - (WINDOW_HEIGHT - ptrBall->coordinateY);

    //calculate the distance
    double distance = sqrt(pow(diffX, 2.0) + pow(diffY,2.0));

    //check if there is a collision
    if(distance <= ptrBall->radius) {
        double theta = atan2(diffY, diffX);
        double degrees = 90.0 - theta * 180/M_PI;

        if(degrees <= 0) {
            degrees += 360;
        }

        cardinalHeading = int(degrees);
    }

    return cardinalHeading;
};

/**
 * Checks if ball collides with any objects
 * @param ptrBall
 * @param ptrBlock
 * @return bool if there is a collision
 */
bool collisionCheck(Ball *ptrBall, Block *ptrBlock) {
    bool collides = false;

    int collisionPoint = getCollisionPoint(ptrBall,ptrBlock);

    if(collisionPoint > 0) {
        collides = true;

        //check horizontal collisions
        if(collisionPoint > 225 && collisionPoint < 315) {
            ptrBall->velocityX *= -1;
            ptrBall->coordinateX = ptrBlock->left + ptrBlock->width + ptrBall->radius + 1;
        }
        else if(collisionPoint > 45 && collisionPoint < 135) {
            ptrBall->velocityX *= -1;
            ptrBall->coordinateX = ptrBlock->left - ptrBall->radius - 1;
        }

        //check vertical collisions
        if(collisionPoint >= 315 || collisionPoint <= 45) {
            ptrBall->velocityY *= -1;
            ptrBall->coordinateY = ptrBlock->top + ptrBlock->height + ptrBall->radius + 1;
        }
        else if(collisionPoint >= 135 && collisionPoint <= 225) {
            ptrBall->velocityY *= -1;
            ptrBall->coordinateY = ptrBlock->top - ptrBall->radius - 1;
        }

    }
    return collides;
};


/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 */
void update(Direction &input, Ball &ball, float delta, Borders theBorders) {

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

    //collision check
    collisionCheck(&ball, &(theBorders.leftBlock));
    collisionCheck(&ball, &(theBorders.topBlock));
    collisionCheck(&ball, &(theBorders.rightBlock));
    collisionCheck(&ball, &(theBorders.bottomBlock));

} // end update

/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 */
void render(sf::RenderWindow &window, Ball ball, float delta, Borders theBorders) {
    // Render drawing objects
    // ------------------------------------------------

    // clear the window with the background color
    window.clear(WINDOW_COLOR);

    //draw the rectangle borders
    window.draw(theBorders.leftBlock.blockShape);
    window.draw(theBorders.bottomBlock.blockShape);
    window.draw(theBorders.rightBlock.blockShape);
    window.draw(theBorders.topBlock.blockShape);

    // draw the ball
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
    window.display();
} // end render

