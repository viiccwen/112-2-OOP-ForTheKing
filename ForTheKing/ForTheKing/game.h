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
	std::vector<Enemy> enemies;
	PositionMap enemyPositionMap;
public:
	Game();
	void run();
	void initRoleAndMap();

	bool processFocus(int moveRoleIndex, int& useFocus);
	int shootCraps(int amount, double chance, int useFocus);
	int calculateMovementPoints(int moveRoleIndex, int useFocus);
	void executeMovement(int moveRoleIndex, int movePoint);
	bool processPlayerInput(int moveRoleIndex, int& movePoint);
	bool move(int press, int moveRoleIndex);
	bool isValidRect(int x, int y);
	void refreshMap(int moveRoleIndex);

	void handleEvents(int moveRoleIndex);

	void handleShop(int moveRoleIndex);
	void showShopList(int index);
	void processShopInput(int moveRoleIndex, int& selectIndex, int press);
	void executePurchase(int moveRoleIndex, int selectIndex);
	
	void handleEnemy(int moveRoleIndex);
	void showCombatPanel(int moveRoleIndex, int selectIndex);
	bool processEnemyInput(int moveRoleIndex,int& selectIndex, int press);

};

#endif // !_GAME_H_
