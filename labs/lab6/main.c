// gcc main.c -lprogbase -lm
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// libprogbase
#include <progbase.h>
#include <progbase/console.h>
#include <progbase/canvas.h>
#include <time.h>

struct Vec2D
{
    float x;
    float y;
};
struct Color
{
    int red;
    int green;
    int blue;
};
struct ball
{
    struct Vec2D loc;
    float radius;
    float sinRadius;
    struct Color fillColor;
    float radAmplitude;
    float radFrequency;
    float rotRadius;
    float rotAlpha;
    float rotSpeed;
};

int main()
{
    float pi = 3.14159;
    srand(time(0));
    Console_clear();
    struct ConsoleSize consoleSize = Console_size();
    int width = consoleSize.columns;
    int height = consoleSize.rows * 2;
    Canvas_setSize(width, height);
    Canvas_invertYOrientation();
    const int nBalls = 10;
    struct ball balls[nBalls];
    struct Vec2D center = {width / 2, height / 2};
    const int delay = 30;
    float dt = delay / 1000.0;
    float t = 0;

    for (int i = 0; i < nBalls; i++)
    {
        balls[i].radius = rand() % (7 - 2 + 1) + 2;
        balls[i].fillColor.red = rand() % (255 - 0 + 1) + 0;
        balls[i].fillColor.green = rand() % (255 - 0 + 1) + 0;
        balls[i].fillColor.blue = rand() % (255 - 0 + 1) + 0;
        balls[i].rotRadius = rand() % (36 - 15 + 1) + 15;
        balls[i].rotAlpha = (rand() / (float)RAND_MAX) * 2 * pi + (-pi);
        balls[i].rotSpeed = (rand() / (float)RAND_MAX) * 2 * pi + (-pi);
        balls[i].radFrequency = rand() % (25 - 17 + 1) + 17;
        balls[i].radAmplitude = rand() % (5 - 2 + 1) + 2;
    }

    while (!Console_isKeyDown())
    {
        t += dt;
        for (int i = 0; i < nBalls; i++)
        {

            balls[i].loc.x = center.x + cos(balls[i].rotAlpha) * balls[i].sinRadius;
            balls[i].loc.y = center.y + sin(balls[i].rotAlpha) * balls[i].sinRadius;
            balls[i].rotAlpha += balls[i].rotSpeed * dt;
            balls[i].sinRadius = balls[i].rotRadius + sin(balls[i].radFrequency * t) * balls[i].radAmplitude;
        }
        //
        Canvas_beginDraw();
        Canvas_setColorRGB(255, 0, 125);
        Canvas_fillCircle((width / 2), (height / 2), 4);
        Canvas_setColorRGB(125, 5, 125);
        Canvas_putPixel((width / 2), (height / 2));
        for (int i = 0; i < nBalls; i++)
        {
            struct ball ball1 = balls[i];
            Canvas_setColorRGB(ball1.fillColor.red, ball1.fillColor.green, ball1.fillColor.blue);
            Canvas_fillCircle(ball1.loc.x, ball1.loc.y, ball1.radius);
        }
        Canvas_endDraw();
        //
        sleepMillis(delay);
    }
    return 0;
}