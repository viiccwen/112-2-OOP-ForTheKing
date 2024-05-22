#include "Entity.h"
#include "Equipment.h"

std::string Weapon::weaponTypeToString() {
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

std::string Armor::armorTypeToString() {
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

std::string Accessory::accessoryTypeToString() {
	switch (Type) {
	case AccessoryType::None: return "None";
	case AccessoryType::HolyGrail: return "Holy Grail";
	case AccessoryType::Shoes: return "Shoes";
	case AccessoryType::Bracelet: return "Bracelet";
	default: return "Unknown";
	}
}

void HaveWeapon(Role& role) {
	switch (role.weapon.Type)
	{
	case WeaponType::WoodenSword:
		role.PAttack += 5;
		role.HitRate += 10;
		break;
	case WeaponType::Hammer:
		role.PAttack += 15;
		role.HitRate -= 15;
		break;
	case WeaponType::GiantHammer:
		role.PAttack += 20;
		role.HitRate -= 15;
		break;
	case WeaponType::MagicWand:
		role.MAttack += 10;
		break;
	case WeaponType::RitualSword:
		role.MAttack += 15;
		break;
	default:
		break;
	}
}

void HaveArmor(Role& role) {
	switch (role.armor.Type)
	{
	case ArmorType::WoodenShield:
		role.PDefense += 10;
		break;
	case ArmorType::PlateArmor:
		role.PDefense += 20;
		role.Speed -= 10;
		break;
	case ArmorType::LeatherArmor:
		role.PDefense += 5;
		break;
	case ArmorType::Robe:
		role.MDefense += 10;
		break;
	case ArmorType::LaurelWreath:
		role.MDefense *= 1.1;
		break;
	default:
		break;
	}
}

void HaveAccessory(Role& role) {
	switch (role.accessory.Type)
	{
	case AccessoryType::HolyGrail:
		role.MDefense += 30;
		break;
	case AccessoryType::Shoes:
		role.Speed += 5;
		break;
	case AccessoryType::Bracelet:
		role.MaxFocus++;
		break;
	default:
		break;
	}
}