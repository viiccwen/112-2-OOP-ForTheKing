#include <cmath>

#include "skill.h"
#include "entity.h"
#include "damagePhase.h"

Skills::Skills(): category(SkillCategoryType::Other) {}

ActiveSkills::ActiveSkills(ActiveSkillType type, Entity& entity): Type(type),curCooldown(0),execute(nullptr){
	switch (type) {
	case ActiveSkillType::Attack:
		name = "Attack";
		description = "Attack the enemy.";
		if(entity.weapon.Type == WeaponType::MagicWand || entity.weapon.Type == WeaponType::RitualSword)
			category = SkillCategoryType::MagicAttack;
		else category = SkillCategoryType::PhysicalAttack;
		cooldown = 0;
		needDice = 1;
		//needDice = entity.weapon.attackCooldown;
		needTarget = 1;
		execute = doAttack;
		break;
	case ActiveSkillType::Flee:
		name = "Flee";
		description = "Flee from the battle.";
		category = SkillCategoryType::Other;
		cooldown = 0;
		needDice = 1;
		needTarget = 0;
		execute = doFlee;
		break;
	case ActiveSkillType::Provoke:
		name = "Provoke";
		description = "Make enemy angry 3 turns.";
		category = SkillCategoryType::GiveDebuff;
		cooldown = 3;//2+1
		needDice = 1;
		needTarget = 1;
		execute = doProvoke;
		break;
	case ActiveSkillType::Shock_Blast:
		name = "Shock Blast";
		description = "Attack all enemies.";
		category = SkillCategoryType::MagicAttack;
		cooldown = 2;//1+1
		needDice = 3;
		needTarget = 0;
		execute = doShock_Blast;
		break;
	case ActiveSkillType::Heal:
		name = "Heal";
		description = "Heal a role.";
		category = SkillCategoryType::Other;
		cooldown = 1;
		needDice = 2;
		needTarget = 2;
		execute = doHeal;
		break;
	case ActiveSkillType::SpeedUp:
		name = "Speed Up";
		description = "Speed Up a role 1 turn.";
		category = SkillCategoryType::GiveBuff;
		cooldown = 3;
		needDice = 2;
		needTarget = 2;
		execute = doSpeedUp;
		break;
	case ActiveSkillType::Nothing:
		name = "doNothing";
		description = "doNothing";
		cooldown = 0;
		needDice = 0;
		needTarget = 0;
		execute = doNothing;
		break;
	case ActiveSkillType::actSkillError:
		name = "Error";
		description = "Error";
		break;
		// 其他技能...
	}
}

ActiveSkills::ActiveSkills() : Type(ActiveSkillType::actSkillError){}

PassiveSkills::PassiveSkills(PassiveSkillType type): Type(type) {
	switch (type) {
	
	}
}

int shootCraps(int amont, double chance, int useFocus = 0) {
	int win = 0;
	for (int i = 0; i < amont; i++)
	{
		if (i < useFocus || (rand() % 100) < chance) {
			win++;
		}
	}
	return win;
}

int doAttack(ActiveSkills& skill,Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	if (shootCraps(1, attacker.HitRate, useFocus))
	{
		int damage = attacker.PAttack;
		DamagePhase(attacker, defender, damage, skill.category,0, result);
	}
	else result = attacker.name + "'s attack fail!";
	return true;
}

int doFlee(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.Vitality / (double)(attacker.MaxVitality + attacker.PDefense + attacker.MDefense) * attacker.Speed;
	chance = chance > 98 ? 98 : chance;
	if (shootCraps(1, chance, useFocus))
	{
		result = "Flee success!";
	}
	else result = "Flee fail!";
	return true;
}

int doProvoke(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	/**/
	return 0;
}

int doShock_Blast(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.HitRate-5;
	int win = shootCraps(3, chance, useFocus);
	int damage = 0;
	bool isCritical = false;
	switch (win) {
	case 0:
		//Fumble
		result = attacker.name + "'s Shock Blast fail!";
		return true;
		break;
	case 3:
		//Critical
		damage = (double)attacker.MAttack * 0.5;
		isCritical = true;
		break;
	default:
		damage = (double)attacker.MAttack * 0.5;
		break;
	}
	DamagePhase(attacker, defender, damage, skill.category,isCritical ,result);
	return true;
}

int doHeal(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	
	return true;
}

int doSpeedUp(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	
	return true;
}

int doNothing(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	result = "do nothing";
	return true;
}

//bool PassiveSkills::execute(Entity& attacker, Entity& defender, int useFocus, std::string& result) {
//	switch (Type) {
//		return false;
//	}
//	return false;
//}