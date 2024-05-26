#ifndef _GAME_H_
#define _GAME_H_

#include "entity.h"
#include "map.h"
#include "control.h"
#include "combat.h"

class Game {
private:
	Control ctl;
	Map map;
	std::vector<Role> roles;
	std::vector<Enemy> enemies;
	PositionMap enemyPositionMap;

	int moveRoleIndex;
public:
	static int Turn;

	Game();
	void run();
	void initRoleAndMap();

	bool processFocus(int& useFocus);
	int shootCraps(int amount, double chance, int useFocus);
	int calculateMovementPoints(int useFocus);
	void executeMovement(int movePoint);
	void processPlayerInput(int& movePoint, bool& passFlag, bool& refreshNeeded);
	bool move(int press);
	bool isValidRect(int x, int y);
	void refreshMap();

	void handleEvents(Point& originPosition);

	void handleShop();
	void showShopList(int index);
	void processShopInput(int& selectIndex, int press);
	void executePurchase(int selectIndex);
	
	bool handleEnemy();
	void showCombatPanel(int selectIndex);
	bool processEnemyInput(int& selectIndex, int press);
};

#endif // !_GAME_H_
