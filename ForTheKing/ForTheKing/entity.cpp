#include "entity.h"
#include "global.h"

int Role::Money = 600;
std::vector<std::shared_ptr<Equipment>> Role::Bag = {};

static int randomBetween(int min, int max, bool includeMax = false) {
	return rand() % (max - min + includeMax) + min;
}

Entity::Entity() {
	index = 0;
};

Role::Role() {
	index = 0;
};

Role::Role(int _index, std::string _name) {
	index = _index;
	name = _name;

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


	weapon = std::make_shared<Weapon>(WeaponType::None);
	armor = std::make_shared<Armor>(ArmorType::None);
	accessory = std::make_shared<Accessory>(AccessoryType::None);
	// todo: give enemies equipment
	/*
	weapon->Type = static_cast<WeaponType>(rand() % (5 - 1 + 1) + 1);
	armor->Type = static_cast<ArmorType>(rand() % (5 - 1 + 1) + 1);
	accessory->Type = static_cast<AccessoryType>(rand() % (3 - 1 + 1) + 1);
	*/
	actSkills = {};
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