#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// libprogbase
#include <progbase.h>
#include <progbase/console.h>
#include <progbase/canvas.h>
#include <time.h>
#include <stdbool.h>

void mainMenu(void)
{
    printf("1. Array\n2. Matrix\n3. Quit\n");
}
void menuArr(void)
{
    printf("1. Fill array with random numbers from L to H\n");
    printf("2. Reset array\n");
    printf("3. Find maximal element of array and its index\n");
    printf("4. Output the sum of positive elements\n");
    printf("5. Swap places of maximal and minimal elements of array\n");
    printf("6. Multiply all elements of array by entered number\n");
    printf("7. < Back to main menu\n");
}
void menuMatrix(void)
{
    printf("1. Fill matrix with random numbers from L to H\n");
    printf("2. Reset matrix\n");
    printf("3. Find minimal element of matrix and its indexes i and j\n");
    printf("4. Output the sum of elements of row\n");
    printf("5. Swap places of maximal and minimal elements of matrix\n");
    printf("6. Replace element\n");
    printf("7. < Back to main menu\n");
}
int getColor(char colorCode);

void clear()
{
    while (getchar() != '\n')
        ;
}

const char image[28][28] = {
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF},
    {0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0xF, 0x0, 0x4, 0x4, 0x4, 0x4, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF},
    {0xF, 0xF, 0xF, 0xF, 0xF, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF},
    {0xF, 0xF, 0xF, 0xF, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF},
    {0xF, 0xF, 0x0, 0x0, 0x4, 0x4, 0x4, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x4, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x4, 0x0, 0x0, 0xF, 0xF, 0xF},
    {0xF, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x4, 0x4, 0x0, 0x4, 0x4, 0x4, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0xF, 0xF},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0x0, 0xF},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0xF, 0x0, 0x0, 0x0, 0xF, 0xF, 0x0, 0xF, 0x0, 0x0, 0xF, 0xF, 0x0, 0xF, 0xF, 0x0},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0xF, 0xF, 0x0, 0x0, 0xF, 0x0, 0x0, 0x0, 0x0, 0xF, 0x0, 0xF, 0x0, 0x0, 0xF, 0xF, 0x0, 0xF},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0xF, 0xF, 0x0, 0x0, 0xF, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0xF},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0xF, 0xF, 0xF},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF},
    {0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x2, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x0},
    {0xF, 0x0, 0x0, 0x4, 0x4, 0x4, 0x0, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF},
    {0xF, 0xF, 0xF, 0x0, 0x0, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0xF, 0xF, 0xF, 0xF},
    {0xF, 0xF, 0xF, 0xF, 0xF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1},
    {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1}};

