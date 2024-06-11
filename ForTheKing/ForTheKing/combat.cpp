#include "combat.h"

#include <iostream>

CombatRole::CombatRole(Role& _role) : role(_role), priority(0), moveCount(0) {
}

CombatEnemy::CombatEnemy(Enemy& _enemy) : enemy(_enemy), priority(0), moveCount(0), useFocus(0) {
}

Combat::Combat(Role& _role, Enemy& _enemy) : combatRole(_role), combatEnemy(_enemy), isTurnChange(1) {
	resultLog = "";
	Combat::combatLoop();
}

void Combat::combatLoop() {
	int selectIndex = 0;
	bool isDizzy = false;

	PrintCombatFrame();
	while (true) {
		resultLog = Combat::isCombatEnd();
		if (isEnd) {
			Combat::showStatus();
			PrintString(25, 12, resultLog);
			system("pause");
			break;
		}

		Combat::effectAutoBuff();
		if (isTurnChange)
			Combat::priorityJudge();
		Combat::showStatus();

		if (isRoleTurn) {
			if (isTurnChange) {//handle cooldown and buff duration
				Combat::roleNewTurn(combatRole.role);
				effectTurnStartBuff(combatRole.role, isDizzy);
			}
			if (isDizzy) {//handle dizzy
				resultLog = "Role is dizzy and can't move!";
				isDizzy = false;
				combatRole.moveCount++;
				isTurnChange = true;//alter in processInput()
			}
			else {//role act
				Combat::showStatus();
				Combat::showCombatPanel(selectIndex);
				int press = ctl.GetInput();
				processInput(selectIndex, press);
			}
		}
		else {//enemy turn
			//main phase
			Combat::roleNewTurn(combatEnemy.enemy);
			effectTurnStartBuff(combatEnemy.enemy, isDizzy);
			Combat::showStatus();
			if (isDizzy) {//handle dizzy
				resultLog = "Enemy is dizzy and can't move!";
				isDizzy = false;
				combatEnemy.moveCount++;
				isTurnChange = true;//alter in processInput()
			}
			else {
				bool canAct = false;
				for (auto a : combatEnemy.enemy.actSkills)
					if (a.curCooldown == 0) {
						canAct = true;
						break;
					}
				if (canAct) {
					int enemyRandomActSkill = rand() % combatEnemy.enemy.actSkills.size();
					ActiveSkills& skill = combatEnemy.enemy.actSkills[enemyRandomActSkill];
					if (skill.needTarget == 2)
						defender = &combatEnemy.enemy;
					//battle phase, damage phase in damagePhase.h
					if (skill.execute(skill, combatEnemy.enemy, *defender, 0, resultLog))//0: enemy don't need focus
					{
						skill.curCooldown = skill.cooldown;
					}
				}
				else {
					resultLog = "Enemy can't act!";
				}
				combatEnemy.moveCount++;
			}
		}
		if (!resultLog.empty()) {
			PrintString(25, 9, resultLog);
			PrintString(25, 23, "");
			system("pause");

			PrintString(25, 8, ReturnSpace(50));
			PrintString(25, 9, ReturnSpace(50));
			PrintString(25, 23, ReturnSpace(50));
		}

		if (NeedRefreshFrame) {
			system("pause");
			PrintCombatFrame();
			NeedRefreshFrame = false;
		}

		Combat::turnEnd();//avoid overdose effect
	}
}

std::string Combat::isCombatEnd() {
	if (resultLog == "Flee success!") {
		isEnd = true;
		return "You fled from the battle!";
	}
	else if (combatRole.role.Vitality <= 0) {
		isEnd = true;
		return "Role is dead!";
	}
	else if (combatEnemy.enemy.Vitality <= 0) {
		isEnd = true;
		return "Enemy is dead!";
	}
	isEnd = false;
	return "";
}

