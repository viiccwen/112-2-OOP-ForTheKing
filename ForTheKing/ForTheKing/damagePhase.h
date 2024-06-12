#ifndef _DAMAGE_H_
#define _DAMAGE_H_

#include "entity.h"

class DamagePhase
{
public:
	DamagePhase(Entity& attacker, Entity& defender,int& damage, DamageType category,bool isCritical,std::string& result);
	void effectBeforeDamageBuff(Entity& attacker, Entity& defender, int& damage, DamageType category, bool isCritical);
	void effectAfterDamageBuff(Entity& attacker, Entity& defender);
};

#endif // !_DAMAGE_H_