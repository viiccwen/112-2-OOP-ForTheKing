#include "Game.h"
Game::Game() {
	map = Map();
}
int Game::shootCraps(int amont, double chance) {
	int win = 0;
	for (int i = 0; i < amont; i++)
	{
		if ((rand() % 100) < chance)
		{
			win++;
		}
	}
	return win;
}

void Game::move(Role& role) {
	int press = ctl.GetInput();
	if (ctl.isUp(press) && role.position.y != 0)
	{
		role.position.y--;
	}
	else if (ctl.isDown(press) && role.position.y != 49)
	{
		role.position.y++;
	}
	else if (ctl.isLeft(press) && role.position.x != 0)
	{
		role.position.x--;
	}
	else if (ctl.isRight(press) && role.position.x != 139)
	{
		role.position.x++;
	}
}