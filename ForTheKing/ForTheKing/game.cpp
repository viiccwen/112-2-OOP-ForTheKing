#include "game.h"
#include "frame.h"
#include "shop.h"
#include "combat.h"
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
	map.map[48][9] = ENEMY;
	map.map[52][14] = ENEMY;


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
	tent_events = {};

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

bool Game::IsRectValid(int x, int y) {
	if (x < 0 || x >= 140 || y < 0 || y >= 50) return false;
	if (map.map[x][y] == WALL) return false;
	return true;
}

bool Game::IsMoveValid(int press) {
	Role& role = roles[move_role_index];
	if (ctl.isUp(press) && IsRectValid(role.position.x, role.position.y - 1)) {
		role.position.y--;
		return true;
	}
	else if (ctl.isDown(press) && IsRectValid(role.position.x, role.position.y + 1)) {
		role.position.y++;
		return true;
	}
	else if (ctl.isLeft(press) && IsRectValid(role.position.x - 1, role.position.y)) {
		role.position.x--;
		return true;
	}
	else if (ctl.isRight(press) && IsRectValid(role.position.x + 1, role.position.y)) {
		role.position.x++;
		return true;
	}
	return false;
}

// issue: WHY HERE???
void PrintBagInfo(std::vector<std::shared_ptr<Equipment>> Bag, int select_index) {
	int y = 1;
	for (auto& item : Bag) {
		std::string bag_str = "";

		if (select_index == y - 1) bag_str += FG_YELLOW;

		bag_str += item->EquipmentTypeToString() + CLOSE;

		PrintString(1, y++, bag_str);
	}
}

bool Game::IsTeleportValid(int x, int y, bool pressing) {
	if (x < 0 || x >= 140 || y < 0 || y >= 50) return false;

	if (pressing) {
		if (map.map[x][y] == WALL) return false;
		if (map.map[x][y] == SHOP) return false;
		if (map.map[x][y] == ENEMY) return false;
		if (map.map[x][y] == EVENT) return false;
	}
	return true;
}

bool Game::IsTeleportMoveValid(int press) {
	Role& role = roles[move_role_index];
	if (ctl.isUp(press) && IsTeleportValid(role.position.x, role.position.y - 1, false)) {
		role.position.y--;
		return true;
	}
	else if (ctl.isDown(press) && IsTeleportValid(role.position.x, role.position.y + 1, false)) {
		role.position.y++;
		return true;
	}
	else if (ctl.isLeft(press) && IsTeleportValid(role.position.x - 1, role.position.y, false)) {
		role.position.x--;
		return true;
	}
	else if (ctl.isRight(press) && IsTeleportValid(role.position.x + 1, role.position.y, false)) {
		role.position.x++;
		return true;
	}
	return false;
}

void Game::HandleTeleportPlayerInput(bool& need_fresh) {
	Role& role = roles[move_role_index];
	int press = ctl.GetInput();

	if (ctl.isEnter(press) && IsTeleportValid(role.position.x, role.position.y, true)) {
		teleport_escape = true;
	}
	else if (IsTeleportMoveValid(press)) {
		need_fresh = true;
	}
}

void Game::Teleport() {
	Role& role = roles[move_role_index];

	bool need_fresh = true;
	teleport_mode = true;

	InitialWalkMode();

	while (true) {
		if (need_fresh) {
			RefreshMap();
			need_fresh = false;
		}

		HandleTeleportPlayerInput(need_fresh);

		if (teleport_escape) break;
	}

	teleport_mode = false;
}

void Game::CheckTentTime() {
	for (int i = 0; i < tent_events.size(); i++) {
		if (Turn == tent_events[i].round && move_role_index == tent_events[i].player_index) {
			map.map[tent_events[i].x][tent_events[i].y] = ROAD;

			tent_events.erase(tent_events.begin() + i);
			i--;
		}
		else {
			map.map[tent_events[i].x][tent_events[i].y] = TENT;
		}
	}
}

void Game::HandleTentEvent() {
	Role& role = roles[move_role_index];
	if (move_point == 0) {
		role.Vitality = (role.Vitality + 50 > role.MaxVitality ? role.MaxVitality : role.Vitality + 50);
		role.Focus = (role.Focus + 5 > role.MaxFocus ? role.MaxFocus : role.Focus + 5);

		PrintRoleInfo(roles);
	}
}

void Game::PlaceTent() {
	Role& role = roles[move_role_index];
	int x = role.position.x;
	int y = role.position.y;

	tent_events.push_back({ Turn + 1, move_role_index , x, y });
}

