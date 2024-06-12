#include "global.h"

void SetConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    SMALL_RECT rect;

    // set buffer size
    coord.X = width;
    coord.Y = height;
    SetConsoleScreenBufferSize(hConsole, coord);

    // set console size
    rect.Top = 0;
    rect.Left = 0;
    rect.Right = width - 2;
    rect.Bottom = height - 2;
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

std::string ReturnSpace(int number) {
    std::string s;
    for (int i = 0; i < number; ++i) s += ' ';
    return s;
}

bool checkConsoleSize(int requiredRows, int requiredCols) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        int consoleRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int consoleCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        PrintString(0, 1, ReturnSpace(100));
        PrintString(0, 2, ReturnSpace(100));
        PrintString(0, 1, "consoleRows: " + std::to_string(consoleRows) + ", requiredRows: " + std::to_string(requiredRows));
        PrintString(0, 2, "consoleCols: " + std::to_string(consoleCols) + ", requiredCols: " + std::to_string(requiredCols));
        if (consoleCols == 149 && consoleRows == 149) return false;
        return (consoleRows >= requiredRows) && (consoleCols >= requiredCols);
    }
    return false;
}

void PrintString(int x, int y, std::string s) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(hConsole, position);
    std::cout << s;
}

void ClearConsole() {
    for (int x = 0; x < GAME_ALL_WIDTH; x++) {
        for (int y = 0; y < GAME_ALL_HEIGHT; y++) {
            PrintString(x, y, " ");
        }
    }
}