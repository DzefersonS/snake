#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN 0
#define MAX 24

struct tail
{
    int x;
    int y;
};

int MAP[MAX][MAX];
struct tail SNAKE[MAX * MAX]; // tail size, X of each bit, Y of each bit;
int SNAKE_LENGTH = 1;
int MOVEMENT_DIR = 0;
bool FOOD_EATEN;
int SCORE = 0;
int DELAY_MS;

void initialize_generator(void);
void initialize_map(void);
void print_map(void);
int check_sum(int row);
void print_commands(int row);
bool move_snake(void);
void delay(int miliseconds);
void read_input(char ch);
bool move_snake_valid(int i);
void generate_food(void);
void grow_snake(void);
bool check_if_eat();

int main(void)
{
    initialize_generator();
    initialize_map();
    char ch;
    int command;
    SNAKE[0].x = rand() % (MAX - 3) + 2;
    SNAKE[0].y = rand() % (MAX - 3) + 2;
    MAP[SNAKE[0].x][SNAKE[0].y] = 1;
    printf("\e[1;1H\e[2J");
    printf("Before the game begins, choose a difficulty:\n");
    printf("1 - 500ms delay between moves;\n");
    printf("2 - 300ms delay between moves;\n");
    printf("3 - 100ms delay between moves.\n");
    printf("Enter your preferred difficulty: ");
    for (;;)
    {
        if (scanf("%d", &command) == 1 && getchar() == '\n')
        {
            if (command < 1 || command > 3)
            {
                printf("Invalid input. Enter again!\n");
            }
            else
            {
                if (command == 1)
                    DELAY_MS = 500;
                else if (command == 2)
                    DELAY_MS = 300;
                else
                    DELAY_MS = 100;
                break;
            }
        }
        else
        {
            printf("Invalid input. Enter again!\n");
            while (getchar() != '\n')
                ;
        }
    }
    generate_food();
    print_map();
    for (;;)
    {
        if (kbhit())
        {
            ch = getch();
            read_input(ch);
            if (ch == 'h' || ch == 'j' || ch == 'k' || ch == 'l')
                break;
        }
    }

    for (;;)
    {
        if (kbhit())
        {
            ch = getch();
            read_input(ch);
        }
        // while (!kbhit())
        //{
        if (move_snake() == true)
        {
            print_map();
        }
        else
        {
            break;
        }
        //}

        delay(DELAY_MS);
    }
    return 0;
}

void initialize_map(void)
{
    for (int i = 0; i < MAX; ++i)
    {
        for (int j = 0; j < MAX; ++j)
        {
            MAP[i][j] = 0;
        }
    }
}

void print_map(void)
{
    printf("\e[1;1H\e[2J");
    printf("##########################\n");
    for (int i = 0; i < MAX; ++i)
    {
        printf("#");
        if (check_sum(i) == 0)
        {
            printf("%*c", 24, ' ');
        }
        else
        {
            for (int j = 0; j < MAX; ++j)
            {
                if (MAP[i][j] == 0)
                {
                    printf(" ");
                }
                else if (MAP[i][j] == 1)
                {
                    printf("o");
                }
                else if (MAP[i][j] == 2)
                {
                    printf("X");
                }
            }
        }
        printf("#");
        print_commands(i);
    }
    printf("##########################\n");
}

int check_sum(int row)
{
    int sum = 0;
    for (int i = MIN; i < MAX; ++i)
    {
        sum += MAP[row][i];
    }
    return sum;
}

void print_commands(int row)
{
    if (row == 1)
        printf(" Welcome to Snake!");
    else if (row == 2)
        printf(" Commands are listed below!");
    else if (row == 3)
        printf(" Author: DzefersonS");
    else if (row == 4)
        printf(" Version: 1.0");
    else if (row == 5)
    {
        if (DELAY_MS == 500)
            printf(" Chosen difficulty: 1");
        else if (DELAY_MS == 300)
            printf(" Chosen difficulty: 2");
        else
            printf(" Chosen difficulty: 3");
    }
    else if (row == 6)
        printf(" Press any direction key to begin...");
    else if (row == 9)
        printf(" Commands: ");
    else if (row == 10)
        printf(" h - move left");
    else if (row == 11)
        printf(" j - move up");
    else if (row == 12)
        printf(" k - move down");
    else if (row == 13)
        printf(" l - move right");
    else if (row == 14)
        printf(" esc - exit program");
    else if (row == 18)
        printf(" SCORE: %d", SCORE);

    printf("\n");
}

bool move_snake(void)
{
    int x1, y1;
    int x2 = SNAKE[0].x;
    int y2 = SNAKE[0].y;
    FOOD_EATEN = false;
    if (move_snake_valid(0) == false)
    {
        return false;
    }

    for (int i = 1; i < SNAKE_LENGTH; ++i)
    {
        MAP[SNAKE[i].x][SNAKE[i].y] = 0;
        x1 = SNAKE[i].x;
        y1 = SNAKE[i].y;
        SNAKE[i].x = x2;
        SNAKE[i].y = y2;
        MAP[SNAKE[i].x][SNAKE[i].y] = 1;
        x2 = x1;
        y2 = y1;
    }
    if (FOOD_EATEN == true)
    {
        generate_food();
    }
    return true;
}

