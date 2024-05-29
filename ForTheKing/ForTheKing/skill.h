#ifndef _SKILL_H_
#define _SKILL_H_

#include <string>

enum class ActiveSkillType {
	Attack,
	Flee,
	Provoke,
	Shock_Blast,
	Heal,
	SpeedUp,
	Nothing,
	actSkillError
};

enum class PassiveSkillType {
	Run,
	Hammer_Splash,
	Destroy,
	Fortify
};

enum class SkillCategoryType {
	PhysicalAttack,
	MagicAttack,
	GiveBuff,
	GiveDebuff,
	Buff,
	Debuff,
	Other
};

class Entity;

class Skills {
public:
	std::string name;
	std::string description;
	int needDice;
	SkillCategoryType category;

	Skills();
};

class ActiveSkills : public Skills {
public:
	ActiveSkillType Type;
	int cooldown;
	int needTarget;//0: no need, 1: need enemy, 2: need role
	int curCooldown;

	ActiveSkills();
	ActiveSkills(ActiveSkillType type,Entity& entity);
	int (*execute)(ActiveSkills& skill,Entity& attacker, Entity& defender, int useFocus, std::string& resultLog);
	void refreshSkill();
};

class PassiveSkills : public Skills {
public:
	PassiveSkillType Type;

	PassiveSkills(PassiveSkillType type);	
};

int doAttack(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);//return damage
int doFlee(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);//1: success, 0: fail
int doProvoke(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);//1: success, 0: fail
int doShock_Blast(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);//return damage
int doHeal(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);//return heal
int doSpeedUp(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);//1: success, 0: fail
int doNothing(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);

#endif // !_SKILL_H_