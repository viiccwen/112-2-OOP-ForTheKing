#include "shop.h"
#include "global.h"

Shop::Shop() {
	// item
	item_lists.push_back(std::make_shared<Else>(ElseType::Tent));
	item_lists.push_back(std::make_shared<Else>(ElseType::Godsbeard));
	item_lists.push_back(std::make_shared<Else>(ElseType::GoldenRoot));
	item_lists.push_back(std::make_shared<Else>(ElseType::TeleportScroll));

	item_value.push_back(50);
	item_value.push_back(100);
	item_value.push_back(150);
	item_value.push_back(2000);

	// weapon
	item_lists.push_back(std::make_shared <Weapon>(WeaponType::WoodenSword));
	item_lists.push_back(std::make_shared <Weapon>(WeaponType::Hammer));
	item_lists.push_back(std::make_shared <Weapon>(WeaponType::MagicWand));

	item_value.push_back(100);
	item_value.push_back(50);
	item_value.push_back(30);

	// armor
	item_lists.push_back(std::make_shared <Armor>(ArmorType::PlateArmor));
	item_lists.push_back(std::make_shared <Armor>(ArmorType::WoodenShield));
	item_value.push_back(100);
	item_value.push_back(120);

	// accessory
	item_lists.push_back(std::make_shared <Accessory>(AccessoryType::HolyGrail));
	item_lists.push_back(std::make_shared<Accessory>(AccessoryType::Bracelet));
	item_value.push_back(30);
	item_value.push_back(70);
}

void Shop::ShowShopLists(int select_index, int money) {
	for (int y = 1; y < 1 + item_lists.size(); y++) {
		auto& item = item_lists[y - 1];

		std::string cur_item_str = "";
		if (select_index == y - 1) cur_item_str += FG_YELLOW;

		// turn type-name to string
		cur_item_str += item->EquipmentTypeToString();

		cur_item_str += "\t\t\t$" + std::to_string(item_value[y - 1]) + CLOSE;
		PrintString(1, y, cur_item_str);
	}
	PrintString(1, 23, ReturnSpace(15));
	PrintString(1, 23, "Money: " + std::to_string(money));
}