bool comparePriority(const Entity& entity1, const Entity& entity2) {
	if (entity1.Speed != entity2.Speed)
		return entity1.Speed > entity2.Speed;
	if ((entity1.PAttack + entity1.MAttack) != (entity2.PAttack + entity2.MAttack))
		return (entity1.PAttack + entity1.MAttack) < (entity2.PAttack + entity2.MAttack);
	if ((entity1.PDefense + entity1.MDefense) != (entity2.PDefense + entity2.MDefense))
		return (entity1.PDefense + entity1.MDefense) < (entity2.PDefense + entity2.MDefense);
	return entity1.Vitality < entity2.Vitality;
}

void Combat::priorityJudge() {
	combatRole.priority = (combatRole.moveCount + 1) / (double)combatRole.role.Speed * 100;
	combatEnemy.priority = (combatEnemy.moveCount + 1) / (double)combatEnemy.enemy.Speed * 100;

	if (combatRole.priority != combatEnemy.priority)
		isRoleTurn = combatRole.priority < combatEnemy.priority;
	else {
		if (comparePriority(combatRole.role, combatEnemy.enemy))
			isRoleTurn = true;
		else
			isRoleTurn = false;
	}

	if (isRoleTurn) {
		attacker = &combatRole.role;
		defender = &combatEnemy.enemy;
	}
	else {
		attacker = &combatEnemy.enemy;
		defender = &combatRole.role;
	}
}


void Combat::showStatus() {
	PrintString(1, 1, "Role: " + combatRole.role.name);
	PrintString(1, 2, "Vitality: " + std::to_string(combatRole.role.Vitality));
	PrintString(1, 3, "Focus: " + std::to_string(combatRole.role.Focus));
	PrintString(1, 4, "Speed: " + std::to_string(combatRole.role.Speed));
	PrintString(1, 5, "PAttack: " + std::to_string(combatRole.role.PAttack));
	PrintString(1, 6, "PDefense: " + std::to_string(combatRole.role.PDefense));
	PrintString(1, 7, "MAttack: " + std::to_string(combatRole.role.MAttack));
	PrintString(1, 8, "MDefense: " + std::to_string(combatRole.role.MDefense));
	PrintString(1, 9, "HitRate: " + std::to_string(combatRole.role.HitRate));
	PrintString(1, 10, "Priority: " + std::to_string(combatRole.priority));
	PrintString(1, 11, "MoveCount: " + std::to_string(combatRole.moveCount));
	
	PrintString(1, 13, "Role's actSkills: ");
	int i = 0;
	for (i = 0; i < combatRole.role.actSkills.size(); i++) {
		auto a = combatRole.role.actSkills[i];
		PrintString(1, 14 + i, a.name + " cooldown: " + std::to_string(a.curCooldown));
	}
	int next = 14 + i + 1;
	PrintString(1, next, "Role's buff: ");
	for (int i = 0; i < combatRole.role.buffs.size(); i++) {
		auto a = combatRole.role.buffs[i];
		PrintString(1, next + i + 1, a.name + " duration " + std::to_string(a.effectDuration));
	}


	PrintString(76, 1, "Enemy: " + combatEnemy.enemy.name);
	PrintString(76, 2, "Vitality: " + std::to_string(combatEnemy.enemy.Vitality));
	PrintString(76, 3, "Focus: " + std::to_string(combatEnemy.enemy.Focus));
	PrintString(76, 4, "Speed: " + std::to_string(combatEnemy.enemy.Speed));
	PrintString(76, 5, "PAttack: " + std::to_string(combatEnemy.enemy.PAttack));
	PrintString(76, 6, "PDefense: " + std::to_string(combatEnemy.enemy.PDefense));
	PrintString(76, 7, "MAttack: " + std::to_string(combatEnemy.enemy.MAttack));
	PrintString(76, 8, "MDefense: " + std::to_string(combatEnemy.enemy.MDefense));
	PrintString(76, 9, "HitRate: " + std::to_string(combatEnemy.enemy.HitRate));
	PrintString(76, 10, "Priority: " + std::to_string(combatEnemy.priority));
	PrintString(76, 11, "MoveCount: " + std::to_string(combatEnemy.moveCount));


	PrintString(76, 13, "Enemy's actSkills: ");
	for (i = 0; i < combatEnemy.enemy.actSkills.size(); i++) {
		auto a = combatEnemy.enemy.actSkills[i];
		PrintString(76, 14 + i, a.name + " cooldown: " + std::to_string(a.curCooldown));
	}
	next = 14 + i + 1;
	PrintString(76, next, "Enemy's buff: ");
	for (int i = 0; i < combatEnemy.enemy.buffs.size(); i++) {
		auto a = combatEnemy.enemy.buffs[i];
		PrintString(76, next + i + 1, a.name + " duration " + std::to_string(a.effectDuration));
	}

	if (isRoleTurn) PrintString(25, 11, "It's Role's turn " + std::to_string(combatRole.moveCount));
	else PrintString(25,11,"It's Enemy's turn!");
}

