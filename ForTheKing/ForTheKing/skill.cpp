#include "skill.h"
#include "entity.h"

Skills::Skills(){}

Skills::Skills(const std::string& name, const std::string& description)
	: name(name), description(description) {}

ActiveSkills::ActiveSkills(ActiveSkillType type): Type(type) {
	switch (type) {
	case ActiveSkillType::Attack:
		name = "Attack";
		description = "Attack the enemy.";
		maxFocus = 1;
		break;
	case ActiveSkillType::Flee:
		name = "Flee";
		description = "Flee from the battle.";
		maxFocus = 1;
		break;
	// test
	case ActiveSkillType::test:
		name = "test";
		description = "test";
		maxFocus = 5;
		break;
	// test
	case ActiveSkillType::actSkillError:
		name = "Error";
		description = "Error";
		break;
		// 其他技能...
	}
}

ActiveSkills::ActiveSkills() : Type(ActiveSkillType::actSkillError) {}

ActiveSkills::ActiveSkills(ActiveSkillType type, const std::string& name, const std::string& description)
	: Skills(name, description), Type(type) {}

bool shootCraps(int amont, double chance, int useFocus = 0) {
	int win = 0;
	for (int i = 0; i < amont; i++)
	{
		if (i < useFocus || (rand() % 100) < chance) {
			win++;
		}
	}
	return win;
}

bool ActiveSkills::execute(Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	switch (Type) {
	case ActiveSkillType::Attack:
		return doAttack(attacker, defender, useFocus, result);
		break;
	case ActiveSkillType::Flee:
		return doFlee( attacker, useFocus, result);
		break;
	//test
	case ActiveSkillType::test:
		return dotest(attacker, defender, useFocus, result);
	//test
	case ActiveSkillType::actSkillError:
		result = "Error";
		return false;
		// 其他技能...
	}
}

bool doAttack(Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	if (shootCraps(1, attacker.HitRate,useFocus))
	{
		defender.Vitality -= attacker.PAttack;
		result = attacker.name + " hit " + defender.name + " for " + std::to_string(attacker.PAttack) + " damage!";
		return true;
	}
	result = attacker.name + "'s attack fail!";
	return false;
}

bool doFlee(Entity& attacker, int useFocus, std::string& result) {
	double chance = attacker.Vitality / (double)(attacker.MaxVitality + attacker.PDefense + attacker.MDefense) * attacker.Speed;
	chance = chance > 98 ? 98 : chance;
	if (shootCraps(1, chance, useFocus))
	{
		result = "Flee success!";
		return true;
	}
	result = "Flee fail!";
	return false;
}

//test
bool dotest(Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	result = "test";
	return true;
}
//test

PassiveSkills::PassiveSkills(PassiveSkillType type): Type(type) {
	switch (type) {
	
	}
}

PassiveSkills::PassiveSkills(PassiveSkillType type, const std::string& name, const std::string& description)
	: Skills(name, description), Type(type) {}

bool PassiveSkills::execute(Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	switch (Type) {
		return false;
	}
	return false;
}