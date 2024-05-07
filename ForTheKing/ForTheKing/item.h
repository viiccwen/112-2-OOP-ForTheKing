#ifndef _ITEM_H_
#define _ITEM_H_

enum class ItemType {
	Godsbeard,
	GoldenRoot,
	TeleportScroll,
	Tent
};

class Item {
public:
	ItemType Type;
};

#endif // !_ITEM_H_