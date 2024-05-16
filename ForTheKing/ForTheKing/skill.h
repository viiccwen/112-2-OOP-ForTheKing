#ifndef _SKILL_H_
#define _SKILL_H_

#include <string>

enum class ActiveSkillType {
	Attack,
	Flee,
	Provoke,
	Shock_Blast,
	Heal,
	SpeedUp
};

enum class PassiveSkillType {
	Run,
	Hammer_Splash,
	Destroy,
	Fortify
};

class Skills {
};

class Entity;

class ActiveSkills : public Skills {
public:
	ActiveSkillType Type;

	ActiveSkills();
	ActiveSkills(ActiveSkillType type);
	bool operator==(const ActiveSkills& other) const;
	bool doAttack(Entity& user, Entity& target, int useFocus, std::string& result);
	bool doFlee(Entity& user,int useFocus, std::string& result,bool& isEnd);
};

class PassiveSkills : public Skills {
public:
	PassiveSkillType Type;
};

#endif // !_SKILL_H_