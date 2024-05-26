#include "game.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

int Game::Turn = 0;

Game::Game() {
	map = Map();
	roles.assign(3, Role());
	for (int i = 0; i < 3; i++) {
		roles[i] = Role(i + 1, "Role" + std::to_string(i + 1));
	}

	for (int i = 0; i < 10; i++) {
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

void Game::initRoleAndMap() {
	Role& role1 = roles[0];

	Role& role2 = roles[1];
	role2.Vitality = role2.MaxVitality = 26;
	role2.Focus = role2.MaxFocus = 3;
	role2.PAttack = role2.MaxPAttack = 24;
	role2.PDefense = role2.MaxPDefense = 3;
	role2.MAttack = role2.MaxMAttack = 0;
	role2.MDefense = role2.MaxMDefense = 3;
	role2.HitRate = role2.MaxHitRate = 80;
	role2.Speed = role2.MaxSpeed = 80;

	role2.actSkills = { ActiveSkillType::Attack };
	map.map[52][13] = SHOP;

	Role& role3 = roles[2];
	role3.Vitality = role3.MaxVitality = 26;
	role3.Focus = role3.MaxFocus = 3;
	role3.PAttack = role3.MaxPAttack = 24;
	role3.PDefense = role3.MaxPDefense = 2;
	role3.MAttack = role3.MaxMAttack = 0;
	role3.MDefense = role3.MaxMDefense = 4;
	role3.HitRate = role3.MaxHitRate = 1;
	role3.Speed = role3.MaxSpeed = 90;

	role1.position = { 135,47 };
	role2.position = { 50,11 };
	role3.position = { 50,12 };
	enemies[0].position = { 48,9 };
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

	// upper, middle, lower frame
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, 0, "-");
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + 1, "-");
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + ROLE_INFO_HEIGHT + 2, "-");

	// left, middle, right frame
	for (int i = 1; i <= MAP_HEIGHT; i++) PrintString(0, i, "|");
	for (int i = 1; i <= MAP_HEIGHT; i++) PrintString(MAP_WIDTH + 1, i, "|");
	for (int i = 1; i <= MAP_HEIGHT; i++) PrintString(GAME_ALL_WIDTH - 1, i, "|");
	
	// player's gap frame
	// vertical
	for (int roleGapIdx = 0; roleGapIdx < 4; roleGapIdx++) {
		for (int i = 21; i <= 35; i++) {
			PrintString(ROLE_INFO_WIDTH * roleGapIdx + roleGapIdx, i, "|");
		}
	}

	// current player status
	PrintString(42, 1, "Turn: ");
	PrintString(42, 2, "Player name: ");
	PrintString(42, 3, "use focus: ");
	PrintString(42, 4, "Action Point: ");

	// player infomation
	PrintRoleInfo(roles);

	// Helper
	std::string wall = "wall: " + BG_GREY + WALL + CLOSE;
	std::string road = "road: " + BG_YELLOW + FG_BLACK + ROAD + CLOSE;
	std::string shop = "shop: " + BG_BLUE + SHOP + CLOSE;
	std::string _event = "event: " + BG_RED + ENEMY + CLOSE;
	std::vector<std::string> helper = {
	"--------------------------------------Helper-------------------------------------",
	"adjust focus: (A), (D)",
	"confirm:      (Enter)",
	"move:         (W), (A), (S), (D)",
	"end turn:     (P)",
	"open bag:     (I)",
	wall,
	road,
	shop,
	_event
	};
	for (int y = 11, idx = 0; y - 11 < helper.size(); y++, idx++) PrintString(42, y, helper[idx]);
}

