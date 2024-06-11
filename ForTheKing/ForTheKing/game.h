#ifndef _GAME_H_
#define _GAME_H_

#include "global.h"
#include "map.h"
#include "control.h"
#include "shop.h"

struct TentEvent {
	int round;
	int player_index;
	int x;
	int y;
};

class Game {
private:
	Map map;
	Control ctl;
	Shop shop;
	std::vector<Role> roles;
	std::vector<Enemy> enemies;
	PositionMap enemyPositionMap;

	int move_role_index;
	int origin_move_point;
	int move_point;
	bool teleport_mode;
	bool teleport_escape;
	std::vector<TentEvent> tent_events;
public:
	Game();

	void InitialRoles();
	void InitialEnemies();
	void InitialMapAndPosition();
	void InitialMovementOrder();
	void InitalEnemyPositionMap();

	int CalculateMovementPoints();

	int RollDice(int amount, double chance, int use_focus);

	void ExecuteMove();

	void HandlePlayerInput(int& move_point, bool& pass_flag, bool& need_fresh);
	void HandleEvents(Point origin_position, bool& need_refresh);
	void HandleShopEvnet();
	void HandleTentEvent();
	void HandleBagEvent();
	void HandleShopInput(int& select_item_index, int press);
	void HandleBagInput(int& select_index, int press);
	void HandlePurchase(int& select_item_index);

	bool IsMoveValid(int press);
	bool IsRectValid(int x, int y);

	void RefreshMap();
	void InitialWalkMode();

	void DisplayMovementPoints();

	void ApplyEquipment(int& select_index, int player_index);
	void UseItem(std::shared_ptr<Else> item);

	bool IsTeleportValid(int x, int y, bool pressing);
	void HandleTeleportPlayerInput(bool& need_fresh);
	bool IsTeleportMoveValid(int press);
	void Teleport();
	void CheckTentTime();
	void PlaceTent();

	void Run();

	static int Turn;
};


#endif // !_GAME_H_
