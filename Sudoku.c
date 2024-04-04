/*
Made by:
    Nome: Thiago Lopes de Araújo RA: 22.218.011-9
    All Rights Reserved
    Date: 26 / 05 / 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio/conio.c"
#define True 1
#define False 0

struct Player
{
    char name[100];
    int number;
};

// Predefinições das funções
static void freeMatrix(int **matrix);
static void recordGame(int **matrix, int vol, char name[], int difficult, int complete);
static void completeMatrix(int **matrix, int difficult);
static int verifyVictory(int **matrixLoaded, int difficult);
static void loadMatrix(int difficult, int **matrix);
static int qtdPlacesWithNumber(int **matrix);
static void showMatrix(int **matrix);
static void print(char phrase[], int time);
static void menu(char options[][100], void (*pointerFunction1)(), void (*pointerFunction2)(), void (*pointerFunction3)());
static int verifyPosition(char position, int **m, struct Player p, int difficult);
static int verifyValue(int value);
static void passMatrixToAnother(int **m1, int **m2);
static void game(int difficult, struct Player p, int newGame, int **matrixFromLoadGame);
static void newGame();
static void loadGame();
static void history();

/* Main */
int main()
{
    printf("Welcome to the game Sudoku");
    char options[][100] = {{"What do you want to do?"}, {"New Game"}, {"Load Game"}, {"See your games"}};
    menu(options, newGame, loadGame, history);
    return 0;
}

/* Funcões utilitárias */
void freeMatrix(int **matrix)
{
    int i;

    for (i = 0; i < 9; i++)
        free(matrix[i]);

    free(matrix);
}

int **matrixGenerator()
{
    int **m;
    int i, j;
    m = malloc(sizeof(int *) * 9);

    for (i = 0; i < 9; i++)
    {
        m[i] = malloc(sizeof(int) * 9);
    }

    return m;
}

void recordGame(int **matrix, int vol, char name[], int difficult, int complete)
{
    int i, j;
    char path[50];
    FILE *archive;
    sprintf(path, "history\\%s.txt", name);
    archive = fopen(path, "w");
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (complete)
                fprintf(archive, "%d ", matrix[i][j]);
            else
                (matrix[i][j] <= 9) ? fprintf(archive, "%d ", matrix[i][j]) : fprintf(archive, "%d ", 0);
        }
        fprintf(archive, "\n");
    }
    fprintf(archive, "%d\n", difficult);
    fprintf(archive, "%d", vol);
    fclose(archive);
    main();
}

void completeMatrix(int **matrix, int difficult)
{
    FILE *archive;
    int i, j;
    char path[50];

    sprintf(path, "gamesComplete/sudoku%d.txt", difficult);
    archive = fopen(path, "r");

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fscanf(archive, "%d", &matrix[i][j]);
        }
    }

    fclose(archive);
}

int verifyVictory(int **matrixLoaded, int difficult)
{
    int i, j;
    int **matrix;

    matrix = matrixGenerator();
    completeMatrix(matrix, difficult);

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (matrixLoaded[i][j] != matrix[i][j])
            {
                freeMatrix(matrix);
                return False;
            }
        }
    }

    freeMatrix(matrix);
    return True;
}

void loadMatrix(int difficult, int **matrix)
{
    int i, j;
    FILE *archive;
    char path[50];
    sprintf(path, "gameLv\\matrix%d.txt", difficult);

    if ((archive = fopen(path, "r")) == NULL)
        printf("\nError, archive not found.\n");
    else
        printf("\nArchive found with success.\n");

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fscanf(archive, "%d", &matrix[i][j]);
        }
    }

    fclose(archive);
    return;
}

int qtdPlacesWithNumber(int **matrix)
{
    int n = 0, i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (matrix[i][j] == 0)
            {
                n++;
            }
        }
    }
    return n;
}

void showMatrix(int **matrix)
{
    int i, j;
    puts("");
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (matrix[i][j] <= 9)
            {
                textcolor(12);
                printf("%3d", matrix[i][j]);
            }
            else
            {
                textcolor(2);
                printf("%3c", matrix[i][j]);
            }
        }
        puts("");
    }
    puts("");
    textcolor(7);
}

void print(char phrase[], int time)
{
    system("cls");
    printf("\n%s", phrase);
    sleep(time);
    system("cls");
}

void menu(char options[][100], void (*pointerFunction1)(), void (*pointerFunction2)(), void (*pointerFunction3)())
{
    int loop = True, choice;
    while (loop == True)
    {
        printf("\n%s \n 1 - %s\n 2 - %s\n 3 - %s\n 4 - Exit\n", options[0], options[1], options[2], options[3]);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("cls");
            pointerFunction1();
            break;
        case 2:
            system("cls");
            pointerFunction2();
            break;
        case 3:
            system("cls");
            pointerFunction3();
            break;
        case 4:
            loop = False;
            break;
        default:
        {
            print("Please, enter a valid option", 1);
        }
        }
    }
    return;
}