void Game::UseItem(std::shared_ptr<Else> item) {
	Role& role = roles[move_role_index];

	switch (item->Type) {
	case ElseType::Godsbeard:
		role.Vitality = (role.Vitality + 25 > role.MaxVitality ? role.MaxVitality : role.Vitality + 25);
		break;
	case ElseType::GoldenRoot:
		role.Focus = (role.Focus + 3 > role.MaxFocus ? role.MaxFocus : role.Focus + 3);
		break;
	case ElseType::TeleportScroll:
		Teleport();
		break;
	case ElseType::Tent:
		PlaceTent();
		break;
	default: break;
	}
}

void Game::ApplyEquipment(int& select_index, int player_index) {
	if (Role::Bag.size() == 0) return;

	auto& equipment = Role::Bag[select_index];
	auto& player = roles[player_index];

	if (auto newWeapon = std::dynamic_pointer_cast<Weapon>(equipment)) {

		// Existing weapon goes to the bag
		if (!player.weapon->isNone()) {
			// take it to the bag
			Role::Bag.push_back(player.weapon);

			// remove effect of the equipment now
			applyEquipmentStats(player, player.weapon, false);
		}

		player.weapon = newWeapon;
		applyEquipmentStats(player, newWeapon, true);
	}
	else if (auto newArmor = std::dynamic_pointer_cast<Armor>(equipment)) {

		// Existing armor goes to the bag
		if (!player.armor->isNone()) {
			Role::Bag.push_back(player.armor);
			applyEquipmentStats(player, player.armor, false);
		}

		player.armor = newArmor;  // Equip new armor
		applyEquipmentStats(player, newArmor, true);
	}
	else if (auto newAccessory = std::dynamic_pointer_cast<Accessory>(equipment)) {

		// Existing accessory goes to the bag
		if (!player.accessory->isNone()) {
			Role::Bag.push_back(player.accessory);
			applyEquipmentStats(player, player.accessory, false);
		}

		player.accessory = newAccessory;  // Equip new accessory
		applyEquipmentStats(player, newAccessory, true);
	}
	else if (auto newItem = std::dynamic_pointer_cast<Else>(equipment)) {
		UseItem(newItem);
	}

	// remove equipment
	Role::Bag.erase(Role::Bag.begin() + select_index);

	if (select_index >= Role::Bag.size()) select_index--;

	// update player status
	PrintRoleInfo(roles);
}

void Game::HandleBagInput(int& select_index, int press) {
	if (ctl.isUp(press)) {
		if (select_index == 0) return;
		select_index--;
	}
	else if (ctl.isDown(press)) {
		if (select_index == Role::Bag.size() - 1) return;
		select_index++;
	}
	else if (ctl.isOne(press)) {
		ApplyEquipment(select_index, 0);
	}
	else if (ctl.isTwo(press)) {
		ApplyEquipment(select_index, 1);
	}
	else if (ctl.isThree(press)) {
		ApplyEquipment(select_index, 2);
	}
	else if (ctl.isEnter(press)) {
		ApplyEquipment(select_index, move_role_index);
	}
}

void Game::HandleBagEvent() {
	PrintBagSpace();

	int select_index = 0;
	while (true) {
		// clear item space
		for (int y = 1; y < 20; y++)
			PrintString(1, y, ReturnSpace(30));

		PrintBagInfo(Role::Bag, select_index);

		int press = ctl.GetInput();
		if (ctl.isBackspace(press)) break;

		HandleBagInput(select_index, press);
		if (teleport_escape) {
			teleport_escape = false;
			break;
		}
	}

	InitialWalkMode();
}

