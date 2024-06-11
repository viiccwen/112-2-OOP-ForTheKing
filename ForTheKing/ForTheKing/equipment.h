#ifndef _EQUIPMENT_H_
#define _EQUIPMENT_H_

#include <string>
#include <memory>

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

enum class ElseType {
	TeleportScroll,
	Godsbeard,
	GoldenRoot,
	Tent
};

class Equipment {
public:
	virtual ~Equipment() {}
	virtual std::string EquipmentTypeToString() const = 0;
	bool isNone() const;
};

class Weapon : public Equipment {
private:
public:
	WeaponType Type;
	Weapon() : Type(static_cast<WeaponType>(0)) {}
	Weapon(WeaponType t) : Type(t) {}
	std::string EquipmentTypeToString() const override;
};

class Armor : public Equipment {
private:
public:
	ArmorType Type;
	Armor() : Type(static_cast<ArmorType>(0)) {}
	Armor(ArmorType t) : Type(t) {}
	std::string EquipmentTypeToString() const override;
};

class Accessory : public Equipment {
private:
public:
	AccessoryType Type;
	Accessory() : Type(static_cast<AccessoryType>(0)) {}
	Accessory(AccessoryType t) : Type(t) {}
	std::string EquipmentTypeToString() const override;
};

class Else : public Equipment {
private:
public:
	ElseType Type;
	Else(ElseType t) : Type(t) {}
	std::string EquipmentTypeToString() const override;
};

#endif // !_EQUIPMENT_H_