int main()
{
    int L = 0;
    int H = 0;
    int N = 0;
    int M = 0;
    int index = 0;
    int number = 0;
    int n = 0;
    float max = 0;
    int min = 0;
    int minInd1 = 0;
    int minInd2 = 0;
    int row = 0;
    float maxim = 0;
    float minim = 0;
    int maxInd = 0;
    int minInd = 0;

    srand(time(0));

    bool isMainMenuRunning = true;
    while (isMainMenuRunning)
    {
        Console_clear();
        mainMenu();
        char mainMenuUserInput = Console_getChar();

        switch (mainMenuUserInput)
        {
        case '3':
        {
            isMainMenuRunning = false;
            break;
        }
        case '1':
        {
            Console_clear();
            int n = 0;
            printf("Enter array length: ");
            scanf("%d", &n);
            clear();
            while (n < 0)
            {
                printf("Enter array length: ");
                scanf("%d", &n);
                clear();
                Console_clear();
            };

            float arr[n];
            for (int i = 0; i < n; i++)
            {
                arr[i] = 0;
            }
            bool isSubmenu1Running = true;
            while (isSubmenu1Running)
            {
                puts("");
                puts("Array: ");
                for (int i = 0; i < n; i++)
                {
                    printf("%.2f  ", arr[i]);
                }
                puts(" ");
                menuArr();
                char submenu1Input = Console_getChar();
                Console_clear();

                switch (submenu1Input)
                {
                case '7':
                {
                    isSubmenu1Running = false;
                    break;
                }
                case '1':
                {
                    do
                    {
                        Console_clear();
                        printf("Enter L-min: ");
                        scanf("%i", &L);
                        puts("");
                        printf("Enter H-max: ");
                        scanf("%i", &H);
                    } while (H - L < 0);
                    Console_clear();
                    for (int i = 0; i < n; i++)
                    {
                        arr[i] = L + (rand() / (float)RAND_MAX) * (H - L);
                    }

                    break;
                }
                case '2':
                {
                    for (int i = 0; i < n; i++)
                    {
                        arr[i] = 0;
                    }
                    break;
                }
                case '3':
                {
                    max = arr[0];
                    for (int i = 0; i < n; i++)
                    {
                        if (arr[i] > max)
                        {
                            max = arr[i];
                            index = i;
                        }
                    }
                    printf("Index: [%d]\nElement: %.3f\n", index, max);
                    break;
                }
                case '4':
                {
                    float sum = 0;
                    for (int i = 0; i < n; i++)
                    {
                        if (arr[i] > 0)
                        {
                            sum += arr[i];
                        }
                    }
                    printf("Sum = %5.2f\n", sum);
                    break;
                }
                case '5':
                {
                    maxim = arr[0];
                    minim = arr[0];
                    for (int i = 0; i < n; i++)
                    {
                        if (arr[i] > maxim)
                        {
                            maxim = arr[i];
                            maxInd = i;
                        }
                        else if (arr[i] < minim)
                        {
                            minim = arr[i];
                            minInd = i;
                        }
                    }
                    float tmp = arr[maxInd];
                    arr[maxInd] = arr[minInd];
                    arr[minInd] = tmp;
                    break;
                }
                case '6':
                {
                    printf("Multiplier:\n");
                    scanf("%i", &number);
                    for (int i = 0; i < n; i++)
                    {
                        arr[i] *= number;
                    }
                    break;
                }
                default:
                {
                    printf("Key does not exist\n");
                    break;
                }
                }
            }
            break;
        }
        case '2':
        {
            Console_clear();
            int N = 0;
            int M = 0;
            do
            {
                Console_clear();
                printf("Enter N - rows: ");
                scanf("%i", &N);
                puts("");
                printf("Enter M - columns: ");
                scanf("%i", &M);
                Console_clear();
            } while (M < 1 || N < 1);

            int matrix[N][M];
            for (int n = 0; n < N; n++)
            {
                for (int m = 0; m < M; m++)
                {
                    matrix[n][m] = 0;
                }
            }

            bool isSubmenu2Running = true;
            while (isSubmenu2Running)
            {
                puts("");
                puts("Matrix:\n");
                for (int n = 0; n < N; n++)
                {
                    for (int m = 0; m < M; m++)
                    {
                        printf("%5i ", matrix[n][m]);
                    }
                    printf("\n");
                }
                puts("");
                menuMatrix();
                char submenu2Input = Console_getChar();
                Console_clear();
                switch (submenu2Input)
                {
                case '7':
                {
                    isSubmenu2Running = false;
                    break;
                }
                case '1':
                {
                    do
                    {
                        Console_clear();
                        printf("Enter L-min: ");
                        scanf("%i", &L);
                        puts("");
                        printf("Enter H-max: ");
                        scanf("%i", &H);
                        Console_clear();
                    } while (H - L < 0);

                    for (int n = 0; n < N; n++)
                    {
                        for (int m = 0; m < M; m++)
                        {
                            matrix[n][m] = rand() % (H - L + 1) + L;
                        }
                    }
                    break;
                }
                case '2':
                {
                    for (int n = 0; n < N; n++)
                    {
                        for (int m = 0; m < M; m++)
                        {
                            matrix[n][m] = 0;
                        }
                    }
                    break;
                }
                case '3':
                {
                    min = matrix[0][0];

                    for (int n = 0; n < N; n++)
                    {
                        for (int m = 0; m < M; m++)
                        {
                            if (min > matrix[n][m])
                            {
                                min = matrix[n][m];
                                minInd1 = n;
                                minInd2 = m;
                            }
                        }
                    }
                    printf("Indexes: [%i][%i]  Element: %i\n", minInd1, minInd2, min);
                    break;
                }
                case '4':
                {
                    int plus = 0;
                    Console_clear();
                    printf("Index of row: ");
                    scanf("%d", &row);
                    puts("");
                    while (row < 0 || row > N - 1)
                    {
                        Console_clear();
                        printf("Enter valid index\n");
                        scanf("%d", &row);
                    }
                    for (int n = 0; n < M; n++)
                    {
                        plus = plus + matrix[row][n];
                    }
                    printf("Sum of elements at %d = %i\n", row, plus);
                    break;
                }
                case '5':
                {
                    int maxMat = 0;
                    int minMat = 0;
                    int indMax1 = 0;
                    int indMin1 = 0;
                    int indMax2 = 0;
                    int indMin2 = 0;
                    minMat = matrix[0][0];
                    maxMat = matrix[0][0];
                    for (int n = 0; n < N; n++)
                    {
                        for (int m = 0; m < M; m++)
                        {

                            if (matrix[n][m] > maxMat)
                            {
                                maxMat = matrix[n][m];
                                indMax1 = n;
                                indMax2 = m;
                            }
                            else if (matrix[n][m] < minMat)
                            {
                                minMat = matrix[n][m];
                                indMin1 = n;
                                indMin2 = m;
                            }
                        }
                    }
                    int tmp1 = matrix[indMax1][indMax2];
                    matrix[indMax1][indMax2] = matrix[indMin1][indMin2];
                    matrix[indMin1][indMin2] = tmp1;
                    break;
                }
                case '6':
                {
                    int indRow;
                    int indCol;
                    int digit;
                    printf("Enter index of row: ");
                    scanf("%i", &indRow);
                    while (indRow < 0 || indRow > N - 1)
                    {
                        Console_clear();
                        printf("Enter valid index\n");
                        scanf("%i", &indRow);
                    }
                    printf("Enter index of column: ");
                    scanf("%i", &indCol);
                    while (indCol < 0 || indCol > M - 1)
                    {
                        printf("Enter valid index\n");
                        scanf("%i", &indCol);
                    }
                    printf("Digit: ");
                    scanf("%i", &digit);
                    for (int n = 0; n < N; n++)
                    {
                        for (int m = 0; m < M; m++)
                        {
                            matrix[indRow][indCol] = digit;
                        }
                    }
                    break;
                }
                default:
                {
                    printf("Key does not exist\n");
                    break;
                }
                }
            }
            break;
        }
        case '4':
        {
            Console_clear();
            bool consoleDrawing = true;
            while (consoleDrawing)
            {
                for (int i = 0; i < 28; i++)
                {
                    for (int j = 0; j < 28; j++)
                    {
                        int color = getColor(image[i][j]);
                        Console_setCursorAttribute(color);
                        Console_setCursorPosition(i, j);
                        printf("  ");
                        printf("%c", image[i][j]);
                    }
                    Console_reset();
                    puts("\n");
                }

                printf("1. Back to main menu\n");
                int exitMenu3 = 0;
                scanf("%i", &exitMenu3);
                if (exitMenu3 == 1)
                {
                    consoleDrawing = false;
                }
                else
                {
                    isMainMenuRunning = false;
                }
            }
            break;
        }
        }
    }

    puts("Bye!");
    return 0;
}
int getColor(char colorCode)
{
    // colors encoding table (hex code -> console color)
    const char colorsTable[16][2] = {
        {0x0, BG_BLACK},
        {0x1, BG_INTENSITY_BLACK},
        {0x2, BG_RED},
        {0x3, BG_INTENSITY_RED},
        {0x4, BG_GREEN},
        {0x5, BG_INTENSITY_GREEN},
        {0x6, BG_YELLOW},
        {0x7, BG_INTENSITY_YELLOW},
        {0x8, BG_BLUE},
        {0x9, BG_INTENSITY_BLUE},
        {0xa, BG_MAGENTA},
        {0xb, BG_INTENSITY_MAGENTA},
        {0xc, BG_CYAN},
        {0xd, BG_INTENSITY_CYAN},
        {0xe, BG_WHITE},
        {0xF, BG_INTENSITY_WHITE}};
    const int tableLength = sizeof(colorsTable) / sizeof(colorsTable[0]);
    for (int i = 0; i < tableLength; i++)
    {
        char colorPairCode = colorsTable[i][0];
        char colorPairColor = colorsTable[i][1];
        if (colorCode == colorPairCode)
        {
            return colorPairColor; // we have found our color
        }
    }
    return 0; // it's an error
}