bool Game::processFocus(int& useFocus) {
	Role& role = roles[moveRoleIndex];
	bool passFlag = false;
	bool refreshNeeded = true;

	while (true) {
		if (refreshNeeded) {
			refreshMap();

			// display focus points
			for (int i = 0; i < role.Focus; i++) {
				std::string curFcous = "";

				if (i < useFocus) curFcous += FG_RED;
				else if (i + 1 > role.Focus) curFcous += FG_GREY;
				else curFcous += FG_YELLOW;

				curFcous += "*" + CLOSE;

				PrintString(53 + i, 3, curFcous);
			}
			refreshNeeded = false;

		}

		int press = ctl.GetInput();

		if (ctl.isLeft(press) && useFocus > 0) {
			useFocus--;
			refreshNeeded = true;
		}
		else if (ctl.isRight(press) && useFocus < role.Focus) {
			useFocus++;
			refreshNeeded = true;
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

// BUG: after press P, move-point lines won't clear 
void Game::run() {
	int moveTurn = 0;

	PrintString(48, 1, std::to_string(Turn));
	do {
		moveRoleIndex = moveTurn % 3;
		
		PrintString(55, 2, roles[moveRoleIndex].name);
		roles[moveRoleIndex].MarkCurrentRole();

		if (moveTurn % 3 == 0) {
			Turn++;
			PrintString(48, 1, std::to_string(Turn));
		}
		
		int useFocus = 0;

		// clear
		PrintString(53, 3, ReturnSpace(20));

		// focus point part
		if (roles[moveRoleIndex].Focus <= 0) {
			PrintString(53, 3, "You have no focus left!");
		}
		else if (processFocus(useFocus)) {
			continue;
		}

		// todo: fumble
		int movePoint = calculateMovementPoints(useFocus);
		if (movePoint == 0) {
			//std::cout << "\n" << roles[moveRoleIndex].name << " Fumble!!!!!\n";
			//std::cout << "Press Any Button To Next Turn.\n";
			//system("pause");
			continue;
		}

		executeMovement(movePoint);
	} while (++moveTurn);
}

void Game::executeMovement(int movePoint) {
	Role& role = roles[moveRoleIndex];
	bool passFlag = false;
	bool refreshNeeded = true;

	int originMovePoint = movePoint;
	while (movePoint > 0 && !passFlag) {
		if (refreshNeeded) {
			refreshMap();

			// display focus points
			for (int i = 0; i < originMovePoint; i++) {
				std::string curMovePoint = "";
				
				if (i < movePoint) curMovePoint += FG_YELLOW;
				else curMovePoint += FG_GREY;

				curMovePoint += "*" + CLOSE;

				PrintString(56 + i, 4, curMovePoint);
				
			}
			
			refreshNeeded = false;
		}
		Point originPosition = role.position;
		processPlayerInput(movePoint, passFlag, refreshNeeded);
		handleEvents(originPosition);

		// let next player's movePoint be clear
		if(movePoint == 0) PrintString(56, 4, ReturnSpace(20));
	}
}

void Game::processPlayerInput(int& movePoint, bool& passFlag, bool& refreshNeeded) {
	Role& role = roles[moveRoleIndex];
	int press = ctl.GetInput();

	if (ctl.isP(press)) {
		passFlag = true;
	}
	else if (ctl.isI(press)) {
		// TODO: Open Inventory
	}
	else if (move(press)) {
		movePoint--;	
		refreshNeeded = true;
	}
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
		return false;
	}
	if (map.map[x][y] == WALL)
	{
		return false;
	}
	return true;
}

void Game::refreshMap() {
	Role& role = roles[moveRoleIndex];
	std::stringstream buffer;
	// system("cls");

	map.printMap(roles, enemyPositionMap, moveRoleIndex);
	
}

void Game::handleEvents(Point& originPosition) {
	Role& role = roles[moveRoleIndex];
	char currentRect = map.map[role.position.x][role.position.y];
	if (currentRect == SHOP) {
		handleShop();
	}
	else if (enemyPositionMap.positionMap.find({ role.position.x, role.position.y }) != enemyPositionMap.positionMap.end()) {
		bool isFlee = handleEnemy();
		if (isFlee) {
			role.position = originPosition;
		}
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

// TODO: add the item to role's bag and reduce the money
void Game::executePurchase(int selectIndex) {
	Role& role = roles[moveRoleIndex];
	
}

bool Game::handleEnemy() {
	Role& roleR = roles[moveRoleIndex];
	int selectIndex = 0;

	Combat combat(roleR, enemies[enemyPositionMap.positionMap[{roleR.position.x, roleR.position.y}]]);
	if (combat.resultLog == "You fled from the battle!") {
		return true;
	}
	return false;
}