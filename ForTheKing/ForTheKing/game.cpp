#include "Game.h"
#include <algorithm>
#include <iostream>
void Game::initRoleAndMap() {
	Role& role1 = roles[0];
	role1.position = { 135,47 };

	Role& role2 = roles[1];
	role2.Vitality = role2.MaxVitality = 26;
	role2.Focus = role2.MaxFocus = 6;
	role2.PAttack = role2.MaxPAttack = 24;
	role2.PDefense = role2.MaxPDefense = 3;
	role2.MAttack = role2.MaxMAttack = 0;
	role2.MDefense = role2.MaxMDefense = 3;
	role2.HitRate = role2.MaxHitRate = 80;
	role2.Speed = role2.MaxSpeed = 80;
	role2.position = { 50,11 };

	Role& role3 = roles[2];
	role3.Vitality = role3.MaxVitality = 26;
	role3.Focus = role3.MaxFocus = 6;
	role3.PAttack = role3.MaxPAttack = 24;
	role3.PDefense = role3.MaxPDefense = 2;
	role3.MAttack = role3.MaxMAttack = 0;
	role3.MDefense = role3.MaxMDefense = 4;
	role3.HitRate = role3.MaxHitRate = 1;
	role3.Speed = role3.MaxSpeed = 90;
	role3.position = { 50,12 };

	map.map[136][47] = WALL;
	map.map[136][48] = WALL;
	map.map[136][49] = WALL;
	map.map[137][47] = WALL;
	map.map[138][47] = WALL;
	map.map[139][47] = WALL;

	map.map[138][48] = EVENT;
	map.map[133][45] = SHOP;
}
Game::Game() {
	map = Map();
	for (int i = 0; i < 3; i++)
	{
		roles[i] = Role(i + 1, "Role" + std::to_string(i + 1));
	}
	initRoleAndMap();
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

int Game::shootCraps(int amont, double chance, int useFocus = 0) {
	int win = 0;
	for (int i = 0; i < amont; i++)
	{
		if (i < useFocus || (rand() % 100) < chance) {
			win++;
		}
	}
	return win;
}

bool Game::isValidRect(int x, int y) {
	if (x < 0 || x >= 140 || y < 0 || y >= 50)
	{
		std::cout << "Invalid movement\n";
		return false;
	}
	if (map.map[x][y] == WALL)
	{
		std::cout << "Invalid movement\n";
		return false;
	}
	return true;
}

bool Game::move(int press, int moveRoleIndex) {
	Role& role = roles[moveRoleIndex];
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

void Game::showShopList(int index) {
	system("cls");
	int page = index / 5;
	std::cout << "Shop List(" << page << "/2)\n";
	// TA's demo include 4 items, 3 weapon, 1 armor, 2 accessory
	// TODO: use item enum class replace this temp array
	std::string items[] = { "TeleportScroll", "Godsbeard", "GoldenRoot", "Tent", "WoodenSword", "Hammer", "MagicWand", "Shoes", "PlateArmor", "Bracelet" };
	for (int i = 0; i < 5; i++)
	{
		int itemIndex = page * 5 + i;
		if (i == index % 5) {
			std::cout << FG_BLUE;
		}
		std::cout << itemIndex << ". " << items[itemIndex] << CLOSE << '\n';

	}
	std::cout << "Press Backspace To Exit.\n";
}
void Game::run() {
	// TODO: move some code to new function
	int moveTurn = 0;

	do {
		std::stringstream buffer;
		bool passFlag = false;
		int moveRoleIndex = moveTurn % 3;
		Role& role = roles[moveRoleIndex];
		int useFocus = 0;
		if (role.Focus > 0) {
			while (true) {
				refreshMap(moveRoleIndex, buffer);
				std::cout << "(You have " << role.Focus << " focus left)\n";
				std::cout << "Use (A) and (D) to use focus, (Enter) to confirm:\n";
				for (int i = 0; i < role.MaxFocus; i++)
				{
					if (i < useFocus) {
						std::cout << FG_RED;
					}
					else if (i + 1 > role.Focus) {
						std::cout << FG_GREY;
					}
					else {
						std::cout << FG_YELLOW;
					}
					std::cout << '*' << CLOSE;
				}
				int press = ctl.GetInput();
				if (ctl.isLeft(press) && useFocus > 0) {
					useFocus--;
				}
				else if (ctl.isRight(press) && useFocus < role.Focus) {
					useFocus++;
				}
				else if (ctl.isEnter(press)) {
					role.Focus -= useFocus;
					break;
				}
				else if (ctl.isP(press)) {
					passFlag = true;
					break;
				}
			}
			if (passFlag) {
				continue;
			}
		}
		else {
			std::cout << "(You have no focus left)\n";
		}

		int maxMovementPoint = static_cast<int>(role.Speed / 10);
		int movePoint = shootCraps(maxMovementPoint, role.Speed, useFocus);
		if (movePoint == 0) {
			std::cout << role.name << " Fumble!!!!!\n";
			std::cout << "Press Any Button To Next Turn.\n";
			system("pause");
			continue;
		}
		while (movePoint > 0)
		{
			system("cls");
			refreshMap(moveRoleIndex, buffer);
			std::cout << movePoint << " movement points left\n";
			while (true) {
				int press = ctl.GetInput();
				if (ctl.isP(press)) {
					passFlag = true;
					break;
				}
				else if (ctl.isI(press)) {
					//TODO: Open Inventory
					break;
				}
				else if (move(press, moveRoleIndex)) {
					movePoint--;
					refreshMap(moveRoleIndex, buffer);
					break;
				}
			}
			if (passFlag) {
				break;
			}
			char currentRect = map.map[role.position.x][role.position.y];
			if (currentRect == SHOP) {
				int index = 0;
				while (true)
				{
					showShopList(index);
					int press = ctl.GetInput();
					if (ctl.isBackspace(press)) {
						break;
					}
					if (ctl.isUp(press)) {
						if (index == 0) continue;
						index--;
					}
					else if (ctl.isDown(press)) {
						if (index == 9) continue;
						index++;
					}
				}
			}
			else if (currentRect == ENEMY) {
				std::cout << "You have entered a Enemy rect\n";
			}

			//if (movePoint == 0) {
			//	std::cout << "Press Any Button To Next Turn.\n";
			//	system("pause");
			//}
		}

	} while (++moveTurn);
}