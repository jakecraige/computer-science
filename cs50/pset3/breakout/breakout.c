//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

#define PADDLE_WIDTH 50
#define PADDLE_HEIGHT 10
#define PADDLE_BOTTOM_MARGIN 20

#define BRICK_TOP_SPACING 50
#define BRICK_MARGIN 5
#define BRICK_HEIGHT 15
#define BRICK_WIDTH (WIDTH + (BRICK_MARGIN * COLS)) / COLS

#define INITIAL_VELOCITY_MULTIPLIER 2

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

char* colors[] = {
    "MAGENTA",
    "ORANGE",
    "PINK",
    "RED",
    "YELLOW",
    "BLACK",
    "BLUE",
    "CYAN",
    "DARK_GRAY",
    "GRAY",
    "GREEN",
    "LIGHT_GRAY",
    "WHITE",
};

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    double xVelocity = drand48() * INITIAL_VELOCITY_MULTIPLIER;
    double yVelocity = INITIAL_VELOCITY_MULTIPLIER;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // Handle mouse events
        GEvent event = getNextEvent(MOUSE_EVENT);

        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, getY(paddle));
            }
        }

        // Handle ball movement
        move(ball, xVelocity, yVelocity);

        // lose life on bottom edge
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            waitForClick();
            removeGWindow(window, ball);
            ball = initBall(window);
            yVelocity = INITIAL_VELOCITY_MULTIPLIER;
            lives--;
        }

        // Detect collisions

        // bounce off left or right edge of window
        if (getX(ball) <= 0 || getX(ball) + getWidth(ball) >= getWidth(window))
        {
            xVelocity = -xVelocity;
        }
        // bounce off top edge of window
        if (getY(ball) <= 0 || getY(ball) + getHeight(ball) >= getHeight(window))
        {
            yVelocity = -yVelocity;
        }

        GObject object = detectCollision(window, ball);

        if (object != NULL)
        {
            if (object == paddle)
            {
                yVelocity = -yVelocity;
            }
            else if(strcmp(getType(object), "GRect") == 0) // Brick
            {
                removeGWindow(window, object);
                yVelocity = -yVelocity;
                bricks--;
                updateScoreboard(window, label, (ROWS * COLS) - bricks);
            }
        }

        pause(10);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{

    double x = BRICK_MARGIN / 2;
    double y = BRICK_TOP_SPACING;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x, y, BRICK_WIDTH, BRICK_HEIGHT);
            setFilled(brick, true);
            setColor(brick, colors[i]);
            add(window, brick);
            x += BRICK_WIDTH + BRICK_MARGIN;
        }
        // Reset x and y for next row
        y += BRICK_HEIGHT + BRICK_MARGIN;
        x = BRICK_MARGIN / 2;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    double x = (WIDTH - RADIUS) / 2;
    double y = (HEIGHT - RADIUS) / 2;

    GOval ball = newGOval(x, y, RADIUS, RADIUS);
    setFilled(ball, true);
    setColor(ball, colors[5]);
    add(window, ball);

    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    double x = (WIDTH - PADDLE_WIDTH) / 2;
    double y = HEIGHT - PADDLE_HEIGHT - PADDLE_BOTTOM_MARGIN;

    GRect paddle = newGRect(x, y, PADDLE_WIDTH, PADDLE_HEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "black");

    add(window, paddle);

    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{

    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-33");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;

    setLocation(label, x, y);
    add(window, label);

    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
