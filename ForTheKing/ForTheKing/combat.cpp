#include "combat.h"

#include <iostream>

CombatRole::CombatRole(Role& _role) : role(_role), priority(0), moveCount(0)/*, useFocus(0)*/ {
}

CombatEnemy::CombatEnemy(Enemy& _enemy) : enemy(_enemy), priority(0), moveCount(0), useFocus(0) {
}

Combat::Combat(Role& _role, Enemy& _enemy) : combatRole(_role), combatEnemy(_enemy){
	resultLog = "";
	Combat::combatLoop();
}

void Combat::combatLoop() {
	int selectIndex = 0;
	while (true) {
		resultLog = Combat::isCombatEnd();
		if (isEnd) {
			Combat::showStatus();
			std::cout << '\n' << resultLog << '\n';
			system("pause");
			break;
		}

		Combat::priorityJudge();
		Combat::showStatus();

		if (isRoleTurn) {
			if(isRoleMove)
				Combat::roleNewTurn();
			Combat::showStatus();
			Combat::showCombatPanel(selectIndex);
			int press = ctl.GetInput();
			processInput(selectIndex, press);
			isRoleMove = false;
		}
		else {
			if (combatEnemy.enemy.actSkills.size() == 0) {
				std::cout << '\n' << combatEnemy.enemy.name << " does not have skill" << '\n';
				combatEnemy.moveCount++;
			}
			int enemyRandomActSkill = rand() % combatEnemy.enemy.actSkills.size();
			ActiveSkills& skill = combatEnemy.enemy.actSkills[enemyRandomActSkill];
			skill.execute(skill,combatEnemy.enemy, combatRole.role, 0, resultLog);
			/*std::cout << '\n' << combatEnemy.enemy.name << " use " << skill.name << '\n';
			std::cout<<'\n' << resultLog << '\n';*/
			combatEnemy.moveCount++;
			//system("pause");
		}
		if (resultLog != "") {
		std::cout<<resultLog<<"\n";
		system("pause");
		}
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
	if (isRoleTurn)std::cout << "It's Role's turn "<<combatRole.moveCount<<"\n\n";
	else std::cout << "It's Enemy's turn!\n\n";
}

void Combat::roleNewTurn() {
	for (auto& a : combatRole.role.actSkills) {
		if(a.curCooldown > 0)
			a.curCooldown--;
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

bool Combat::processInput(int& selectIndex, int press) {
	isRoleMove = false;
	//chose skill
	if (ctl.isUp(press)) {
		if (selectIndex > 0)
			selectIndex--;
	}
	else if (ctl.isDown(press)) {
		if (selectIndex < combatRole.role.actSkills.size())
			selectIndex++;
	}
	//confirm
	else if (ctl.isEnter(press)) {
		if (selectIndex == combatRole.role.actSkills.size()){
			isRoleMove = true;
			resultLog = "Use item!";
			// TODO: Use Item
		}
		else {
			ActiveSkills& skill = combatRole.role.actSkills[selectIndex];
			isRoleMove = battlePhase(skill);
		}
		if(isRoleMove)combatRole.moveCount++;
	}
	return isRoleMove;
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