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

	skills = {};
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

	skills = {};
}