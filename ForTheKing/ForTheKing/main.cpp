#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "Game.h"
int main() {
	srand(time(NULL));
	Game game = Game();
	Role roles[] = { Role(1, "role1"), Role(2, "role2"), Role(3, "role3") };
	std::sort(std::begin(roles), std::end(roles), [](const Role& lhs, const Role& rhs) {
		return lhs.Speed < rhs.Speed;
		});
	int turn = 0;

	do {
		int moveRoleIndex = turn % 3;
		int maxMovementPoint = static_cast<int>(roles[moveRoleIndex].Speed / 10);
		int movePoint = game.shootCraps(maxMovementPoint, roles[moveRoleIndex].Speed);
		for (; movePoint > 0; movePoint--)
		{
			std::stringstream buffer;
			system("cls");
			buffer << roles[moveRoleIndex].name << ": " << roles[moveRoleIndex].position.x << " " << roles[moveRoleIndex].position.y << '\n';
			game.map.printMap(roles, moveRoleIndex, buffer);
			buffer << movePoint << " movement points left\n";
			std::cout << buffer.str();
			game.move(roles[moveRoleIndex]);
		}

	} while (++turn);
}