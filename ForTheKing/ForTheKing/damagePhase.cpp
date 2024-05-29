#include "damagePhase.h"

#include <cmath>

DamagePhase::DamagePhase(Entity& attacker, Entity& defender, int damage, SkillCategoryType category,bool isCritical, std::string& result) {
	if (category == SkillCategoryType::PhysicalAttack) {
		double absorption = 1 - defender.PDefense / (double)(defender.PDefense + 50);
		damage = std::floor(absorption * attacker.PAttack);
		result = attacker.name + " hit " + defender.name + " for " + std::to_string(damage) + " physics damage!";
	}
	else if (category == SkillCategoryType::MagicAttack) {
		double absorption = 1 - defender.MDefense / (double)(defender.MDefense + 50);
		damage = std::floor(absorption * attacker.MAttack);
		result = attacker.name + " hit " + defender.name + " for " + std::to_string(damage) + " magic damage!";
	}
	if (isCritical) {
		result += " (Critical)";
	}
	defender.Vitality -= damage;
}