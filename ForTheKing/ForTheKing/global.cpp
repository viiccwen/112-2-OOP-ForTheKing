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


void PrintString(int x, int y, std::string s) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(hConsole, position);
	std::cout << s;
}

std::string ReturnSpace(int number) {
	std::string s;
	for (int i = 0; i < number; ++i) s += ' ';
	return s;
}