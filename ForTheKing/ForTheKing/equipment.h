#ifndef _EQUIPMENT_H_
#define _EQUIPMENT_H_

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
};

class Armor : public Equipment {
public:
	ArmorType Type;
};

class Accessory : public Equipment {
public:
	AccessoryType Type;
};

#endif // !_EQUIPMENT_H_