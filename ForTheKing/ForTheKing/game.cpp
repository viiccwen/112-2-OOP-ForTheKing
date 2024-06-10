#include "game.h"
#include "frame.h"
#include "shop.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>

int Game::Turn = 0;

void Game::InitialRoles() {
	for (int i = 0; i < roles.size(); i++) {
		roles[i] = Role(i + 1, "Role" + std::to_string(i + 1));
	}
}

void Game::InitialEnemies() {
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i] = Enemy(i + 1, "Enemy" + std::to_string(i + 1));
	}
}

void Game::InitialMapAndPosition() {
	map.map[52][13] = SHOP;
	map.map[48][10] = SHOP;
	map.map[136][47] = WALL;
	map.map[136][48] = WALL;
	map.map[136][49] = WALL;
	map.map[137][47] = WALL;
	map.map[138][47] = WALL;
	map.map[139][47] = WALL;
	map.map[138][48] = EVENT;
	map.map[133][45] = SHOP;


	roles[0].position = { 135,47 };
	roles[1].position = { 50,11 };
	roles[2].position = { 50,12 };
	enemies[0].position = { 48,9 };
	enemies[1].position = { 52,14 };

	PrintWalkFrame();

	PrintRoleInfo(roles);
}

void Game::InitialMovementOrder() {
	std::sort(std::begin(roles), std::end(roles), [](const Role& lhs, const Role& rhs) {
		return lhs.Speed < rhs.Speed;
		});
}

void Game::InitalEnemyPositionMap() {
	for (auto& enemy : enemies) {
		enemyPositionMap.addPosition(enemy.position.x, enemy.position.y, enemy.index);
	}
}

Game::Game() {
	do {
		PrintString(0, 0, "Please Enlarge The Console");
		Sleep(100);
	} while (!checkConsoleSize(GAME_ALL_HEIGHT, GAME_ALL_WIDTH));

	map = Map();
	shop = Shop();

	roles.assign(3, Role());
	enemies.assign(2, Enemy());

	InitialRoles();
	InitialEnemies();
	InitialMovementOrder();
	InitialMapAndPosition();
	InitalEnemyPositionMap();
}

int Game::RollDice(int amount, double chance, int use_focus) {
	int win = 0;
	for (int i = 0; i < amount; i++) {
		if (i < use_focus || (rand() % 100) < chance) {
			win++;
		}
	}
	return win;
}

int Game::CalculateMovementPoints() {
	Role& role = roles[move_role_index];
	int max_movement_point = static_cast<int>(role.Speed / 10);
	return RollDice(max_movement_point, role.Speed, 0);
}

bool Game::isRectValid(int x, int y) {
	if (x < 0 || x >= 140 || y < 0 || y >= 50) return false;
	if (map.map[x][y] == WALL) return false;
	return true;
}

bool Game::IsMoveValid(int press) {
	Role& role = roles[move_role_index];
	if (ctl.isUp(press) && isRectValid(role.position.x, role.position.y - 1)) {
		role.position.y--;
		return true;
	}
	else if (ctl.isDown(press) && isRectValid(role.position.x, role.position.y + 1)) {
		role.position.y++;
		return true;
	}
	else if (ctl.isLeft(press) && isRectValid(role.position.x - 1, role.position.y)) {
		role.position.x--;
		return true;
	}
	else if (ctl.isRight(press) && isRectValid(role.position.x + 1, role.position.y)) {
		role.position.x++;
		return true;
	}
	return false;
}

void Game::HandlePlayerInput(int& move_point, bool& pass_flag, bool& need_fresh) {
	Role& role = roles[move_role_index];
	int press = ctl.GetInput();

	if (ctl.isP(press)) pass_flag = true;
	else if (ctl.isI(press)) {
		// TODO: Open Inventory
	}
	else if (IsMoveValid(press)) {
		move_point--;
		need_fresh = true;
	}
}

