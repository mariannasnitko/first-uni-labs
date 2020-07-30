#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <progbase.h>

int main()
{
    float x = 0;
    float y = 0;
    float z = 0;

    printf("Enter x:");
    scanf("%f", &x);

    printf("Enter y:");
    scanf("%f", &y);

    printf("Enter z:");
    scanf("%f", &z);

    if (z != 0 && x != y && x != 0)
    {
        float a0 = pow(x, (y + 1)) / pow((x - y), 1 / z);
        float a1 = (3 * y) + (z / x);
        float a2 = pow((3 + sin(y)), (cos(x) / z) + fabs(x - y));
        float a = a0 + a1 + a2;
        printf("x=%.3f\n", x);
        printf("y=%.3f\n", y);
        printf("z=%.3f\n", z);
        printf("a0=%.3f\n", a0);
        printf("a1=%.3f\n", a1);
        printf("a2=%.3f\n", a2);
        printf("a=%.3f\n", a);
    }
    else
    {
        printf("ERROR\n");
    }

    return 0;
}