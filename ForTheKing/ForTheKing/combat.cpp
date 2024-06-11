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
	while (true) {
		resultLog = Combat::isCombatEnd();
		if (isEnd) {
			Combat::showStatus();
			std::cout << '\n' << resultLog << '\n';
			system("pause");
			break;
		}

		Combat::effectAutoBuff();
		if (isTurnChange)
			Combat::priorityJudge();
		//Combat::showStatus();

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
			std::cout << resultLog << "\n";
			system("pause");
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
	system("cls");

	std::cout << "Role: " << combatRole.role.name << "\t\t" << "Enemy: " << combatEnemy.enemy.name << '\n';
	std::cout << "Vitality: " << combatRole.role.Vitality << "\t\t" << "Vitality: " << combatEnemy.enemy.Vitality << '\n';
	std::cout << "Focus: " << combatRole.role.Focus << "\t\t" << "Focus: " << combatEnemy.enemy.Focus << '\n';
	std::cout << "Speed: " << combatRole.role.Speed << "\t\t" << "Speed: " << combatEnemy.enemy.Speed << '\n';
	std::cout << "PAttack: " << combatRole.role.PAttack << "\t\t" << "PAttack: " << combatEnemy.enemy.PAttack << '\n';
	std::cout << "PDefense: " << combatRole.role.PDefense << "\t\t" << "PDefense: " << combatEnemy.enemy.PDefense << '\n';
	std::cout << "MAttack: " << combatRole.role.MAttack << "\t\t" << "MAttack: " << combatEnemy.enemy.MAttack << '\n';
	std::cout << "MDefense: " << combatRole.role.MDefense << "\t\t" << "MDefense: " << combatEnemy.enemy.MDefense << '\n';
	std::cout << "HitRate: " << combatRole.role.HitRate << "\t\t" << "HitRate: " << combatEnemy.enemy.HitRate << '\n';
	std::cout << "Priority: " << combatRole.priority << "\t\t" << "Priority: " << combatEnemy.priority << '\n';
	std::cout << "MoveCount: " << combatRole.moveCount << "\t\t" << "MoveCount: " << combatEnemy.moveCount << '\n';
	std::cout << '\n';

	std::cout << "Role's actSkills: \n";
	for (auto a : combatRole.role.actSkills)
		std::cout << a.name << " cooldown: " << a.curCooldown << '\t';
	std::cout << "\nRole's buff: \n";
	for (auto a : combatRole.role.buffs)
		std::cout << a.name << " duration: " << a.effectDuration << '\t';
	std::cout << "\nenemy's actSkills: \n";
	for (auto a : combatEnemy.enemy.actSkills)
		std::cout << a.name << " cooldown: " << a.curCooldown << '\t';
	std::cout << "\nenemy's buff: \n";
	for (auto a : combatEnemy.enemy.buffs)
		std::cout << a.name << " duration: " << a.effectDuration << '\t';
	std::cout << '\n';
	if (isRoleTurn)std::cout << "It's Role's turn " << combatRole.moveCount << "\n\n";
	else std::cout << "It's Enemy's turn!\n\n";
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
			std::cout << i + 1 << ". " << combatRole.role.actSkills[i].name << CLOSE << '\n';
		else std::cout << i + 1 << ". " << "use item" << CLOSE << '\n';
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
				resultLog = "Use item!";
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
		std::cout << "Choose your target:\n";

		if (skill.needTarget == 1) {
			if (selectX == 0) {
				std::cout << FG_YELLOW << combatEnemy.enemy.name << CLOSE << '\n';
			}
			else {
				std::cout << combatEnemy.enemy.name << '\n';
			}
		}
		else if (skill.needTarget == 2) {
			if (selectX == 0) {
				std::cout << FG_YELLOW << combatRole.role.name << CLOSE << '\n';
			}
			else {
				std::cout << combatRole.role.name << '\n';
			}
		}

		int press = ctl.GetInput();
		if (ctl.isEnter(press)) {
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
		std::cout << "(You have " << combatRole.role.Focus - useFocus << " focus left)\n";
		for (int i = 0; i < combatRole.role.MaxFocus; i++) {
			if (i < useFocus) {
				std::cout << FG_RED;
			}
			else if (i + 1 > combatRole.role.Focus) {
				std::cout << FG_GREY;
			}
			else {
				std::cout << FG_YELLOW;
			}
			std::cout << '*' << CLOSE;
		}
		std::cout << '\n';

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