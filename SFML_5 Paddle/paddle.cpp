/* --------------------------------------------------------
 *    File: paddle.cpp
 *  Author: Delphine Sintamour
 *   Class: COP 2001, Spring 2022
 * Purpose: Apply paddle to ball program
 * -------------------------------------------------------- */

#include <cmath>
#include <SFML/Graphics.hpp>
#include "breakout_defs.h"

// Function declarations (prototypes)
Direction processInput() ;
void setup(Ball &ball, Borders &borders, MovingBlock &paddle);
bool update(Direction &input, Ball &ball, float delta, Borders theBorders, MovingBlock &paddle, bool &started);
void render(sf::RenderWindow &window, Ball ball, float delta, Borders theBorder, MovingBlock paddle);
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

    //declare a variable of Borders
    Borders theBlocks;

    // declare a ball variable
    Ball theBall;

    //declare a paddle of type Moving Blocks
    MovingBlock thePaddle;

    //initialize both the ball and the borders
    setup(theBall, theBlocks, thePaddle);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;

    //used for start flag
    bool started = false;

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

            gameOver = update(userInput, theBall, delta, theBlocks, thePaddle, started);

            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        render(window, theBall, delta, theBlocks, thePaddle);

    } // end main game loop

    // game ending, close the graphics window

    window.close();
    return 0;   // return success to the OS
} // end main

/**
 * All the initialized values for the ball, walls, paddle, ets
 * @param ball - the ball
 * @param borders - the walls
 * @param paddle - the paddle
 */
void setup(Ball &ball, Borders &borders, MovingBlock &paddle) {
    //initialize leftBlock
    borders.leftBlock.left = 0.0;
    borders.leftBlock.top = 0.0;
    borders.leftBlock.width = BLOCK_THICKNESS;
    borders.leftBlock.height = WINDOW_HEIGHT;
    borders.leftBlock.blockColor = BLOCK_COLOR;
    borders.leftBlock.blockShape.setSize(sf::Vector2(borders.leftBlock.width, borders.leftBlock.height));
    borders.leftBlock.blockShape.setPosition(borders.leftBlock.left, borders.leftBlock.top);
    borders.leftBlock.blockShape.setFillColor(borders.leftBlock.blockColor);

    //initialize topBlock
    borders.topBlock.left = 0.0;
    borders.topBlock.top = 0.0;
    borders.topBlock.width = WINDOW_WIDTH;
    borders.topBlock.height = BLOCK_THICKNESS;
    borders.topBlock.blockColor = BLOCK_COLOR;
    borders.topBlock.blockShape.setSize(sf::Vector2(borders.topBlock.width, borders.topBlock.height));
    borders.topBlock.blockShape.setPosition(borders.topBlock.left, borders.topBlock.top);
    borders.topBlock.blockShape.setFillColor(borders.topBlock.blockColor);

    //initialize rightBlock
    borders.rightBlock.left = WINDOW_WIDTH-BLOCK_THICKNESS;
    borders.rightBlock.top = 0.0;
    borders.rightBlock.width = BLOCK_THICKNESS;
    borders.rightBlock.height = WINDOW_HEIGHT;
    borders.rightBlock.blockColor = BLOCK_COLOR;
    borders.rightBlock.blockShape.setSize(sf::Vector2(borders.rightBlock.width, borders.rightBlock.height));
    borders.rightBlock.blockShape.setPosition(borders.rightBlock.left, borders.rightBlock.top);
    borders.rightBlock.blockShape.setFillColor(borders.rightBlock.blockColor);

    //initialize bottomBlock
    borders.bottomBlock.left = 0.0;
    borders.bottomBlock.top = WINDOW_HEIGHT - BLOCK_THICKNESS;
    borders.bottomBlock.width = WINDOW_WIDTH;
    borders.bottomBlock.height = BLOCK_THICKNESS;
    borders.bottomBlock.blockColor = BLOCK_COLOR;
    borders.bottomBlock.blockShape.setSize(sf::Vector2(borders.bottomBlock.width, borders.bottomBlock.height));
    borders.bottomBlock.blockShape.setPosition(borders.bottomBlock.left, borders.bottomBlock.top);
    borders.bottomBlock.blockShape.setFillColor(borders.bottomBlock.blockColor);

    //initialize the paddle
    paddle.theMovingBlock.left = (WINDOW_WIDTH - PADDLE_WIDTH)/2.0;
    paddle.theMovingBlock.top = WINDOW_HEIGHT - (2.0 * PADDLE_THICKNESS);
    paddle.theMovingBlock.width = PADDLE_WIDTH;
    paddle.theMovingBlock.height = PADDLE_THICKNESS;
    paddle.theMovingBlock.blockColor = PADDLE_COLOR;
    paddle.theMovingBlock.blockShape.setSize(sf::Vector2(paddle.theMovingBlock.width, paddle.theMovingBlock.height));
    paddle.theMovingBlock.blockShape.setFillColor(PADDLE_COLOR);
    paddle.velocityX = 0.0;
    paddle.velocityY = 0.0;

    //initialize ball
    ball.radius = BALL_RADIUS;
    ball.coordinateX = paddle.theMovingBlock.left + (paddle.theMovingBlock.width / 2.0);
    ball.coordinateY = paddle.theMovingBlock.top - ball.radius - 1;
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;

};


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
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        input = Start;
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
 * @param paddle - properties of paddle
 * @param theBorders - properties for walls
 * @param started - start flag bool variable
 */
