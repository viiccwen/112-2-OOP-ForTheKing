#ifndef _GAME_H_
#define _GAME_H_

#include "global.h"
#include "map.h"
#include "control.h"
#include "shop.h"

class Game {
private:
	Map map;
	Control ctl;
	Shop shop;
	std::vector<Role> roles;
	std::vector<Enemy> enemies;
	PositionMap enemyPositionMap;

	int move_role_index;
public:
	Game();

	void InitialRoles();
	void InitialEnemies();
	void InitialMapAndPosition();
	void InitialMovementOrder();
	void InitalEnemyPositionMap();

	int CalculateMovementPoints();

	int RollDice(int amount, double chance, int use_focus);

	void ExecuteMove(int move_point);

	void HandlePlayerInput(int& move_point, bool& pass_flag, bool& need_fresh);
	void HandleEvents(Point origin_position, bool& need_refresh);
	void HandleShopEvnet();
	void HandlePlayerShopInput(int& select_item_index, int press);
	void HandlePurchase(int& select_item_index);

	bool IsMoveValid(int press);
	bool isRectValid(int x, int y);

	void RefreshMap();

	void DisplayMovementPoints(int origin, int current);

	void Run();

	static int Turn;
};


#endif // !_GAME_H_
