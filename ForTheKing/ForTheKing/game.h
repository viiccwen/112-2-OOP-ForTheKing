#ifndef _GAME_H_
#define _GAME_H_

#include "entity.h"
#include "map.h"
#include "control.h"

class Game {
private:
	Control ctl;
	Map map;
	Role roles[3];
	int moveRoleIndex;
	std::vector<Enemy> enemies;
	PositionMap enemyPositionMap;
public:
	Game();
	void run();
	void initRoleAndMap();

	bool processFocus(int& useFocus);
	int shootCraps(int amount, double chance, int useFocus);
	int calculateMovementPoints(int useFocus);
	void executeMovement(int movePoint);
	bool processPlayerInput(int& movePoint);
	bool move(int press);
	bool isValidRect(int x, int y);
	void refreshMap();

	void handleEvents();

	void handleShop();
	void showShopList(int index);
	void processShopInput(int& selectIndex, int press);
	void executePurchase(int selectIndex);
	
	void handleEnemy();
	void showCombatPanel(int selectIndex);
	bool processEnemyInput(int& selectIndex, int press);

};

#endif // !_GAME_H_