int verifyPosition(char position, int **m, struct Player p, int difficult)
{
    if (position == 48)
        recordGame(m, 0, p.name, difficult, 0);

    int i, j, n;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (position == m[i][j])
            {
                return True;
            }
        }
    }
    return False;
}

int verifyValue(int value)
{
    return (value >= 1 && value <= 9);
}

void passMatrixToAnother(int **m1, int **m2)
{
    int i, j;
    for (int i = 0; i < 9; ++i)
    {
        for (j = 0; j < 9; j++)
        {
            m1[i][j] = m2[i][j];
        }
    }
}

/* Funções principais */
void game(int difficult, struct Player p, int newGame, int **matrixFromLoadGame)
{
    int value, i, j, victory, positionsLeft, count = 0, x;
    char position;
    int **matrixLoaded;

    matrixLoaded = matrixGenerator();
    if (newGame)
        loadMatrix(difficult, matrixLoaded);
    else
    {
        passMatrixToAnother(matrixLoaded, matrixFromLoadGame);
        free(matrixFromLoadGame);
    }

    positionsLeft = qtdPlacesWithNumber(matrixLoaded);

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (matrixLoaded[i][j] == 0)
            {
                x = 'a' + count;
                matrixLoaded[i][j] = x;
                count++;
            }
        }
    }

    while (positionsLeft != 0)
    {
        showMatrix(matrixLoaded);
        printf("\nPositions Left:%d\n", positionsLeft);
        printf("\n Select a place to play or enter 0 to exit and save your game: ");
        scanf(" %c", &position);

        if (!verifyPosition(position, matrixLoaded, p, difficult))
        {
            printf("\nInvalid Option\n");
            continue;
        }

        printf("\n Now, a value to replace it ");
        scanf(" %d", &value);

        if (!verifyValue(value))
        {
            printf("\nare you reading or not? INVALID OPTION");
            continue;
        }

        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                if (position == matrixLoaded[i][j])
                {
                    matrixLoaded[i][j] = value;
                    positionsLeft--;
                }
            }
        }
    }

    print("Game ended!", 2);
    victory = verifyVictory(matrixLoaded, difficult);
    printf("%s\n", (victory ? "\nYou Win, congratulations, you're a Genius!" : "\nYou Lost!, You have to decrease the difficult, NEWBIE!"));
    printf("\nTo see your games, what you have to do is just insert your name in the history:\n");
    recordGame(matrixLoaded, victory, p.name, difficult, 1);
    freeMatrix(matrixLoaded);
}

void newGame()
{
    int choice;
    struct Player p;

    printf("\n What's your name?:\n");
    scanf("%s", &p.name);
    printf("\nOk %s,\nSelect a Difficult:\n 1 - Newbie\n 2 - Little Nerd\n 3 - Genius!\n", p.name);
    scanf("%d", &choice);
    (choice >= 1 && choice <= 4) ? printf("At least you're not a dumb!\n") : printf("You're a Dumb!!\n The game will start at newbie's difficult\n");
    (choice >= 1 && choice <= 4) ? (choice = choice) : (choice = 1);
    game(choice, p, True, NULL);
    return;
}

void loadGame()
{
    struct Player p;
    int **m;
    int i, j, difficult;
    char path[50];
    FILE *archive;

    m = matrixGenerator();

    printf("\n Enter the name of the player:");
    scanf("%s", &p.name);

    sprintf(path, "history\\%s.txt", p.name);
    archive = fopen(path, "r");
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fscanf(archive, "%d", &m[i][j]);
        }
    }

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            printf("%d", m[i][j]);
        }
    }
    fscanf(archive, "%d", &difficult);
    fclose(archive);
    game(difficult, p, False, m);
}

void history()
{
    char name[50], path[50];
    char difficultOfTheGame[][20] = {{"Newbie game bleh"}, {"Nerd Game..."}, {"GENIUS GAMER"}};
    int i, j, vol, difficult;
    int **matrix;
    FILE *archive;

    matrix = matrixGenerator();
    printf("\nEnter the player of the game:");
    scanf("%s", name);

    sprintf(path, "history\\%s.txt", name);
    archive = fopen(path, "r");
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fscanf(archive, "%d", &matrix[i][j]);
        }
    }
    fscanf(archive, "%d", &difficult);
    fscanf(archive, "%d", &vol);
    fclose(archive);

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            printf("%3d", matrix[i][j]);
        }
        puts("");
    }
    printf("%s\n", (vol ? "victory" : "loser!!!"));
    printf("%s", difficultOfTheGame[difficult - 1]);
    puts("");
    freeMatrix(matrix);
}