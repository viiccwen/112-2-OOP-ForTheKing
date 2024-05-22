#ifndef _EQUIPMENT_H_
#define _EQUIPMENT_H_
#include <string>

enum class WeaponType {
	None,
	WoodenSword,
	Hammer,
	GiantHammer,
	MagicWand,
	RitualSword
};

enum class ArmorType {
	None,
	WoodenShield,
	PlateArmor,
	LeatherArmor,
	Robe,
	LaurelWreath
};

enum class AccessoryType {
	None,
	HolyGrail,
	Shoes,
	Bracelet
};

class Equipment {

};

class Weapon : public Equipment {
public:
	WeaponType Type;
	std::string weaponTypeToString();
};

class Armor : public Equipment {
public:
	ArmorType Type;
	std::string armorTypeToString();
};

class Accessory : public Equipment {
public:
	AccessoryType Type;
	std::string accessoryTypeToString();
};

#endif // !_EQUIPMENT_H_