void Game::HandlePlayerInput(int& move_point, bool& pass_flag, bool& need_fresh) {
	Role& role = roles[move_role_index];
	int press = ctl.GetInput();

	if (ctl.isP(press)) pass_flag = true;
	else if (ctl.isI(press)) {
		HandleBagEvent();
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

void Game::HandleShopInput(int& select_item_index, int press) {
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

void Game::HandleShopEvent() {
	Role& role = roles[move_role_index];
	int select_item_index = 0;

	PrintShopFrame();
	while (true) {
		shop.ShowShopLists(select_item_index, Role::Money);

		int press = ctl.GetInput();
		if (ctl.isBackspace(press)) break;

		HandleShopInput(select_item_index, press);
	}
}

void Game::HandleCombatEvent() {
	PrintCombatEventSpace();
	int select_index = 0;
	while (true) {
		std::string attack = select_index == 0 ? FG_YELLOW + "Attack" + CLOSE : "Attack";
		std::string back = select_index == 1 ? FG_YELLOW + "Back" + CLOSE : "Back";
		PrintString(1, 1, attack);
		PrintString(1, 2, back);
		int press = ctl.GetInput();
		if (HandleCombatInput(select_index, press))
			break;
	}
}

bool Game::HandleCombatInput(int& select_index, int press) {
	if (ctl.isUp(press)) {
		if (select_index == 0) return false;
		select_index--;
	}
	else if (ctl.isDown(press)) {
		if (select_index == 1) return false;
		select_index++;
	}
	else if (ctl.isEnter(press)) {
		HandleCombat(select_index);
		return true;
	}
	return false;
}

void Game::HandleCombat(int& select_index) {
	if (select_index == 0) {
		Role& roleR = roles[move_role_index];
		Combat combat(roleR, enemies[enemyPositionMap.positionMap[{roleR.position.x, roleR.position.y}] - 1]);
		
		for (int i = 0; i < enemyPositionMap.positionMap.size(); i++) {
			if (enemies[i].Vitality <= 0) {
				map.map[enemies[i].position.x][enemies[i].position.y] = ROAD;
				enemyPositionMap.positionMap.erase({ enemies[i].position.x , enemies[i].position.y });
			}
		}
	}
}

void Game::HandleRandomEvent() {
	PrintString(42, 6, "CONGRATS!! You got 999!");
	Role::Money += 999;
	Sleep(1500);
	PrintString(42, 6, ReturnSpace(30));
	map.map[roles[move_role_index].position.x][roles[move_role_index].position.y] = ROAD;
}

void Game::HandleEvents(Point origin_position, bool& need_refresh) {
	Role& role = roles[move_role_index];
	char currentRect = map.map[role.position.x][role.position.y];
	if (currentRect == SHOP) {

		HandleShopEvent();
		role.position = origin_position;

		InitialWalkMode();
		need_refresh = true;
	}
	else if (enemyPositionMap.positionMap.find({ role.position.x, role.position.y }) != enemyPositionMap.positionMap.end()) {

		HandleCombatEvent();
		
		if (map.map[role.position.x][role.position.y] == ENEMY) {
			role.position = origin_position;
		}

		InitialWalkMode();
		need_refresh = true;
	}
	else if (currentRect == TENT) {
		HandleTentEvent();
	}
	else if (currentRect == EVENT) {
		HandleRandomEvent();
	}
}

void Game::RefreshMap() {
	// every tent only live for one round.
	CheckTentTime();

	map.printMap(roles, enemyPositionMap, move_role_index, teleport_mode);
}

void Game::InitialWalkMode() {
	ClearConsole();
	PrintWalkFrame();
	RefreshMap();
	MarkCurrentPlayerFrame(move_role_index + 1);
	DisplayMovementPoints();
	PrintString(55, 2, roles[move_role_index].name);
	PrintString(48, 1, std::to_string(Turn));
	PrintRoleInfo(roles);
}

void Game::DisplayMovementPoints() {
	std::string curMovePoint = "";
	for (int i = 0; i < origin_move_point; i++) {
		if (i < move_point) curMovePoint += FG_YELLOW;
		else curMovePoint += FG_GREY;

		curMovePoint += "*" + CLOSE;

	}
	curMovePoint += ReturnSpace(30);
	PrintString(56, 4, curMovePoint);
}

void Game::ExecuteMove() {
	Role& role = roles[move_role_index];
	bool pass_flag = false;
	bool need_refresh = false;

	origin_move_point = move_point;

	// first display
	RefreshMap();
	DisplayMovementPoints();

	while (move_point && !pass_flag) {
		if (need_refresh) {
			RefreshMap();
			DisplayMovementPoints();
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

		if (roles[move_role_index].Vitality <= 0) continue;

		RefreshMap();
		// print current player name
		PrintString(55, 2, roles[move_role_index].name);

		// mark current player frame (green)
		MarkCurrentPlayerFrame(move_role_index + 1);

		if (move_turn % 3 == 0) {
			Turn++;
		}

		// print turn
		PrintString(48, 1, std::to_string(Turn));

		// clear name space
		PrintString(53, 3, ReturnSpace(20));

		move_point = CalculateMovementPoints();
		ExecuteMove();

	} while (++move_turn);
}