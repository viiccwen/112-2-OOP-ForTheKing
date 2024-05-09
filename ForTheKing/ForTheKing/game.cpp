#include "Game.h"
#include <algorithm>
#include <iostream>

Game::Game() {
	map = Map();
	for (int i = 0; i < 3; i++)
	{
		roles[i] = Role(i + 1, "Role" + std::to_string(i + 1));
	}
	std::sort(std::begin(roles), std::end(roles), [](const Role& lhs, const Role& rhs) {
		return lhs.Speed < rhs.Speed;
		});

	// test shop and enemy position
	for (int i = 0; i < 3; i++)
	{
		if (isValidRect(roles[i].position.x + 2, roles[i].position.y + 2)) {
			map.map[roles[i].position.x + 2][roles[i].position.y + 2] = SHOP;
		}
		if (isValidRect(roles[i].position.x - 2, roles[i].position.y - 2)) {
			map.map[roles[i].position.x - 2][roles[i].position.y - 2] = ENEMY;
		}
	}
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

bool Game::isValidRect(int x, int y) {
	if (x < 0 || x >= 140 || y < 0 || y >= 50)
	{
		return false;
	}
	if (map.map[x][y] == WALL)
	{
		return false;
	}

	return true;
}

bool Game::move(int moveRoleIndex) {
	Role& role = roles[moveRoleIndex];
	int press = ctl.GetInput();
	if (ctl.isUp(press) && isValidRect(role.position.x, role.position.y - 1))
	{
		role.position.y--;
		return true;
	}
	else if (ctl.isDown(press) && isValidRect(role.position.x, role.position.y + 1))
	{
		role.position.y++;
		return true;
	}
	else if (ctl.isLeft(press) && isValidRect(role.position.x - 1, role.position.y))
	{
		role.position.x--;
		return true;
	}
	else if (ctl.isRight(press) && isValidRect(role.position.x + 1, role.position.y))
	{
		role.position.x++;
		return true;
	}
	return false;
}

void Game::refreshMap(int moveRoleIndex, std::stringstream& buffer) {
	Role& role = roles[moveRoleIndex];
	system("cls");
	buffer << role.name << ": " << role.position.x << " " << role.position.y << '\n';
	map.printMap(roles, moveRoleIndex, buffer);
	std::cout << buffer.str();
	buffer.str("");
}

void Game::run() {
	int moveTurn = 0;

	do {
		std::stringstream buffer;
		int moveRoleIndex = moveTurn % 3;
		Role& role = roles[moveRoleIndex];
		refreshMap(moveRoleIndex, buffer);

		int maxMovementPoint = static_cast<int>(role.Speed / 10);
		int movePoint = shootCraps(maxMovementPoint, role.Speed);
		if (movePoint == 0) {
			std::cout << role.name << " Fumble!!!!!\n";
			std::cout << "Press Any Button To Next Turn.\n";
			system("pause");
			continue;
		}

		while (movePoint > 0)
		{
			std::cout << movePoint << " movement points left\n";
			if (move(moveRoleIndex)) {
				movePoint--;
				system("cls");
				refreshMap(moveRoleIndex, buffer);
			}
			else {
				buffer << "Invalid movement\n";
				continue;
			}

			char currentRect = map.map[role.position.x][role.position.y];
			if (currentRect == SHOP) {
				std::cout << "You have entered a Shop rect\n";
			}
			else if (currentRect == ENEMY) {
				std::cout << "You have entered a Enemy rect\n";
			}

			if (movePoint == 0) {
				std::cout << "Press Any Button To Next Turn.\n";
				system("pause");
			}
		}

	} while (++moveTurn);
}