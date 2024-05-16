#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "entity.h"
#include "map.h"
#include "control.h"
#include "skill.h"

class Role;
class Entity;

class CombatRole{
public:
	Role& role;
	double priority;
	int moveCount;
	int useFocus;

	CombatRole(Role& _role);
};

class CombatEnemy {
	public:
	Enemy& enemy;
	double priority;
	int moveCount;
	int useFocus;

	CombatEnemy(Enemy& _enemy);
};

class Combat {
public:
	Control ctl;
	CombatRole combatRole;
	CombatEnemy combatEnemy;
	Entity* attacker;
	Entity* defender;

	std::string resultLog;
	bool isRoleTurn;
	bool isEnd;

	Combat(Role& role, Enemy& enemy);

	void combatLoop();
	std::string isCombatEnd();
	void priorityJudge();
	void showCombatPanel(int selectIndex);
	void processInput(int& selectIndex, int press);
	void showStatus();
};

#endif // !_COMBAT_H_