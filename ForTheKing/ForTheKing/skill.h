#ifndef _SKILL_H_
#define _SKILL_H_

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

class ActiveSkills : public Skills {
public:
	ActiveSkillType Type;
};

class PassiveSkills : public Skills {
public:
	PassiveSkillType Type;
};

#endif // !_SKILL_H_