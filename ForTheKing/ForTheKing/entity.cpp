#include "entity.h"
#include "global.h"

// todo: change it to 600
int Role::Money = 600;
std::vector<std::shared_ptr<Equipment>> Role::Bag = {};

static int randomBetween(int min, int max, bool includeMax = false) {
	return rand() % (max - min + includeMax) + min;
}

Entity::Entity() {
	index = 0;
};

void Entity::addSkill(ActiveSkills skill) {
	if (std::find(actSkills.begin(), actSkills.end(), skill) == actSkills.end())
		actSkills.push_back(skill);
}

void Entity::removeSkill(ActiveSkills skill) {
	if (std::find(actSkills.begin(), actSkills.end(), skill) != actSkills.end())
		actSkills.erase(std::remove(actSkills.begin(), actSkills.end(), skill), actSkills.end());
}

void Entity::addBuff(Buffs buff) {
	if (std::find(buffs.begin(), buffs.end(), buff) == buffs.end())
		buffs.push_back(buff);
	else {
		auto it = std::find(buffs.begin(), buffs.end(), buff);
		if (buffs[it - buffs.begin()].effectDuration < buff.effectDuration) {
			buffs[it - buffs.begin()].disable(buffs[it - buffs.begin()], *this);
			buffs.erase(it);
			buffs.push_back(buff);
		}
	}
}

void Entity::removeBuff(Buffs buff) {
	if (std::find(buffs.begin(), buffs.end(), buff) != buffs.end())
		buffs.erase(std::remove(buffs.begin(), buffs.end(), buff), buffs.end());
}

Role::Role() {
	index = 0;
};

Role::Role(int _index, std::string _name) {
	index = _index;
	name = _name;

	// **testing**
	/*
	// [30, 45)
	Vitality = MaxVitality = randomBetween(30, 100);

	// 3
	Focus = MaxFocus = 3;

	// [30, 55)
	Speed = MaxSpeed = randomBetween(99, 100);

	//[40, 60)
	HitRate = MaxHitRate = randomBetween(99, 100);

	// [5, 15]
	PAttack = MaxPAttack = randomBetween(5, 15, true);
	MAttack = MaxMAttack = randomBetween(5, 15, true);

	// [0, 20]
	PDefense = MaxPDefense = randomBetween(0, 20, true);
	MDefense = MaxMDefense = randomBetween(0, 20, true);
	*/
	// **real**

	
	//  int x = rand() % (max - min + 1) + min;
	// [30, 45)
	Vitality = MaxVitality = randomBetween(30, 45);

	// 3
	Focus = MaxFocus = 3;

	// [30, 55)
	Speed = MaxSpeed = randomBetween(30, 55);

	//[40, 60)
	HitRate = MaxHitRate = randomBetween(40, 60);

	// [5, 15]
	PAttack = MaxPAttack = randomBetween(5, 15, true);
	MAttack = MaxMAttack = randomBetween(5, 15, true);

	// [0, 20]
	PDefense = MaxPDefense = randomBetween(0, 20, true);
	MDefense = MaxMDefense = randomBetween(0, 20, true);
	
	weapon = std::make_shared<Weapon>(WeaponType::None);
	armor = std::make_shared<Armor>(ArmorType::None);
	accessory = std::make_shared<Accessory>(AccessoryType::None);

	// TODO
	// actSkills = { ActiveSkills(ActiveSkillType::Attack) };
	actSkills = { ActiveSkillType::Attack, ActiveSkillType::Flee };

}

Enemy::Enemy() {
	index = 0;
};

Enemy::Enemy(int _index, std::string _name) {
	index = _index;
	name = _name;

	// [30, 45)
	Vitality = MaxVitality = randomBetween(30, 45);

	// 3
	Focus = MaxFocus = 3;

	// [30, 55)
	Speed = MaxSpeed = randomBetween(30, 55);

	//[40, 60)
	HitRate = MaxHitRate = randomBetween(40, 60);

	// [5, 15]
	PAttack = MaxPAttack = randomBetween(5, 15, true);
	MAttack = MaxMAttack = randomBetween(5, 15, true);

	// [0, 20]
	PDefense = MaxPDefense = randomBetween(0, 20, true);
	MDefense = MaxMDefense = randomBetween(0, 20, true);


	weapon = std::make_shared<Weapon>(WeaponType::None);
	armor = std::make_shared<Armor>(ArmorType::None);
	accessory = std::make_shared<Accessory>(AccessoryType::None);
	// todo: give enemies equipment
	/*
	weapon->Type = static_cast<WeaponType>(rand() % (5 - 1 + 1) + 1);
	armor->Type = static_cast<ArmorType>(rand() % (5 - 1 + 1) + 1);
	accessory->Type = static_cast<AccessoryType>(rand() % (3 - 1 + 1) + 1);
	*/
	actSkills = { ActiveSkillType::Attack };
}

