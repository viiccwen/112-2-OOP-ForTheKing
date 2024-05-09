#ifndef _GAME_H_
#define _GAME_H_

#include "entity.h"
#include "map.h"
#include "control.h"
class Game {
private:

public:
	Control ctl;
	Map map;
	Role roles[3];
	Game();
	int shootCraps(int amont, double chance);
	bool isValidRect(int x, int y);
	bool move(int moveRoleIndex);
	void refreshMap(int moveRoleIndex, std::stringstream& buffer);
	void run();
};

#endif // !_GAME_H_
