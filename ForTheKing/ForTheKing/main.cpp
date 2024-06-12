#include <stdlib.h>
#include <time.h>
#include "Game.h"

int main() {
	SetConsoleSize(150, 150);
	srand(time(NULL));
	Game game = Game();
	game.Run();
}