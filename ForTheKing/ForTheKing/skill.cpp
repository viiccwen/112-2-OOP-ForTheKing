#include <cmath>

#include "skill.h"
#include "entity.h"
#include "damagePhase.h"

Skills::Skills() {}

ActiveSkills::ActiveSkills() {}

ActiveSkills::ActiveSkills(ActiveSkillType type) : Type(type), curCooldown(0), execute(doNothing) {
	switch (type) {
	case ActiveSkillType::Attack:
		name = "Attack";
		description = "Attack the enemy.";
		category = DamageType::PhysicalSingalAttack;
		cooldown = 0;
		needDice = 1;
		needTarget = 1;
		execute = doAttack;
		break;
	case ActiveSkillType::Flee:
		name = "Flee";
		description = "Flee from the battle.";
		category = DamageType::None;
		cooldown = 0;
		needDice = 1;
		needTarget = 0;
		execute = doFlee;
		break;
	case ActiveSkillType::Provoke:
		name = "Provoke";
		description = "Make enemy angry 3 turns.";
		category = DamageType::None;
		cooldown = 3;//2+1
		needDice = 1;
		needTarget = 1;
		execute = doProvoke;
		break;
	case ActiveSkillType::Shock_Blast:
		name = "Shock Blast";
		description = "Attack all enemies.";
		category = DamageType::MagicMultiAttack;
		cooldown = 2;//1+1
		needDice = 3;
		needTarget = 0;
		execute = doShock_Blast;
		break;
	case ActiveSkillType::Heal:
		name = "Heal";
		description = "Heal a role.";
		category = DamageType::None;
		cooldown = 2;//1+1
		needDice = 2;
		needTarget = 2;
		execute = doHeal;
		break;
	case ActiveSkillType::SpeedUp:
		name = "Speed Up";
		description = "Speed Up a role 1 turn.";
		category = DamageType::None;
		cooldown = 4;//3+1
		needDice = 2;
		needTarget = 2;
		execute = doSpeedUp;
		break;
	case ActiveSkillType::Poison:
		name = "Poison";
		description = "Poison enemy 1 turn.";
		category = DamageType::None;
		cooldown = 4;//3+1
		needDice = 2;
		needTarget = 1;
		execute = doPoison;
		break;
	case ActiveSkillType::Nothing:
		name = "doNothing";
		description = "doNothing";
		cooldown = 0;
		needDice = 0;
		needTarget = 0;
		execute = doNothing;
		break;
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

bool haveBuff(Entity& attacker,BuffType type) {
	return std::find_if(attacker.buffs.begin(), attacker.buffs.end(),
		[type](const Buffs& buff) { return buff.Type == type; }) != attacker.buffs.end();
}

bool doAttack(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	int attack = skill.category == DamageType::PhysicalSingalAttack ? attacker.PAttack : attacker.MAttack;
	double chance = attacker.HitRate;
	if (haveBuff(attacker, BuffType::Angry))
		chance *= 0.7;
	
	// todo: critical
	// int win = skill.needDice;
	int win = shootCraps(skill.needDice, chance, useFocus);
	int damage = 0;
	bool isCritical = false;
	if (win == 0) {//Fumble
		result = attacker.name + "'s Attack fail!";
		return true;
	}
	else if (win == skill.needDice && skill.needDice>1) {//Critical
		damage = attack;
		isCritical = true;
	}
	else {
		damage = (double)attack * 0.5 * (double)win / skill.needDice;
	}
	DamagePhase(attacker, defender, damage, skill.category, isCritical, result);
	return true;


}

bool doFlee(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.Vitality / (double)(attacker.MaxVitality + attacker.PDefense + attacker.MDefense) * attacker.Speed;
	if (haveBuff(attacker, BuffType::Angry))
		chance *= 0.7;
	chance = chance > 98 ? 98 : chance;
	if (shootCraps(skill.needDice, chance, useFocus))
	{
		result = "Flee success!";
	}
	else result = "Flee fail!";
	return true;
}

bool doProvoke(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.Vitality/ (double)(attacker.MaxVitality + attacker.PDefense + attacker.MDefense) * attacker.Speed;
	if (haveBuff(attacker, BuffType::Angry))
		chance *= 0.7;
	int win = shootCraps(skill.needDice, chance, useFocus);
	if (win == 0) {
		result = attacker.name + "'s Provoke fail!";
		return true;
	}
	else if (win == skill.needDice && skill.needDice > 1) {
		defender.addBuff(Buffs(BuffType::Angry, 4));//3+1
		result = attacker.name + " Provoke " + defender.name + " 3 turn!";
	}
	else {
		defender.addBuff(Buffs(BuffType::Angry, 4));//3+1
		result = attacker.name + " Provoke " + defender.name + " 3 turn!";
	}
	return 0;
}

bool doShock_Blast(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.HitRate - 5;
	if(haveBuff(attacker,BuffType::Angry))
		chance *= 0.7;
	int win = shootCraps(skill.needDice, chance, useFocus);
	int damage = 0;
	bool isCritical = false;
	if (win == 0) {
		//Fumble
		result = attacker.name + "'s Shock Blast fail!";
		return true;
	}
	else if (win == skill.needDice && skill.needDice > 1) {
		//Critical
		damage = (double)attacker.MAttack * 0.5;
		isCritical = true;
	}
	else {
		damage = (double)attacker.MAttack * 0.5 * (double)win / skill.needDice;
	}
	DamagePhase(attacker, defender, damage, skill.category, isCritical, result);
	return true;
}

bool doHeal(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.HitRate - 5;
	int win = shootCraps(skill.needDice, chance, useFocus);
	if (win == 0) {
		result = attacker.name + "'s Heal fail!";
		return true;
	}
	else if (win == skill.needDice && skill.needDice > 1) {
		int heal = (double)attacker.MAttack * 1.5;
		defender.Vitality = defender.Vitality + heal > defender.MaxVitality ? defender.MaxVitality : defender.Vitality + heal;
		result = attacker.name + " heal " + defender.name + " for " + std::to_string(heal) + "! (Critical)";
	}
	else {
		int heal = (double)attacker.MAttack * 1.5 * (double)win / skill.needDice;
		defender.Vitality = defender.Vitality + heal > defender.MaxVitality ? defender.MaxVitality : defender.Vitality + heal;
		result = attacker.name + " heal " + defender.name + " for " + std::to_string(heal) + "!";
	}
	return true;
}

bool doSpeedUp(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.HitRate;
	int win = shootCraps(skill.needDice, chance, useFocus);
	if (win == skill.needDice) {
		defender.addBuff(Buffs(BuffType::SpeedUp, 2));//1+1
		result = attacker.name + " Speed Up " + defender.name + " 1 turn!";
	}
	else {
		result = attacker.name + "'s Speed Up fail!";
	}
	return true;
}

bool doPoison(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	double chance = attacker.HitRate;
	int win = shootCraps(skill.needDice, chance, useFocus);
	if (win != 0) {
		defender.addBuff(Buffs(BuffType::Poisoned, 5));//1+1
		result = attacker.name + " Poison " + defender.name + " 4 turn!";
	}
	else {
		result = attacker.name + " Poison fail!";
	}
	return true;
}

bool doNothing(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result) {
	result = "do nothing";
	return true;
}

Buffs::Buffs() {}

Buffs::Buffs(BuffType type, int duration) : Type(type), effectDuration(duration),execute(doNothing), disable(doNothing), delta(0) {
	switch (Type)
	{
	case BuffType::Run:
		//i don't know wtf doc says
		break;
	case BuffType::Hammer_Splash:
		name = "Hammer Splash";
		description = "When crtical attack single target,dizzy target for 1 turn";
		effectTime = EffectTime::BeforeDamagePhase;
		break;
	case BuffType::Destroy:
		name = "Destroy";
		description = "When attack,random remove enemy's equipment.";
		effectTime = EffectTime::AfterDamagePhase;
		execute = doDestroy;
		break;
	case BuffType::Fortify:
		name = "Fortify";
		description = "When attacked by single attack, reduce 10% damage.";
		effectTime = EffectTime::BeforeDamagePhase;
		break;
	case BuffType::Angry:
		name = "Angry";
		description = "Reduce the success rate of dice by 30%.";
		effectTime = EffectTime::TurnStart;
		disable = doNothing;
		break;
	case BuffType::Dizziness:
		name = "Dizziness";
		description = "Can't move.";
		effectTime = EffectTime::TurnStart;
		break;
	case BuffType::Poisoned:
		name = "Poisoned";
		description = "Lose 10% of vitality every turn.";
		effectTime = EffectTime::TurnStart;
		execute = doPoisoned;
		break;
	case BuffType::SpeedUp:
		name = "Speed Up";
		description = "Increase 50% Stat::Speed.";
		effectTime = EffectTime::Auto;
		execute = doSpeedUp;
		disable = disableSpeedUp;
		break;
	default:
		break;
	}
}

bool doRun(Buffs& buff, Entity& attacker) {
	//todo implement run
	return 0;
}

bool doDestroy(Buffs& buff, Entity& attacker) {
	//todo implement destroy random remove enemy's equipment
	return 0;
}

bool doPoisoned(Buffs& buff, Entity& attacker) {
	int damage = std::floor((double)attacker.Vitality * 0.1);
	attacker.Vitality -= damage>0?damage:1;
	return false;
}
bool doSpeedUp(Buffs& buff, Entity& attacker) {
	buff.delta = (double)attacker.Speed * 0.5;
	attacker.Speed += buff.delta;
	return false;
}

bool disableRun(Buffs& buff, Entity& attacker) {
	return 0;
}

bool disableSpeedUp(Buffs& buff, Entity& attacker) {
	attacker.Speed -= buff.delta;
	buff.delta = 0;
	return 0;
}
bool doNothing(Buffs& buff, Entity& attacker) {
	return 0;
}
