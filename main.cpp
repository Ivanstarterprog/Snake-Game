#include <conio.h> 
#include <iostream> 
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

const int width = 80;
const int height = 20;

int snakeCoordinateX, snakeCoordinateY;
int fruitCoordinateX, fruitCoordinateY;
int playerScore;
int highScore;
int snakeTailX[1000], snakeTailY[1000];
int snakeTailLen; 
bool isGameContinues;
snakesDirection currentDirection;


void GameInit() {

    isGameContinues = true;
    currentDirection = STOP;
    snakeCoordinateX = width / 2;
    snakeCoordinateY = height / 2;
    do {
        fruitCoordinateX = rand() % width;
        fruitCoordinateY = rand() % height;
    } while (fruitCoordinateX == snakeCoordinateX && fruitCoordinateY == snakeCoordinateY);
    playerScore = 0;
    snakeTailLen = 0;
}


void GameRender(string playerName)
{
    system("cls");

    for (int column = 0; column < width + 2; ++column) {
        cout << "-";
    }
    cout << endl;

    for (int row = 0; row < height; ++row) {
        for (int column = 0; column <= width; ++column) {
            if (column == 0 || column == width) {
                cout << "|";
            }
            if (row == snakeCoordinateY && column == snakeCoordinateX) {
                cout << "\x1B[32mO\033[0m";
            }
            else if (row == fruitCoordinateY && column == fruitCoordinateX) {
                cout << "\x1B[31m@\033[0m";
            }
            else {
                bool isTale;
                isTale = false;
                for (int cell = 0; cell < snakeTailLen; ++cell) {
                    if (snakeTailX[cell] == column && snakeTailY[cell] == row) {
                        cout << "\x1B[32mo\033[0m";
                        isTale = true;
                    }
                }
                if (!isTale) {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }

    for (int column = 0; column < width + 2; ++column)
        cout << "-";
    cout << endl;

    cout << "Очки игрока " << playerName << ": " << playerScore << endl;
    cout << "Рекорд: " << highScore << endl;
}

void UpdateGame()
{
    int prevTailX = snakeTailX[0];
    int prevTailY = snakeTailY[0];
    int tempTailX, tempTailY;
    snakeTailX[0] = snakeCoordinateX;
    snakeTailY[0] = snakeCoordinateY;

    for (int cell = 1; cell < snakeTailLen; ++cell) {
        tempTailX = snakeTailX[cell];
        tempTailY = snakeTailY[cell];
        snakeTailX[cell] = prevTailX;
        snakeTailY[cell] = prevTailY;
        prevTailX = tempTailX;
        prevTailY = tempTailY;
    }

    switch (currentDirection) {
        case LEFT:
            --snakeCoordinateX;
            break;
        case RIGHT:
            ++snakeCoordinateX;
            break;
        case UP:
            --snakeCoordinateY;
            break;
        case DOWN:
            ++snakeCoordinateY;
            break;
    }

    if (snakeCoordinateX >= width || snakeCoordinateX < 0 || snakeCoordinateY >= height || snakeCoordinateY < 0) {
        isGameContinues = false;
    }

    for (int cell = 0; cell < snakeTailLen; ++cell) {
        if (snakeTailX[cell] == snakeCoordinateX && snakeTailY[cell] == snakeCoordinateY) {
            isGameContinues = false;
        }
    }

    if (snakeCoordinateX == fruitCoordinateX && snakeCoordinateY == fruitCoordinateY) {
        playerScore += 10;
        do {
            fruitCoordinateX = rand() % width;
            fruitCoordinateY = rand() % height;
        } while (fruitCoordinateX == snakeCoordinateX && fruitCoordinateY == snakeCoordinateY);
        ++snakeTailLen;
    }
    if (playerScore > highScore) {
        highScore = playerScore;
    }
}

int SetDifficulty()
{
    int speedOfSnake, choice;
    cout << "\nУРОВНИ СЛОЖНОСТИ:\n1: Лёгкая\n2: Средняя\n3: Сложная "
        "\nУчтите: если будет введён символ, не написанный выше, "
        "будет автоматически введена средняя сложность "
        "\nВыберите уровень сложности: ";
    cin >> choice;
    switch (choice) {
    case 1:
        speedOfSnake = 200;
        break;
    case 2:
        speedOfSnake = 100;
        break;
    case 3:
        speedOfSnake = 50;
        break;
    default:
        speedOfSnake = 100;
    }
    return speedOfSnake;
}

void UserInput()
{
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            currentDirection = LEFT;
            break;
        case 'd':
            currentDirection = RIGHT;
            break;
        case 'w':
            currentDirection = UP;
            break;
        case 's':
            currentDirection = DOWN;
            break;
        case 'x':
            isGameContinues = false;
            break;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    srand(time(0));

    string playerName;
    cout << "Введите никнейм: ";
    cin >> playerName;
    int speedOfSnake;
    speedOfSnake = SetDifficulty();

    while (true){
        GameInit();
        while (isGameContinues) {
            GameRender(playerName);
            UserInput();
            UpdateGame();
            Sleep(speedOfSnake);
        }
        char isRestart;
        cout << "Продолжить?: ";
        isRestart = _getch();
        if (isRestart != 'Y' && isRestart != 'y') {
            break;
        };
        cout << endl;
        char isChangeDifficulty;
        cout << "Скорость сейчас: " << speedOfSnake << endl;
        cout << "Сменить сложность?: ";
        isChangeDifficulty = _getch();
        cout << endl;
        if (isChangeDifficulty == 'Y' || isChangeDifficulty == 'y') {
            speedOfSnake = SetDifficulty();
        };

    }
    system("cls");
    cout << "Спасибо за игру!" << endl;
    cout << "Рекорд игрока " << playerName << ": " << highScore << " очков";

    return 0;
}
