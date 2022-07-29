/* --------------------------------------------------------
 *    File: breakout_defs.h
 *  Author: Delphine Sintamour
 *   Class: COP 2001, Spring 2022
 * Purpose: definitions for breakout game
 * -------------------------------------------------------- */

#include <SFML/Graphics.hpp>

#ifndef SFML_5_PADDLE_BREAKOUT_DEFS_H
#define SFML_5_PADDLE_BREAKOUT_DEFS_H

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

//paddle properties
const float PADDLE_WIDTH = 80.0;
const float PADDLE_THICKNESS = 10.0;
sf::Color PADDLE_COLOR = sf::Color::White;
const float PADDLE_SPEED = PADDLE_WIDTH/2.5/1000.0;

//brick constants
const int BRICK_ROWS = 8;                                                       //rows of bricks in rack
const int BRICK_COLUMNS = 14;                                                   //number of bricks in row
const float BRICK_WIDTH = WINDOW_WIDTH / BRICK_COLUMNS;                         //width of 1 brick
const float BRICK_HEIGHT = PADDLE_THICKNESS * 2;                                //height of 1 brick (2 x paddle)
const float BRICKS_HEIGHT = BRICK_ROWS * BRICK_HEIGHT;                          //height of the rack of bricks
const float BRICKS_TOP = WINDOW_HEIGHT / 5.0 - BRICK_HEIGHT * 0.75;             //top of the rack (shift up 3/4 from center)
const float BRICKS_LEFT = BLOCK_THICKNESS;                                      //left of first brick in each row
const float FIRST_BRICK = BRICKS_TOP + (BRICK_ROWS - 1) * BRICK_HEIGHT;         //top of the lowest brick in rack


// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float BALL_RADIUS = 10.0;                     //modification for high velocity
const float BALL_SPEED_X = BALL_RADIUS * 15.0/1000.0;    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS * 15.0/1000.0;   // span  vertically

// Type definitions
enum Direction {
    Exit = -1,
    None,
    Left,
    Up,
    Right,
    Down,
    Start
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

//paddle motion properties
struct MovingBlock {
    Block theMovingBlock;
    float velocityX;
    float velocityY;
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

//brick properties
struct Brick {
    Block block;
    bool hit;
    int points;
    float speedAdjust;
};


#endif //SFML_5_PADDLE_BREAKOUT_DEFS_H
