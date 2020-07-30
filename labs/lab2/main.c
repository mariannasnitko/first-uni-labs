#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <progbase.h>

int main()
{
    const float xmin = -10.0;
    const float xmax = 10.0;
    const float xstep = 0.5;
    float x = xmin;
    float y = 0;
    while (xmin <= x && x <= xmax)
    {
        if (x > -3 && x <= 3)
        {
            y = (cos(x - 5)) / (sin(x - 5)) - 5;
            printf("y(%.1f)= %f\n", x, y);
        }

        else if (x > -3 && x <= 10)
        {
            y = -2 * sqrt(x + 3);
            printf("y(%.1f)= %f\n", x, y);
        }
        else
        {
            printf("y(%.1f)=ERROR\n", x);
        }
        x = x + xstep;
    }
    return 0;
}