std::string Entity::getAttribute(int attributeIndex) {
	switch (attributeIndex) {
	case 0: return name;
	case 1: return std::to_string(Vitality) + "/" + std::to_string(MaxVitality);
	case 2: return std::to_string(Focus) + "/" + std::to_string(MaxFocus);
	case 3: return std::to_string(PAttack);
	case 4: return std::to_string(PDefense);
	case 5: return std::to_string(MAttack);
	case 6: return std::to_string(MDefense);
	case 7: return std::to_string(Speed);
	case 8: return std::to_string(HitRate);
	case 9: return weapon->EquipmentTypeToString();
	case 10: return armor->EquipmentTypeToString();
	case 11: return accessory->EquipmentTypeToString();
	default: return "";
	}
}

void PrintRoleInfo(std::vector<Role>& roles) {
	std::vector<std::string> attributes = {
		"Name",
		"HP",
		"Focus",
		"Physical ATK",
		"Physical DEF",
		"Magical ATK",
		"Magical DEF",
		"Speed",
		"HitRate",
		"Weapon",
		"Armor",
		"Accessory"
	};

	for (int roleIdx = 0; roleIdx < roles.size(); roleIdx++) {
		int roleGap = 41 * roleIdx;
		for (int attrIdx = 0; attrIdx < attributes.size(); attrIdx++) {
			PrintString(1 + roleGap, 22 + attrIdx, " " + attributes[attrIdx] + ": " + roles[roleIdx].getAttribute(attrIdx) + ReturnSpace(10));
		}
	}
}

void applyEquipmentStats(Role& role, std::shared_ptr<Weapon> weapon, bool apply) {
	int factor = apply ? 1 : -1;
	auto applySkill = [apply](Role& role, ActiveSkills skill) {
		if (apply) {
			role.addSkill(skill);
		}
		else {
			role.removeSkill(skill);
		}
	};
	auto applyBuff = [apply](Role& role, BuffType buff) {
		if (apply) {
			role.addBuff(Buffs(buff, 99999));
		}
		else {
			role.removeBuff(Buffs(buff, 99999));
		}
	};
	switch (weapon->Type) {
	case WeaponType::WoodenSword:
		role.PAttack += 5 * factor;
		role.HitRate += 10 * factor;
		applySkill(role, ActiveSkillType::SpeedUp);
		break;
	case WeaponType::Hammer:
		role.PAttack += 15 * factor;
		role.HitRate -= 15 * factor;
		applyBuff(role, BuffType::Hammer_Splash);
		applySkill(role, ActiveSkillType::Provoke);
		break;
	case WeaponType::GiantHammer:
		role.PAttack += 20 * factor;
		role.HitRate -= 15 * factor;
		applyBuff(role, BuffType::Hammer_Splash);
		break;
	case WeaponType::MagicWand:
		role.MAttack += 10 * factor;
		applySkill(role, ActiveSkillType::Shock_Blast);
		applySkill(role, ActiveSkillType::Heal);
		applySkill(role, ActiveSkillType::Poison);
		break;
	case WeaponType::RitualSword:
		role.MAttack += 15 * factor;
		applySkill(role, ActiveSkillType::Shock_Blast);
		break;
	default:
		break;
	}
}

void applyEquipmentStats(Role& role, std::shared_ptr<Armor> armor, bool apply) {
	int factor = apply ? 1 : -1;
	auto applyBuff = [apply](Role& role, BuffType buff) {
		if (apply) {
			role.addBuff(Buffs(buff, 99999));
		}
		else {
			role.removeBuff(Buffs(buff, 99999));
		}
	};
	switch (armor->Type) {
	case ArmorType::WoodenShield:
		role.PDefense += 10 * factor;
		role.addSkill(ActiveSkillType::Provoke);
		break;
	case ArmorType::PlateArmor:
		role.PDefense += 20 * factor;
		role.Speed -= 10 * factor;
		applyBuff(role, BuffType::Fortify);
		break;
	case ArmorType::LeatherArmor:
		role.PDefense += 5 * factor;
		applyBuff(role, BuffType::Fortify);
		break;
	case ArmorType::Robe:
		role.MDefense += 10 * factor;
		break;
	case ArmorType::LaurelWreath:
		if (apply) role.MDefense *= 1.1;
		else role.MDefense /= 1.1;  // maybe need more specific way to handle
		break;
	default:
		break;
	}
}

void applyEquipmentStats(Role& role, std::shared_ptr<Accessory> accessory, bool apply) {
	int factor = apply ? 1 : -1;
	auto applySkill = [apply](Role& role, ActiveSkills skill) {
		if (apply) {
			role.addSkill(skill);
		}
		else {
			role.removeSkill(skill);
		}
	};
	auto applyBuff = [apply](Role& role, BuffType buff) {
		if (apply) {
			role.addBuff(Buffs(buff, 99999));
		}
		else {
			role.removeBuff(Buffs(buff, 99999));
		}
	};
	switch (accessory->Type) {
	case AccessoryType::HolyGrail:
		role.MDefense += 30 * factor;
		applySkill(role, ActiveSkillType::Heal);
		break;
	case AccessoryType::Shoes:
		role.Speed += 5 * factor;
		applyBuff(role, BuffType::Run);
		break;
	case AccessoryType::Bracelet:
		role.MaxFocus += 1 * factor;
		break;
	default:
		break;
	}
}