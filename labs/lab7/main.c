#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <progbase.h>
#include <progbase/console.h>
#include <progbase/canvas.h>
#include <time.h>
#include <assert.h>
#include <string.h>

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

float length(struct Vec2D v);
struct Vec2D negative(struct Vec2D v);
struct Vec2D add(struct Vec2D a, struct Vec2D b);
struct Vec2D mult(struct Vec2D v, float n);
struct Vec2D norm(struct Vec2D v);
struct Vec2D rotate(struct Vec2D v, float angle);
float distance(struct Vec2D a, struct Vec2D b);
float angle(struct Vec2D v);
struct Vec2D fromPolar(float angle, float length);
int equals(struct Vec2D a, struct Vec2D b);
int fequals(float a, float b);
int randInt(int max, int min);
float randFloat(float max, float min);
void mainTest(void);
//
struct ball createBall(int canvasWidth, int canvasHeight);
struct ball updateBall(struct ball b, float dt, float t, int cWidth, int cHeight);
void drawBall(struct ball ball1);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }
    else if (strcmp(argv[1], "-n") == 0)
    {
        float pi = 3.141592;
        srand(time(0));
        Console_clear();
        struct ConsoleSize consoleSize = Console_size();
        int width = consoleSize.columns;
        int height = consoleSize.rows * 2;
        Canvas_setSize(width, height);
        Canvas_invertYOrientation();
        int nBalls = atoi(argv[2]);
        struct ball balls[nBalls];
        struct Vec2D center = {width / 2, height / 2};
        const int delay = 30;
        float dt = delay / 1000.0;
        float t = 0;

        for (int i = 0; i < nBalls; i++)
        {
            balls[i] = createBall(width, height);
        }

        while (!Console_isKeyDown())
        {
            t += dt;
            for (int i = 0; i < nBalls; i++)
            {
                balls[i] = updateBall(balls[i], dt, t, width, height);
            }
            //
            Canvas_beginDraw();
            Canvas_setColorRGB(255, 0, 125);
            Canvas_fillCircle((width / 2), (height / 2), 4);
            Canvas_setColorRGB(125, 5, 125);
            Canvas_putPixel((width / 2), (height / 2));
            for (int i = 0; i < nBalls; i++)
            {
                drawBall(balls[i]);
            }
            Canvas_endDraw();
            sleepMillis(delay);
        }
        return 0;
    }
    else if (strcmp(argv[1], "-t") == 0)
    {
        mainTest();
    }
}

float length(struct Vec2D v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

struct Vec2D negative(struct Vec2D v)
{
    struct Vec2D neg = v;
    neg.x *= -1;
    neg.y *= -1;
    return neg;
}

struct Vec2D add(struct Vec2D a, struct Vec2D b)
{
    struct Vec2D sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    return sum;
}

struct Vec2D norm(struct Vec2D v)
{
    struct Vec2D normVec = v;
    float locLength = length(v);
    float invLen = (1 / locLength);
    normVec.x *= invLen;
    normVec.y *= invLen;
    return normVec;
}

struct Vec2D mult(struct Vec2D v, float n)
{
    struct Vec2D newVec = v;
    newVec.x *= n;
    newVec.y *= n;
    return newVec;
}

struct Vec2D rotate(struct Vec2D v, float angle)
{
    angle += acos(v.x / length(v));
    float len = length(v);
    struct Vec2D rotated = fromPolar(angle, len);
    return rotated;
}

float angle(struct Vec2D v)
{
    float angle = acos(v.x / length(v));
    return angle;
}

float distance(struct Vec2D a, struct Vec2D b)
{
    float dist = sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
    return dist;
}

int fequals(float a, float b)
{
    return fabs(a - b) < 1e-4;
}

int equals(struct Vec2D a, struct Vec2D b)
{
    return fequals(a.x, b.x) && fequals(a.y, b.y);
}

int randInt(int max, int min)
{
    int r = rand() % (max - min + 1) + min;
    return r;
}

float randFloat(float max, float min)
{
    float r = (rand() / (float)RAND_MAX) * 2 * max + (min);
    return r;
}

struct Vec2D fromPolar(float angle, float length)
{
    struct Vec2D frompolar;
    frompolar.x = length * cos(angle);
    frompolar.y = length * sin(angle);
    return frompolar;
}

struct ball createBall(int canvasWidth, int canvasHeight)
{
    float pi = 3.14159;
    struct ball ball = {
        .radius = randInt(7, 2),
        .fillColor.red = randInt(255, 0),
        .fillColor.green = randInt(255, 0),
        .fillColor.blue = randInt(255, 0),
        .rotRadius = randInt(36, 15),
        .rotAlpha = randFloat(pi, -pi),
        .rotSpeed = randFloat(pi, -pi),
        .radFrequency = randInt(25, 17),
        .radAmplitude = randInt(5, 2),
    };
    return ball;
}

struct ball updateBall(struct ball b, float dt, float t, int cWidth, int cHeight)
{
    struct Vec2D center = {cWidth / 2, cHeight / 2};
    b.loc.x = center.x + fromPolar(b.rotAlpha, b.sinRadius).x;
    b.loc.y = center.y + fromPolar(b.rotAlpha, b.sinRadius).y;
    b.rotAlpha += b.rotSpeed * dt;
    b.sinRadius = b.rotRadius + sin(b.radFrequency * t) * b.radAmplitude;
    return b;
}

void drawBall(struct ball ball1)
{
    Canvas_setColorRGB(ball1.fillColor.red, ball1.fillColor.green, ball1.fillColor.blue);
    Canvas_fillCircle(ball1.loc.x, ball1.loc.y, ball1.radius);
}

void mainTest(void)
{
    struct Vec2D a = {1, 0};
    struct Vec2D b = {2, -2};
    struct Vec2D c = {3, 4};
    assert(equals(a, a));
    assert(fequals(length(a), 1));
    struct Vec2D expected = {-1, 0};
    assert(equals(negative(a), expected));
    struct Vec2D ex = {3, -2};
    assert(equals(add(a, b), ex));
    // norm
    struct Vec2D s = {0.6, 0.8};
    assert(equals(norm(c), s));
    // mult
    struct Vec2D k = {2, 0};
    assert(equals(mult(a, 2), k));
    // distance
    float m = sqrt(5);
    assert(fequals(distance(a, b), m));
    //angle
    assert(fequals(angle(a), acos(1)));
    // fromPolar
    struct Vec2D polar = {length(a) * cos(90), length(a) * sin(90)};
    assert(equals(fromPolar(90, length(a)), polar));
    // rotate
    struct Vec2D rot = {1 * cos(90 + acos(1)), 1 * sin(90 + acos(1))};
    assert(equals(rotate(a, 90), rot));
}