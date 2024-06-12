#include "damagePhase.h"

#include <cmath>
#include "skill.h"

DamagePhase::DamagePhase(Entity& attacker, Entity& defender, int& damage, DamageType category,bool isCritical, std::string& result) {
	if (category == DamageType::PhysicalSingalAttack || category == DamageType::PhysicalMultiAttack) {
		double absorption = 1 - defender.PDefense / (double)(defender.PDefense + 50);
		damage = std::floor(absorption * damage);
		result = attacker.name + " hit " + defender.name + " for " + std::to_string(damage) + " physics damage!";
	}
	else if (category == DamageType::MagicSingalAttack|| category == DamageType::MagicMultiAttack) {
		double absorption = 1 - defender.MDefense / (double)(defender.MDefense + 50);
		damage = std::floor(absorption * damage);
		result = attacker.name + " hit " + defender.name + " for " + std::to_string(damage) + " magic damage!";
	}
	if (isCritical) {
		result += " (Critical)";
	}
	effectBeforeDamageBuff(attacker, defender, damage, category, isCritical);
	if (defender.Vitality - damage < 0) defender.Vitality = 0;
	else defender.Vitality -= damage;
	effectAfterDamageBuff(attacker, defender);
}

void DamagePhase::effectBeforeDamageBuff(Entity& attacker, Entity& defender, int& damage, DamageType category, bool isCritical) {
	for(auto& a : attacker.buffs){
		if(a.effectTime == EffectTime::BeforeDamagePhase){
			if (a.Type == BuffType::Hammer_Splash) {
				if (isCritical && (category == DamageType::PhysicalSingalAttack || category == DamageType::MagicSingalAttack))
				{
					defender.addBuff(Buffs(BuffType::Dizziness, 2));//1+1
				}
			}
			//other attacker buff
			//else {
			//	a.execute(a, attacker, defender);
			//}
		}
	}
	for (auto& a : defender.buffs) {
		if (a.effectTime == EffectTime::BeforeDamagePhase) {
			if (a.Type == BuffType::Fortify) {
				damage = std::floor((double)damage * 0.9);
			}
			//other defender buff
			//else {
			//	a.execute(a, attacker, defender);
			//}
		}
	}
}

void DamagePhase::effectAfterDamageBuff(Entity& attacker, Entity& defender) {
	for (auto& a : attacker.buffs) {
		if (a.effectTime == EffectTime::AfterDamagePhase) {
			//todo implement destroy random remove enemy's equipment
			a.execute(a,attacker);
		}
	}
}