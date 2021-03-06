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

// height and width of game's paddle in pixels
#define PHEIGHT 10
#define PWIDTH 55

// height and width of the ball in pixels
#define BWIDTH WIDTH/2 - PWIDTH/2 + 15
#define BHEIGHT WIDTH/2 - PWIDTH/2 + 15    

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

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

    double drand48(void);
    double xVelo = drand48() + 1; 
    double yVelo = drand48() + 2; 
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        move(ball, xVelo, yVelo);
        
        pause(10);
        
        GObject object = detectCollision(window, ball);
        
        if(object != NULL)
        {
            if (object == paddle)
            {
                yVelo = -yVelo;
            }
            else if(strcmp(getType(object), "GRect") == 0 && object != paddle)
            {
                removeGWindow(window, object);
                points++;
                bricks--;
                updateScoreboard(window, label, points);
                yVelo = -yVelo;
            }
            else if(strcmp(getType(object), "GLabel") == 0)
            {
                continue;
            }
        }
        
        if(getX(ball) <= 0)
        {
            xVelo = -xVelo;
        }
        
        else if(getY(ball) <= 0)
        {
            yVelo = -yVelo;
        }
        else if (getX(ball) + getWidth(ball) >= getWidth(window) || getX(ball) <= 0)
        {
            xVelo = -xVelo;
        }
        else if(getY(ball) > HEIGHT)
        {
            lives--;
            setLocation(ball, (WIDTH/2), (HEIGHT/2));
            setLocation(paddle, 172, 525);
            waitForClick();
        }
     
        
        GEvent event = getNextEvent(MOUSE_EVENT);

        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure circle follows top cursor
                double x = getX(event) - getWidth(paddle) / 2;
                double y = 525;
                setLocation(paddle, x, y);
            }
        }
            
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

//Initializes window with a grid of bricks.
void initBricks(GWindow window)
{
    int xDistance = 2;
    int yDistance = 30;    
    string rowColors[5];
    rowColors[0] = "RED";
    rowColors[1] = "MAGENTA";
    rowColors[2] = "ORANGE";
    rowColors[3] = "YELLOW";
    rowColors[4] = "GREEN";
        
    for(int row = 0; row < ROWS; row++)
    {
        for(int col = 0; col < COLS; col++, xDistance += 40)
        {
            GRect brick = newGRect(xDistance, yDistance, WIDTH/11, 10);
            setFilled(brick, true);
            setColor(brick, rowColors[row]);
            add(window, brick);
        }
        
        xDistance = 2;
        yDistance += 20;
    }
}

//Instantiates ball in center of window.  Returns ball.
GOval initBall(GWindow window)
{
    //X and Y are at top left of ball - offset by size of ball, 20. 15 seems more comfy.
    //int bX = ((WIDTH/2) - (PWIDTH/2))+15;
    //int bY = (HEIGHT/2) - (PWIDTH/2);
    
    GOval ball = newGOval(BWIDTH, BHEIGHT, 20, 20);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    //move(ball, 172, 150); REDUNDANT
    return ball;
}

// Instantiates paddle in bottom-middle of window.
GRect initPaddle(GWindow window)
{
    //location = Window divided by 2 - paddle divided by 2?
    //Y is fixed - 525 is comfy
    
    int pX = (WIDTH/2) - (PWIDTH/2);
    
    GRect paddle = newGRect(pX, 525, 55, 10);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    //setLocation(paddle, 172, 525); REDUNDANT
    
    return paddle;
}

//Instantiates, configures, and returns label for scoreboard.
GLabel initScoreboard(GWindow window)
{
    int points = 0;
    
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    setLocation(label, 25, 25);
    add(window, label);
    return label;
    
    //updateScoreboard(window, label, points);
}

//Updates scoreboard's label, keeping it centered in window.
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
