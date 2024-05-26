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
	//test
	test
	//test
	,actSkillError
};

enum class PassiveSkillType {
	Run,
	Hammer_Splash,
	Destroy,
	Fortify
};

class Entity;

class Skills {
public:
	std::string name;
	std::string description;

	Skills();
	Skills(const std::string& name, const std::string& description);
	virtual bool execute(Entity& attacker, Entity& defender, int useFocus, std::string& result) = 0;
};

class ActiveSkills : public Skills {
public:
	ActiveSkillType Type;
	int maxFocus;

	ActiveSkills();
	ActiveSkills(ActiveSkillType type);
	ActiveSkills(ActiveSkillType type, const std::string& name, const std::string& description);

	bool execute(Entity& attacker, Entity& defender,int useFocus, std::string& result) override;
};

class PassiveSkills : public Skills {
public:
	PassiveSkillType Type;

	PassiveSkills(PassiveSkillType type);
	PassiveSkills(PassiveSkillType type, const std::string& name, const std::string& description);
	bool execute(Entity& attacker, Entity& defender, int useFocus, std::string& result) override;
};

bool doAttack(Entity& attacker, Entity& defender, int useFocus, std::string& result);
bool doFlee(Entity& attacker, int useFocus, std::string& result);
//test
bool dotest(Entity& attacker, Entity& defender, int useFocus, std::string& result);
//test

#endif // !_SKILL_H_