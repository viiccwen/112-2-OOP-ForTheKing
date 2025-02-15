#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "entity.h"
#include "map.h"
#include "control.h"
#include "skill.h"
#include "frame.h"
#include "global.h"

class Role;
class Entity;

class CombatRole {
public:
	Role& role;
	double priority;
	int moveCount;

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
	bool isTurnChange;
	bool NeedRefreshFrame;

	Combat(Role& role, Enemy& enemy);

	void combatLoop();
	std::string isCombatEnd();
	void priorityJudge();
	void roleNewTurn(Entity& entity);
	void showCombatPanel(int selectIndex);
	bool processInput(int& selectIndex, int press);
	void showStatus();
	bool battlePhase(ActiveSkills& skill);
	int chooseFocus(int maxFocus);
	Entity& chooseTarget(ActiveSkills skill);
	void effectAutoBuff();
	void effectTurnStartBuff(Entity& entity, bool& isdizzy);
	void turnEnd();

	bool HandleUseItemEvent();
	bool HandleUseItemInput(int& select_index, int press, std::vector<std::pair<std::shared_ptr<Else>, int>> items);
	void HandleUseItem(std::shared_ptr<Else> item);

};

#endif // !_COMBAT_H_