bool update(Direction &input, Ball &ball, float delta, Borders theBorders, MovingBlock &paddle, bool &started) {

    //gameOver condition
    bool gameOver = false;

    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            case Left: // Left
                paddle.velocityX -= PADDLE_SPEED;
                break;
            case Right: // Right
                paddle.velocityX += PADDLE_SPEED;
                break;

            case Start: //Start
                if(!started) {
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y * -1;    //start moving upwards

                    /* randomly set horizontal velocity to positive or negative
                        by seeing if the tenths place of current delta
                        (i.e. 1st digit after the decimal) is odd or even.  */
                    if ((int(delta * 10) & 1) % 2) {
                        ball.velocityX *= -1;  // ball goes left if odd
                    }

                    started = true;
                }

            /*case Up: // Up
                paddle.velocityY -= BALL_SPEED_Y;
                break;
            */
            /*case Down: // Down
                paddle.velocityY += BALL_SPEED_Y;
            */
        }
        // clear input
        input = None;
    }

    //adjust the paddle location
    paddle.theMovingBlock.left += paddle.velocityX * delta;

    if (started) {
        // adjust the location of the ball for speed * time
        ball.coordinateX += ball.velocityX*delta;
        ball.coordinateY += ball.velocityY*delta;
    }
    else {
        //fixes ball's coordinates relative to the player's paddle
        ball.coordinateX = paddle.theMovingBlock.left + (paddle.theMovingBlock.width / 2.0);
        ball.coordinateY = paddle.theMovingBlock.top - ball.radius - 1;
    }

    //collision check
    //check vertical collisions
    if(!collisionCheck(&ball, &paddle.theMovingBlock)) {
        if(!collisionCheck(&ball, &(theBorders.topBlock))) {
            gameOver = collisionCheck(&ball, &(theBorders.bottomBlock));
        }
    }

    //check horizontal collisions
    if(!collisionCheck(&ball, &(theBorders.leftBlock))) {
        collisionCheck(&ball, &(theBorders.rightBlock));
    }

    return gameOver;
} // end update

/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 * @param theBorders - structure that contains the properties for the walls
 * @param paddle - structure variable with properties for paddle
 */
void render(sf::RenderWindow &window, Ball ball, float delta, Borders theBorders, MovingBlock paddle) {
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

    //calculate coordinates of the player paddle block
    paddle.theMovingBlock.left += paddle.velocityX * delta;
    paddle.theMovingBlock.top += paddle.velocityY * delta;
    paddle.theMovingBlock.blockShape.setPosition(paddle.theMovingBlock.left, paddle.theMovingBlock.top);
    window.draw(paddle.theMovingBlock.blockShape);

    // show the new window
    window.display();
} // end render

