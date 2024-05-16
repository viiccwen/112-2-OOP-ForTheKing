#include "combat.h"

#include <iostream>

CombatRole::CombatRole(Role& _role) : role(_role), priority(0), moveCount(0), useFocus(0) {
}

CombatEnemy::CombatEnemy(Enemy& _enemy) : enemy(_enemy), priority(0), moveCount(0), useFocus(0) {
}

Combat::Combat(Role& _role, Enemy& _enemy) : combatRole(_role), combatEnemy(_enemy) {
	resultLog = "";
	Combat::combatLoop();
}

void Combat::combatLoop() {
	int selectIndex = 0;
	while (true) {
		resultLog = Combat::isCombatEnd();
		if (isEnd) {
			Combat::showStatus();
			std::cout<<'\n'<<resultLog<<'\n';
			system("pause");
			break;
		}

		Combat::priorityJudge();
		Combat::showStatus();

		if (isRoleTurn) {
			Combat::showCombatPanel(selectIndex);
			int press = ctl.GetInput();
			processInput(selectIndex, press);
		}
		else {
			system("pause");
			combatEnemy.moveCount++;
		}
	}
}

std::string Combat::isCombatEnd() {
	if (isEnd)return "Flee success!";
	
	if (combatRole.role.Vitality <= 0) {
		isEnd = true;
		return "Role is dead!";
	}
	else if (combatEnemy.enemy.Vitality <= 0) {
		isEnd = true;
		return "Enemy is dead!";
	}
	return "";
}

void Combat::priorityJudge() {
	combatRole.priority = (combatRole.moveCount + 1) / (double)combatRole.role.Speed * 100;
	combatEnemy.priority = (combatEnemy.moveCount + 1) / (double)combatEnemy.enemy.Speed * 100;

	if (combatRole.priority > combatEnemy.priority) {
		attacker = &combatEnemy.enemy;
		defender = &combatRole.role;
		isRoleTurn = false;
	}
	else {
		attacker = &combatRole.role;
		defender = &combatEnemy.enemy;
		isRoleTurn = true;
	}
}

void Combat::showStatus() {
	system("cls");

	std::cout << "Role: " << combatRole.role.name << "\t\t" << "Enemy: " << combatEnemy.enemy.name << '\n';
	std::cout << "Vitality: " << combatRole.role.Vitality << "\t\t" << "Vitality: " << combatEnemy.enemy.Vitality << '\n';
	if (isRoleTurn)std::cout << "It's Role's turn!\n\n";
	else std::cout << "It's Enemy's turn!\n\n";
}

void Combat::showCombatPanel(int selectIndex) {
	std::cout << "(You have " << combatRole.role.Focus - combatRole.useFocus << " focus left)\n";
	for (int i = 0; i < combatRole.role.MaxFocus; i++)
	{
		if (i < combatRole.useFocus) {
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

	std::string actions[] = { "Attack", "Use Item", "Flee" ,"do nothing" };
	for (int i = 0; i < 4; i++)
	{
		if (i == selectIndex) {
			std::cout << FG_BLUE;
		}
		std::cout << i + 1 << ". " << actions[i] << CLOSE << '\n';
	}
}


void Combat::processInput(int& selectIndex, int press) {
	//chose skill
	if (ctl.isUp(press)) {
		if (selectIndex > 0)
			selectIndex--;
	}
	else if (ctl.isDown(press)) {
		if (selectIndex < 3)
			selectIndex++;
	}
	//use focus
	else if (ctl.isRight(press)) {
		if (combatRole.useFocus < combatRole.role.Focus) {
			combatRole.useFocus++;
		}
	}
	else if (ctl.isLeft(press)) {
		if (combatRole.useFocus > 0) {
			combatRole.useFocus--;
		}
	}
	//confirm
	else if (ctl.isEnter(press)) {
		ActiveSkills skill;
		if (selectIndex == 0) {
			skill.doAttack(*attacker, *defender, combatRole.useFocus, resultLog);
		}
		else if (selectIndex == 1) {
			// TODO: Use Item
		}
		else if (selectIndex == 2) {
			skill.doFlee(*attacker, combatRole.useFocus, resultLog, isEnd);
		}
		else if (selectIndex == 3) {
			resultLog = attacker->name + " do nothing";
		}
		combatRole.moveCount++;
	}
	if (resultLog != "") {
		std::cout << '\n' << resultLog << '\n';
		system("pause");
	}
}