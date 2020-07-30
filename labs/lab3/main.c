#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <progbase.h>
#include <progbase/console.h>

int main()
{

    int x = 0;
    int y = 0;
    //for border rectangle
    const int x0 = 1;
    const int y0 = 1;
    const int width = 60;
    const int height = 40;
    //coordinates for top left corner of A
    int ax0 = 5;
    int ay0 = 10;
    const int aw = 30;
    const int ah = 20;
    //coordinates for top left corner of B
    int bx0 = 16;
    int by0 = 18;
    const int bw = 15;
    const int bh = 10;

    int key = 0;

    do //start of the loop
    {
        Console_clear();
        //drawing horizontal lines
        for (x = x0; x <= x0 + width; x++)
        {
            y = y0;
            Console_setCursorPosition(y, x);
            printf(".");
            y = y0 + height;
            Console_setCursorPosition(y, x);
            printf(".");
        }
        //drawing vertical lines
        for (y = y0; y <= y0 + height; y++)
        {
            x = x0;
            Console_setCursorPosition(y, x);
            printf(".");
            x = x0 + width;
            Console_setCursorPosition(y, x);
            printf(".");
        }

        Console_setCursorAttribute(BG_GREEN);
        for (x = ax0; x < ax0 + aw; x++)
        {
            for (y = ay0; y < ay0 + ah; y++)
            {
                Console_setCursorPosition(y, x);
                putchar(' ');
            }
        }

        Console_setCursorAttribute(BG_MAGENTA);
        for (x = bx0; x < bx0 + bw; x++)
        {
            for (y = by0; y < by0 + bh; y++)
            {
                Console_setCursorPosition(y, x);
                putchar(' ');
            }
        }

        key = Console_getChar();

        switch (key)
        {

        //for rectangle A
        
        case 'w':
        {
            if (ay0 <= 2)
            {}
            else
            {
                ay0 -= 1;
                by0 -= 1;
            }
            Console_setCursorPosition(y, x);
        }
            break;

        case 'a':
        {
            if (ax0 <= 2)
            {
                ax0 -= 1;
                bx0 -= 1;
            }
            Console_setCursorPosition(y, x); 
        }
            break;

        case 's':
        {
            if (ay0 + ah != y0 + height)
            {
                ay0 += 1;
                by0 += 1;
            }
            Console_setCursorPosition(y, x);
        }
            break;

        case 'd':
        {
            if (ax0 + aw != x0 + width)
            {
                ax0 += 1;
                bx0 += 1;
            }
            Console_setCursorPosition(y, x);
        }
            break;

        // for rectangle B
        case 'i':
        {
            if (by0 < ay0)
            {
                by0 -= 1;
            }
            Console_setCursorPosition(y, x);
        }
            break;

        case 'j':
        {
            if (bx0 < ax0)
            {
                bx0 -= 1;
            }
            Console_setCursorPosition(y, x);
        }
            break;

        case 'k':
        {
            if (by0 - bh > ay0)
            {
                by0 += 1;
            }
            Console_setCursorPosition(y, x);
        }
            break;

        case 'l':
        {
            if (bx0 - bw > ax0)
            {
                bx0 += 1;
            }
            Console_setCursorPosition(y, x);
            break;
        }
            
        }

        Console_setCursorPosition(y0 + height, 1);
        Console_reset();
        puts("");

    }

    while (key != '\n'); // press enter to end loop

    return 0;
}
