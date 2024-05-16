#include "skill.h"
#include "entity.h"

ActiveSkills::ActiveSkills(){
}

//ActiveSkills::ActiveSkills(ActiveSkillType type) : Type(type) {
//}
//
//bool ActiveSkills::operator==(const ActiveSkills& other) const {
//	return Type == other.Type;
//}
//
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

bool ActiveSkills::doAttack(Entity& attacker, Entity& defender, int useFocus,std::string& result) {
	if (shootCraps(1, attacker.HitRate))
	{
		defender.Vitality -= attacker.PAttack;
		result = attacker.name + " hit " + defender.name + " for " + std::to_string(attacker.PAttack) + " damage!";
		return true;
	}
	result = attacker.name + "'s attack fail!";
	return false;
}

bool ActiveSkills::doFlee(Entity& attacker,int useFocus, std::string& result,bool& isEnd) {
	double chance = attacker.Vitality/(double)(attacker.MaxVitality+attacker.PDefense+attacker.MDefense)*attacker.Speed;
	chance = chance > 98 ? 98 : chance;
	if (shootCraps(1, chance, useFocus))
	{
		result = "Flee success!";
		isEnd = true;
		return true;
	}
	result = "Flee fail!";
	isEnd = false;
	return false;
}