#include "Equipment.h"
#include "Entity.h"

std::string Weapon::EquipmentTypeToString() const {
	switch (Type) {
	case WeaponType::None: return "None";
	case WeaponType::WoodenSword: return "Wooden Sword";
	case WeaponType::Hammer: return "Hammer";
	case WeaponType::GiantHammer: return "Giant Hammer";
	case WeaponType::MagicWand: return "Magic Wand";
	case WeaponType::RitualSword: return "Ritual Sword";
	default: return "Unknown";
	}
}

std::string Armor::EquipmentTypeToString() const {
	switch (Type) {
	case ArmorType::None: return "None";
	case ArmorType::WoodenShield: return "Wooden Shield";
	case ArmorType::PlateArmor: return "Plate Armor";
	case ArmorType::LeatherArmor: return "Leather Armor";
	case ArmorType::Robe: return "Robe";
	case ArmorType::LaurelWreath: return "Laurel Wreath";
	default: return "Unknown";
	}
}

std::string Accessory::EquipmentTypeToString() const {
	switch (Type) {
	case AccessoryType::None: return "None";
	case AccessoryType::HolyGrail: return "Holy Grail";
	case AccessoryType::Shoes: return "Shoes";
	case AccessoryType::Bracelet: return "Bracelet";
	default: return "Unknown";
	}
}

std::string Else::EquipmentTypeToString() const {
	switch (Type) {
	case ElseType::Godsbeard: return "Godsbeard";
	case ElseType::GoldenRoot: return "GoldenRoot";
	case ElseType::TeleportScroll: return "TeleportScroll";
	case ElseType::Tent: return "Tent";
	default: return "Unknown";
	}
}

bool Equipment::isNone() const {
	return EquipmentTypeToString() == "None";
}