#include "Entity.h"
#include "map.h"

int Role::Money = 600;
std::vector<std::string> Bag = {};

static int randomBetween(int min, int max, bool includeMax = false) {
	return rand() % (max - min + includeMax) + min;
}

Entity::Entity() {
	index = 0;
};

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
	case 9: return weapon.weaponTypeToString();
	case 10: return armor.armorTypeToString();
	case 11: return accessory.accessoryTypeToString();
	default: return "";
	}
}

void Entity::addSkill(ActiveSkills skill) {
	if(std::find(actSkills.begin(), actSkills.end(), skill) == actSkills.end())
		actSkills.push_back(skill);
}

void Entity::removeSkill(ActiveSkills skill) {
	if(std::find(actSkills.begin(), actSkills.end(), skill) != actSkills.end())
		actSkills.erase(std::remove(actSkills.begin(), actSkills.end(), skill), actSkills.end());
}

void Entity::addBuff(Buffs buff) {
	if (std::find(buffs.begin(), buffs.end(), buff) == buffs.end())
		buffs.push_back(buff);
	else {
		auto it = std::find(buffs.begin(), buffs.end(), buff);
		if(buffs[it - buffs.begin()].effectDuration < buff.effectDuration){
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

	//  int x = rand() % (max - min + 1) + min;
	// [30, 45)
	Vitality = MaxVitality = rand() % (45 - 30) + 30;

	// 3
	Focus = MaxFocus = 3;

	// [30, 55)
	Speed = MaxSpeed = rand() % (55 - 30) + 30;

	//[40, 60)
	HitRate = MaxHitRate = rand() % (60 - 40) + 40;

	// [5, 15]
	PAttack = MaxPAttack = rand() % (15 - 5 + 1) + 5;
	MAttack = MaxMAttack = rand() % (15 - 5 + 1) + 5;

	// [0, 20]
	PDefense = MaxPDefense = rand() % 20 + 1;
	MDefense = MaxMDefense = rand() % 20 + 1;

	weapon.Type = WeaponType::None;
	armor.Type = ArmorType::None;
	accessory.Type = AccessoryType::None;

	actSkills = { ActiveSkillType::Attack,ActiveSkillType::Flee};
	buffs = {};
}

Enemy::Enemy() {
	index = 0;
};

Enemy::Enemy(int _index, std::string _name) {
	index = _index;
	name = _name;

	//  int x = rand() % (max - min + 1) + min;
	// [30, 45)
	Vitality = MaxVitality = rand() % (45 - 30) + 30;

	// 3
	Focus = MaxFocus = 3;

	// [30, 55)
	Speed = MaxSpeed = rand() % (55 - 30) + 30;

	//[40, 60)
	HitRate = MaxHitRate = rand() % (60 - 40) + 40;

	// [5, 15]
	PAttack = MaxPAttack = rand() % (15 - 5 + 1) + 5;
	MAttack = MaxMAttack = rand() % (15 - 5 + 1) + 5;

	// [0, 20]
	PDefense = MaxPDefense = rand() % 20 + 1;
	MDefense = MaxMDefense = rand() % 20 + 1;

	weapon.Type = static_cast<WeaponType>(rand() % (5 - 1 + 1) + 1);
	armor.Type = static_cast<ArmorType>(rand() % (5 - 1 + 1) + 1);
	accessory.Type = static_cast<AccessoryType>(rand() % (3 - 1 + 1) + 1);

	actSkills = { ActiveSkillType::Attack};
	buffs = {};
}