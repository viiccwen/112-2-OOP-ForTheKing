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
	Nothing
};

enum class DamageType {
	PhysicalSingalAttack,
	MagicSingalAttack,
	PhysicalMultiAttack,
	MagicMultiAttack,
	None
};

enum class BuffType {
	//passive skills
	Run,
	Hammer_Splash,
	Destroy,
	Fortify,
	//buff debuff
	Angry,
	Dizziness,
	Poisoned,
	SpeedUp
};

enum class EffectTime {
	Auto,
	TurnStart,
	BeforeDamagePhase,
	AfterDamagePhase
};

class Entity;

class Skills {
public:
	std::string name;
	std::string description;

	Skills();
};

class ActiveSkills : public Skills {
public:
	ActiveSkillType Type;
	DamageType category;

	int cooldown;
	int needDice;
	int needTarget;//0: no need, 1: need enemy, 2: need role
	int curCooldown;

	ActiveSkills();
	ActiveSkills(ActiveSkillType type);
	bool (*execute)(ActiveSkills& skill,Entity& attacker, Entity& defender, int useFocus, std::string& resultLog);
	bool operator==(const ActiveSkills& other) const {
		return this->Type == other.Type;
	}
};

class Buffs :public Skills {
public:
	BuffType Type;

	EffectTime effectTime;
	int delta;
	int effectDuration;
	Buffs();
	Buffs(BuffType type, int duration);
	bool (*execute)(Buffs& buff, Entity& attacker);
	bool (*disable)(Buffs& buff, Entity& attacker);
	bool operator==(const Buffs& other) const {
		return this->Type == other.Type;
	}
};

int shootCraps(int amont, double chance, int useFocus);

bool doAttack(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doFlee(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doProvoke(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doShock_Blast(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doHeal(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doSpeedUp(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doNothing(ActiveSkills& skill, Entity& attacker, Entity& defender, int useFocus, std::string& result);

//todo implement run 
bool doRun(Buffs& buff, Entity& attacker);
//todo implement destroy random remove enemy's equipment
bool doDestroy(Buffs& buff, Entity& attacker);
bool doPoisoned(Buffs& buff, Entity& attacker);
bool doSpeedUp(Buffs& buff, Entity& attacker);

bool disableRun(Buffs& buff, Entity& attacker);
bool disableSpeedUp(Buffs& buff, Entity& attacker);
bool doNothing(Buffs& buff, Entity& attacker);

#endif // !_SKILL_H_