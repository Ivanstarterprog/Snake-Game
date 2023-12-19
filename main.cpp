#include <conio.h> 
#include <iostream> 
#include <windows.h>
#include <string>

using namespace std;

enum snakesDirection { STOP, LEFT, RIGHT, UP, DOWN };
enum dificultes {EASY = 1,  MEDIUM, HARD};

const int width = 40;
const int height = 20;

int snakeCoordinateX, snakeCoordinateY;
int fruitCoordinateX, fruitCoordinateY;
int playerScore;
int highScore;
int snakeTailX[1000], snakeTailY[1000];
int snakeTailLength; 
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
    snakeTailLength = 0;
}


void GameRender(string playerName) {
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
                for (int cellIndex = 0; cellIndex < snakeTailLength; ++cellIndex) {
                    if (snakeTailX[cellIndex] == column && snakeTailY[cellIndex] == row) {
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

void UpdateGame() {
    int prevTailX = snakeTailX[0];
    int prevTailY = snakeTailY[0];
    int tempTailX, tempTailY;
    snakeTailX[0] = snakeCoordinateX;
    snakeTailY[0] = snakeCoordinateY;

    for (int cellIndex = 1; cellIndex < snakeTailLength; ++cellIndex) {
        tempTailX = snakeTailX[cellIndex];
        tempTailY = snakeTailY[cellIndex];
        snakeTailX[cellIndex] = prevTailX;
        snakeTailY[cellIndex] = prevTailY;
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

    if (snakeCoordinateX >= width) {
        snakeCoordinateX = 0;
    }
    if (snakeCoordinateX < 0) {
        snakeCoordinateX = width - 1;
    }
    if (snakeCoordinateY >= height) {
        snakeCoordinateY = 0;
    }
    if (snakeCoordinateY < 0) {
        snakeCoordinateY = height - 1;
    }

    for (int cellIndex = 0; cellIndex < snakeTailLength; ++cellIndex) {
        if (snakeTailX[cellIndex] == snakeCoordinateX && snakeTailY[cellIndex] == snakeCoordinateY) {
            isGameContinues = false;
        }
    }

    if (snakeCoordinateX == fruitCoordinateX && snakeCoordinateY == fruitCoordinateY) {
        playerScore += 10;
        do {
            fruitCoordinateX = rand() % width;
            fruitCoordinateY = rand() % height;
        } while (fruitCoordinateX == snakeCoordinateX && fruitCoordinateY == snakeCoordinateY);
        ++snakeTailLength;
    }
    if (playerScore > highScore) {
        highScore = playerScore;
    }
}

int SetDifficulty() {
    int screenUpdateRate, choiceDigit;
    cout << "\nУРОВНИ СЛОЖНОСТИ:\n1: Лёгкая\n2: Средняя\n3: Сложная "
        "\nУчтите: если будет введён символ, не написанный выше, "
        "будет автоматически выбрана средняя сложность "
        "\nВыберите уровень сложности: ";
    cin >> choiceDigit;
    switch (choiceDigit) {
    case EASY:
        screenUpdateRate = 200;
        break;
    case MEDIUM:
        screenUpdateRate = 100;
        break;
    case HARD:
        screenUpdateRate = 50;
        break;
    default:
        screenUpdateRate = 100;
    }
    return screenUpdateRate;
}

void UserInput() {
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

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    srand(time(0));

    string playerName;
    cout << "Введите никнейм: ";
    cin >> playerName;
    int screenUpdateRate;
    screenUpdateRate = SetDifficulty();

    while (true){
        GameInit();
        while (isGameContinues) {
            GameRender(playerName);
            UserInput();
            UpdateGame();
            Sleep(screenUpdateRate);
        }
        char restartAnswerChar;
        cout << "Ты проиграл!" << endl;

        cout << "Продолжить? (Y/y): ";
        restartAnswerChar = _getch();
        if (restartAnswerChar != 'Y' && restartAnswerChar != 'y') {
            break;
        };
        cout << endl;
        char changeDifficultyAnswerChar;        
        cout << "Сменить сложность? (Y/y): ";
        changeDifficultyAnswerChar = _getch();
        cout << endl;
        if (changeDifficultyAnswerChar == 'Y' || changeDifficultyAnswerChar == 'y') {
            screenUpdateRate = SetDifficulty();
        };

    }
    system("cls");
    cout << "Спасибо за игру!" << endl;
    cout << "Рекорд игрока " << playerName << ": " << highScore << " очков";

    return 0;
}