void Game::HandlePurchase(int& select_item_index) {
	Role& role = roles[move_role_index];

	if (Role::Money >= shop.item_value[select_item_index]) {
		Role::Money -= shop.item_value[select_item_index];

		// put it into bag
		Role::Bag.push_back(shop.item_lists[select_item_index]);
	}
}

void Game::HandlePlayerShopInput(int& select_item_index, int press) {
	if (ctl.isUp(press)) {
		if (select_item_index == 0) return;
		select_item_index--;
	}
	else if (ctl.isDown(press)) {
		if (select_item_index == 10) return;
		select_item_index++;
	}
	else if (ctl.isEnter(press)) {
		HandlePurchase(select_item_index);
	}
}

void Game::HandleShopEvnet() {
	Role& role = roles[move_role_index];
	int select_item_index = 0;

	PrintShopFrame();
	while (true) {
		shop.ShowShopLists(select_item_index, Role::Money);

		int press = ctl.GetInput();
		if (ctl.isBackspace(press)) break;

		HandlePlayerShopInput(select_item_index, press);
	}
}

void Game::HandleEvents(Point origin_position, bool& need_refresh) {
	Role& role = roles[move_role_index];
	char currentRect = map.map[role.position.x][role.position.y];
	if (currentRect == SHOP) {

		// hotfix: if you don't buy anything and press backspace, then the turn, name will disapear
		HandleShopEvnet();
		role.position = origin_position;

		ClearConsole();
		PrintWalkFrame();
		MarkCurrentPlayerFrame(move_role_index + 1);
		PrintRoleInfo(roles);
		need_refresh = true;
	}
	else if (enemyPositionMap.positionMap.find({ role.position.x, role.position.y }) != enemyPositionMap.positionMap.end()) {
		// todo: combat
		/*
		bool isFlee = handleEnemy();
		if (isFlee) {
			role.position = originPosition;
		}
		*/
	}
}

void Game::RefreshMap() {
	map.printMap(roles, enemyPositionMap, move_role_index);
}

void Game::DisplayMovementPoints(int origin, int current) {
	for (int i = 0; i < origin; i++) {
		std::string curMovePoint = "";

		if (i < current) curMovePoint += FG_YELLOW;
		else curMovePoint += FG_GREY;

		curMovePoint += "*" + CLOSE;

		PrintString(56 + i, 4, curMovePoint);
	}
}

void Game::ExecuteMove(int move_point) {
	Role& role = roles[move_role_index];
	bool pass_flag = false;
	bool need_refresh = false;

	int origin_move_point = move_point;

	// first display
	RefreshMap();
	DisplayMovementPoints(origin_move_point, move_point);

	while (move_point && !pass_flag) {
		if (need_refresh) {
			RefreshMap();
			DisplayMovementPoints(origin_move_point, move_point);
			need_refresh = false;
		}

		Point origin_position = role.position;

		HandlePlayerInput(move_point, pass_flag, need_refresh);

		HandleEvents(origin_position, need_refresh);
	}

	// if there is no action move point
	if (origin_move_point == 0) {
		PrintString(56, 4, "No action move point!");
		Sleep(2000);
	}

	// clear move_point space
	if (move_point == 0 || pass_flag)  PrintString(56, 4, ReturnSpace(20));
}

void Game::Run() {
	int move_turn = 0;

	PrintString(48, 1, std::to_string(Turn));
	do {
		// determine current player index
		move_role_index = move_turn % 3;

		RefreshMap();
		// print current player name
		PrintString(55, 2, roles[move_role_index].name);

		// mark current player frame (green)
		MarkCurrentPlayerFrame(move_role_index + 1);

		if (move_turn % 3 == 0) {
			Turn++;
			PrintString(48, 1, std::to_string(Turn));
		}

		// clear name space
		PrintString(53, 3, ReturnSpace(20));

		int move_point = CalculateMovementPoints();
		ExecuteMove(move_point);

	} while (++move_turn);
}