void Combat::roleNewTurn(Entity& entity) {
	for (auto& a : entity.actSkills) {
		if (a.curCooldown > 0)
			a.curCooldown--;
	}
	for (auto& a : entity.buffs) {
		a.effectDuration--;
		if (a.effectDuration == 0) {
			a.disable(a, entity);
			entity.removeBuff(a);
		}
	}
}

void Combat::showCombatPanel(int selectIndex) {
	for (int i = 0; i <= combatRole.role.actSkills.size(); i++) {
		if (i == selectIndex) {
			std::cout << FG_BLUE;
		}
		if (i != combatRole.role.actSkills.size())
			PrintString(26, i+1, std::to_string(i + 1) + ". " + combatRole.role.actSkills[i].name + CLOSE);
		else PrintString(26, i+1, std::to_string(i + 1) + ". " + "use item" + CLOSE);
	}
}

bool Combat::HandleUseItemEvent() {
	int select_index = 0;
	std::vector<std::pair<std::shared_ptr<Else>, int>> items;
	for (auto i = 0; i < Role::Bag.size(); i++)
	{
		if (auto item = std::dynamic_pointer_cast<Else>(Role::Bag[i])) {
			switch (item->Type) {
			case ElseType::Godsbeard:
			case ElseType::GoldenRoot:
				items.push_back({ item ,i });
				break;
			default: break;
			}
		}
	}
	if (items.size() == 0)return false;
	PrintUseItemFrame();
	while (true) {
		int y = 1;
		for (auto& item : items) {
			std::string bag_str = "";
			if (select_index == y - 1) bag_str += FG_YELLOW;
			bag_str += item.first->EquipmentTypeToString() + CLOSE;
			PrintString(1, y++, bag_str);
		}
		int press = ctl.GetInput();
		if (ctl.isBackspace(press)) return false;

		if (HandleUseItemInput(select_index, press, items)) {
			return true;
		}

	}
}

bool Combat::HandleUseItemInput(int& select_index, int press, std::vector<std::pair<std::shared_ptr<Else>, int>> items) {
	if (ctl.isUp(press)) {
		if (select_index == 0) return false;
		select_index--;
	}
	else if (ctl.isDown(press)) {
		if (select_index == items.size() - 1) return false;
		select_index++;
	}
	else if (ctl.isEnter(press)) {
		HandleUseItem(std::dynamic_pointer_cast<Else>(items[select_index].first));
		Role::Bag.erase(Role::Bag.begin() + items[select_index].second);
		return true;
	}
	return false;
}

void Combat::HandleUseItem(std::shared_ptr<Else> item) {
	switch (item->Type) {
	case ElseType::Godsbeard:
		combatRole.role.Vitality = (combatRole.role.Vitality + 25 > combatRole.role.MaxVitality ? combatRole.role.MaxVitality : combatRole.role.Vitality + 25);
		break;
	case ElseType::GoldenRoot:
		combatRole.role.Focus = (combatRole.role.Focus + 3 > combatRole.role.MaxFocus ? combatRole.role.MaxFocus : combatRole.role.Focus + 3);
		break;
	default: break;
	}
}


