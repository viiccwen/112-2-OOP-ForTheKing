#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "entity.h"
#include "map.h"
#include "control.h"
int shootCraps(int amont, double chance) {
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

void move(Control& ctl, Role &role) {
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

int main() {
	srand(time(NULL));
	Control ctl;
	Map map = Map();
	Role roles[] = { Role(1, "role1"), Role(2, "role2"), Role(3, "role3") };
	std::sort(std::begin(roles), std::end(roles), [](const Role& lhs, const Role& rhs) {
		return lhs.Speed < rhs.Speed;
		});
	int turn = 0;

	do {
		int moveRoleIndex = turn % 3;
		int maxMovementPoint = static_cast<int>(roles[moveRoleIndex].Speed / 10);
		int movePoint = shootCraps(maxMovementPoint, roles[moveRoleIndex].Speed);
		for (; movePoint > 0; movePoint--)
		{
			std::stringstream buffer;
			system("cls");
			buffer << roles[moveRoleIndex].name << ": " << roles[moveRoleIndex].position.x << " " << roles[moveRoleIndex].position.y << '\n';
			map.printMap(roles[moveRoleIndex], buffer);
			buffer << movePoint << " movement points left\n";
			std::cout << buffer.str();
			move(ctl, roles[moveRoleIndex]);
		}

	} while (++turn);
}