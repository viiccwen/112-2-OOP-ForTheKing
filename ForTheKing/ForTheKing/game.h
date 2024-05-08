#pragma once
#include "entity.h"
#include "map.h"
#include "control.h"
class Game {
private:

public:
	Control ctl;
	Map map;
	Game();
	int shootCraps(int amont, double chance);
	void move(Role& role);
};