bool move_snake_valid(int i)
{
    if (MOVEMENT_DIR == 1)
    {
        if (SNAKE[i].y != (MAX - 1) && MAP[SNAKE[i].x][SNAKE[i].y + 1] != 1)
        {
            MAP[SNAKE[i].x][SNAKE[i].y] = 0;
            if (check_if_eat() == true)
            {
                grow_snake();
                FOOD_EATEN = true;
            }
            SNAKE[i].y += 1;
            MAP[SNAKE[i].x][SNAKE[i].y] = 1;
        }
        else
        {
            return false;
        }
    }
    else if (MOVEMENT_DIR == 2)
    {
        if (SNAKE[i].y != MIN && MAP[SNAKE[i].x][SNAKE[i].y - 1] != 1)
        {
            MAP[SNAKE[i].x][SNAKE[i].y] = 0;
            if (check_if_eat() == true)
            {
                grow_snake();
                FOOD_EATEN = true;
            }
            SNAKE[i].y -= 1;
            MAP[SNAKE[i].x][SNAKE[i].y] = 1;
        }
        else
        {
            return false;
        }
    }
    else if (MOVEMENT_DIR == 3)
    {
        if (SNAKE[i].x != MIN && MAP[SNAKE[i].x - 1][SNAKE[i].y] != 1)
        {
            MAP[SNAKE[i].x][SNAKE[i].y] = 0;
            if (check_if_eat() == true)
            {
                grow_snake();
                FOOD_EATEN = true;
            }
            SNAKE[i].x -= 1;
            MAP[SNAKE[i].x][SNAKE[i].y] = 1;
        }
        else
        {
            return false;
        }
    }
    else if (MOVEMENT_DIR == 4)
    {
        if (SNAKE[i].x != (MAX - 1) && MAP[SNAKE[i].x + 1][SNAKE[i].y] != 1)
        {
            MAP[SNAKE[i].x][SNAKE[i].y] = 0;
            if (check_if_eat() == true)
            {
                grow_snake();
                FOOD_EATEN = true;
            }
            SNAKE[i].x += 1;
            MAP[SNAKE[i].x][SNAKE[i].y] = 1;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void delay(int miliseconds)
{
    clock_t start_time = clock();

    while ((clock()) < start_time + miliseconds)
        ;
}

void read_input(char ch)
{
    if (ch == 'l' && MOVEMENT_DIR != 2)
    {
        MOVEMENT_DIR = 1;
    }
    else if (ch == 'h' && MOVEMENT_DIR != 1)
    {
        MOVEMENT_DIR = 2;
    }
    else if (ch == 'j' && MOVEMENT_DIR != 4)
    {
        MOVEMENT_DIR = 3;
    }
    else if (ch == 'k' && MOVEMENT_DIR != 3)
    {
        MOVEMENT_DIR = 4;
    }
    else if ((int)ch == 27)
    {
        exit(0);
    }
}

void initialize_generator(void)
{
    srand((unsigned)time(NULL));
}

void generate_food(void)
{
    int x;
    int y;
    for (;;)
    {
        x = rand() % MAX;
        y = rand() % MAX;
        if (MAP[x][y] == 0)
        {
            MAP[x][y] = 2;
            return;
        }
    }
}

bool check_if_eat()
{
    if (MOVEMENT_DIR == 1)
    {
        if (MAP[SNAKE[0].x][SNAKE[0].y + 1] == 2)
        {
            return true;
        }
    }
    if (MOVEMENT_DIR == 2)
    {
        if (MAP[SNAKE[0].x][SNAKE[0].y - 1] == 2)
        {
            return true;
        }
    }
    if (MOVEMENT_DIR == 3)
    {
        if (MAP[SNAKE[0].x - 1][SNAKE[0].y] == 2)
        {
            return true;
        }
    }
    if (MOVEMENT_DIR == 4)
    {
        if (MAP[SNAKE[0].x + 1][SNAKE[0].y] == 2)
        {
            return true;
        }
    }

    return false;
}

void grow_snake(void)
{
    if (MOVEMENT_DIR == 1)
    {
        if (SNAKE[SNAKE_LENGTH - 1].y != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y - 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y - 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x - 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x - 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x + 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x + 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y + 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y + 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
    }
    else if (MOVEMENT_DIR == 2)
    {
        if (SNAKE[SNAKE_LENGTH - 1].y != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y + 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y + 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x - 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x - 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x + 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x + 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y - 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y - 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
    }
    else if (MOVEMENT_DIR == 3)
    {
        if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x + 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x + 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].y != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y + 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y - 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y - 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y + 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x - 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x - 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
    }
    else if (MOVEMENT_DIR == 4)
    {
        if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x - 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x - 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].y != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y + 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y - 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MAX - 1 && MAP[SNAKE[SNAKE_LENGTH - 1].x][SNAKE[SNAKE_LENGTH - 1].y - 1] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y + 1;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
        else if (SNAKE[SNAKE_LENGTH - 1].x != MIN && MAP[SNAKE[SNAKE_LENGTH - 1].x + 1][SNAKE[SNAKE_LENGTH - 1].y] != 1)
        {
            SNAKE[SNAKE_LENGTH].y = SNAKE[SNAKE_LENGTH - 1].y;
            SNAKE[SNAKE_LENGTH].x = SNAKE[SNAKE_LENGTH - 1].x + 1;
            MAP[SNAKE[SNAKE_LENGTH].x][SNAKE[SNAKE_LENGTH].y] = 1;
        }
    }
    SNAKE_LENGTH++;
    SCORE++;
}