#include "global.h"

void PrintString(int x, int y, std::string s) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(hConsole, position);
	std::cout << s;
}