#include <stdlib.h>
#include <time.h>
#include "Game.h"
int main() {
	srand(time(NULL));
	Game game = Game();
	game.run();	
}