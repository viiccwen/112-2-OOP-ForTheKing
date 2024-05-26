#include "Entity.h"
#include "map.h"
#include "global.h"

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

	actSkills = { ActiveSkills(ActiveSkillType::Attack)};
}

void Role::MarkCurrentRole() {
	
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + 1, "-");
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + ROLE_INFO_HEIGHT + 2, "-");
	
	for (int roleGapIdx = 0; roleGapIdx < 4; roleGapIdx++) {
		for (int i = 21; i <= 35; i++) {
			PrintString(ROLE_INFO_WIDTH * roleGapIdx + roleGapIdx, i, "|");
		}
	}
	
	for (int i = ROLE_INFO_WIDTH * (index - 1) + index; i <= ROLE_INFO_WIDTH * index + index; i++) PrintString(i, MAP_HEIGHT + 1, FG_GREEN + "-" + CLOSE);
	for (int i = ROLE_INFO_WIDTH * (index - 1) + index; i <= ROLE_INFO_WIDTH * index + index; i++) PrintString(i, MAP_HEIGHT + ROLE_INFO_HEIGHT + 2, FG_GREEN + "-" + CLOSE);

	for (int i = MAP_HEIGHT + 1; i < GAME_ALL_HEIGHT; i++) PrintString(ROLE_INFO_WIDTH * (index - 1) + (index - 1), i, FG_GREEN + "|" + CLOSE);
	for (int i = MAP_HEIGHT + 1; i < GAME_ALL_HEIGHT; i++) PrintString(ROLE_INFO_WIDTH * index + index, i, FG_GREEN + "|" + CLOSE);
}

void PrintRoleInfo(std::vector<Role> roles) {
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
			PrintString(1 + roleGap, 22 + attrIdx, " " + attributes[attrIdx] + ": " + roles[roleIdx].getAttribute(attrIdx));
		}
	}
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

	actSkills = {};
}