bool Combat::processInput(int& selectIndex, int press) {
	isTurnChange = false;
	if (ctl.isUp(press)) {
		if (selectIndex > 0)
			selectIndex--;
	}
	else if (ctl.isDown(press)) {
		if (selectIndex < combatRole.role.actSkills.size())
			selectIndex++;
	}
	else if (ctl.isEnter(press)) {
		if (selectIndex == combatRole.role.actSkills.size()) {
			isTurnChange = HandleUseItemEvent();
			if (isTurnChange) {
				PrintString(50, 1, "use item!");
				NeedRefreshFrame = true;
			}
		}
		else {
			ActiveSkills& skill = combatRole.role.actSkills[selectIndex];
			isTurnChange = battlePhase(skill);
		}
		if (isTurnChange)combatRole.moveCount++;
	}
	return isTurnChange;
}

bool Combat::battlePhase(ActiveSkills& skill) {
	if (skill.curCooldown > 0) {
		resultLog = "This skill is in cooldown!";
		return false;
	}
	if (skill.needTarget)
		defender = &chooseTarget(skill);
	int useFocus = chooseFocus(skill.needDice);

	if (skill.execute(skill, *attacker, *defender, useFocus, resultLog)) {
		skill.curCooldown = skill.cooldown;
		return true;
	}
	return false;
}

Entity& Combat::chooseTarget(ActiveSkills skill) {
	int selectX = 0;
	while (true) {
		showStatus();
		PrintString(25, 8, "Choose your target:");

		if (skill.needTarget == 1) {
			if (selectX == 0) {
				PrintString(25, 9, FG_YELLOW + combatEnemy.enemy.name + CLOSE);
			}
			else {
				PrintString(25, 9, combatEnemy.enemy.name);
			}
		}
		else if (skill.needTarget == 2) {
			if (selectX == 0) {
				PrintString(25, 9, FG_YELLOW + combatRole.role.name + CLOSE);
			}
			else {
				PrintString(25, 9, combatRole.role.name);
			}
		}

		int press = ctl.GetInput();
		if (ctl.isEnter(press)) {
			PrintString(25, 8, ReturnSpace(50));
			PrintString(25, 9, ReturnSpace(50));

			if (skill.needTarget == 1) {
				return combatEnemy.enemy;
			}
			else if (skill.needTarget == 2) {
				return combatRole.role;
			}
		}
	}
}


int Combat::chooseFocus(int maxFocus) {
	int useFocus = 0;
	while (true) {
		showStatus();
		PrintString(25, 8, "(You have " + std::to_string(combatRole.role.Focus - useFocus) + " focus left)");

		std::string focus_str = "";
		for (int i = 0; i < combatRole.role.MaxFocus; i++) {
			if (i < useFocus) {
				focus_str += FG_RED;
			}
			else if (i + 1 > combatRole.role.Focus) {
				focus_str += FG_GREY;
			}
			else {
				focus_str += FG_YELLOW;
			}
			focus_str += '*' + CLOSE;
		}
		PrintString(25, 9, focus_str);

		int press = ctl.GetInput();
		if (ctl.isRight(press)) {
			if (useFocus < combatRole.role.Focus && useFocus < maxFocus) {
				useFocus++;
			}
		}
		else if (ctl.isLeft(press)) {
			if (useFocus > 0) {
				useFocus--;
			}
		}
		else if (ctl.isEnter(press)) {
			break;
		}
	}
	combatRole.role.Focus -= useFocus;
	return useFocus;
}

void Combat::effectAutoBuff() {
	for (auto& a : combatRole.role.buffs) {
		if (a.effectTime == EffectTime::Auto) {
			a.execute(a, combatRole.role);
		}
	}
	for (auto& a : combatEnemy.enemy.buffs) {
		if (a.effectTime == EffectTime::Auto) {
			a.execute(a, combatEnemy.enemy);
		}
	}
}

void Combat::effectTurnStartBuff(Entity& entity, bool& isdizzy) {
	for (auto& a : entity.buffs) {
		if (a.effectTime == EffectTime::TurnStart) {
			if (a.Type == BuffType::Dizziness) {
				isdizzy = true;
			}
			a.execute(a, entity);
		}
	}
}

void Combat::turnEnd() {
	for (auto& a : combatRole.role.buffs) {
		a.disable(a, combatRole.role);
	}
	for (auto& a : combatEnemy.enemy.buffs) {
		a.disable(a, combatEnemy.enemy);
	}
}