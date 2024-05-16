#include "Game.h"
#include <algorithm>
#include <iostream>

Game::Game() {
	map = Map();
	for (int i = 0; i < 3; i++)
	{
		roles[i] = Role(i + 1, "Role" + std::to_string(i + 1));
	}
	for (int i = 0; i < 10; i++)
	{
		enemies.push_back(Enemy(i + 1, "Enemy" + std::to_string(i + 1)));
	}
	initRoleAndMap();
	std::sort(std::begin(roles), std::end(roles), [](const Role& lhs, const Role& rhs) {
		return lhs.Speed < rhs.Speed;
		});
	for (auto& enemy : enemies) {
		enemyPositionMap.addPosition(enemy.position.x, enemy.position.y, enemy.index);
	}
}

void Game::run() {
	int moveTurn = 0;
	do {
		moveRoleIndex = moveTurn % 3;
		int useFocus = 0;
		if (roles[moveRoleIndex].Focus <= 0) {
			std::cout << "(You have no focus left)\n";
		}
		else if (processFocus(useFocus)) {
			continue;
		}

		int movePoint = calculateMovementPoints(useFocus);
		if (movePoint == 0) {
			std::cout << "\n" << roles[moveRoleIndex].name << " Fumble!!!!!\n";
			std::cout << "Press Any Button To Next Turn.\n";
			system("pause");
			continue;
		}

		executeMovement(movePoint);
	} while (++moveTurn);
}

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
	enemies[0].position = { 48,9 };
	map.map[52][13] = SHOP;

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
	enemies[1].position = { 52,14 };
	map.map[48][10] = SHOP;


	map.map[136][47] = WALL;
	map.map[136][48] = WALL;
	map.map[136][49] = WALL;
	map.map[137][47] = WALL;
	map.map[138][47] = WALL;
	map.map[139][47] = WALL;

	map.map[138][48] = EVENT;
	map.map[133][45] = SHOP;
}

bool Game::processFocus(int& useFocus) {
	Role& role = roles[moveRoleIndex];
	bool passFlag = false;

	while (true) {
		refreshMap();
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

	return passFlag;
}

int Game::shootCraps(int amont, double chance, int useFocus) {
	int win = 0;
	for (int i = 0; i < amont; i++)
	{
		if (i < useFocus || (rand() % 100) < chance) {
			win++;
		}
	}
	return win;
}

int Game::calculateMovementPoints(int useFocus) {
	Role& role = roles[moveRoleIndex];
	int maxMovementPoint = static_cast<int>(role.Speed / 10);
	return shootCraps(maxMovementPoint, role.Speed, useFocus);
}

void Game::executeMovement(int movePoint) {
	Role& role = roles[moveRoleIndex];
	bool passFlag = false;

	while (movePoint > 0 && !passFlag) {
		refreshMap();
		std::cout << movePoint << " movement points left\n";
		passFlag = processPlayerInput(movePoint);
		handleEvents();
	}
}

bool Game::processPlayerInput(int& movePoint) {
	Role& role = roles[moveRoleIndex];
	int press = ctl.GetInput();
	if (ctl.isP(press)) {
		return true;
	}
	else if (ctl.isI(press)) {
		//TODO: Open Inventory
	}
	else if (move(press)) {
		movePoint--;
		refreshMap();
	}
	return false;
}

bool Game::move(int press) {
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

void Game::refreshMap() {
	Role& role = roles[moveRoleIndex];
	std::stringstream buffer;
	system("cls");
	buffer << role.name << ": " << role.position.x << " " << role.position.y << '\n';
	map.printMap(roles, enemyPositionMap, moveRoleIndex, buffer);
	std::cout << buffer.str();
}

void Game::handleEvents() {
	Role& role = roles[moveRoleIndex];
	char currentRect = map.map[role.position.x][role.position.y];
	if (currentRect == SHOP) {
		handleShop();
	}
	else if (enemyPositionMap.positionMap.find({ role.position.x, role.position.y }) != enemyPositionMap.positionMap.end()) {
		handleEnemy();
	}
}

void Game::handleShop() {
	Role& role = roles[moveRoleIndex];
	int selectIndex = 0;
	while (true) {
		showShopList(selectIndex);
		int press = ctl.GetInput();
		if (ctl.isBackspace(press)) {
			break;
		}
		processShopInput(selectIndex, press);
	}
}

void Game::showShopList(int selectIndex) {
	system("cls");
	int page = selectIndex / 5;
	std::cout << "Money: " << roles[0].Money << "\n";
	std::cout << "Shop List(" << page + 1 << "/2)\n";
	// TA's demo include 4 items, 3 weapon, 1 armor, 2 accessory
	// TODO: use item enum class replace this temp array
	std::string items[] = { "TeleportScroll", "Godsbeard", "GoldenRoot", "Tent", "WoodenSword", "Hammer", "MagicWand", "Shoes", "PlateArmor", "Bracelet" };
	for (int i = 0; i < 5; i++)
	{
		int itemIndex = page * 5 + i;
		if (i == selectIndex % 5) {
			std::cout << FG_BLUE;
		}
		std::cout << itemIndex + 1 << ". " << items[itemIndex] << CLOSE << '\n';

	}
	std::cout << "Press Backspace To Exit.\n";
}

void Game::processShopInput(int& selectIndex, int press) {
	if (ctl.isUp(press)) {
		if (selectIndex == 0) return;
		selectIndex--;
	}
	else if (ctl.isDown(press)) {
		if (selectIndex == 9) return;
		selectIndex++;
	}
	else if (ctl.isEnter(press)) {
		executePurchase(selectIndex);
	}
}

void Game::executePurchase(int selectIndex) {
	Role& role = roles[moveRoleIndex];
	// TODO: add he item to role's bag and reduce the money
}

void Game::handleEnemy() {
	Role& roleR = roles[moveRoleIndex];
	int selectIndex = 0;

	Combat combat(roleR, enemies[enemyPositionMap.positionMap[{roleR.position.x, roleR.position.y}]]);

}