#include "Entity.h"
#include "Equipment.h"

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