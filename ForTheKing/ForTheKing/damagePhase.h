#ifndef _DAMAGE_H_
#define _DAMAGE_H_

#include "entity.h"

class DamagePhase
{
public:
	DamagePhase(Entity& attacker, Entity& defender,int damage, SkillCategoryType category,bool isCritical,std::string& result);
};

#endif // !_DAMAGE_H_