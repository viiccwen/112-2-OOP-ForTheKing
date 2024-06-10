#ifndef _SHOP_H_
#define _SHOP_H_

#include "equipment.h"
#include <vector>
#include <memory>

class Shop {
public:
	std::vector<std::shared_ptr<Equipment>> item_lists;
	std::vector<int> item_value;

	Shop();
	void ShowShopLists(int select_index, int money);
};

#endif // !_SHOP_H_
