#include "map.h"

Map::Map() {
	map.assign(140, std::vector<char>(50, '.'));
}

void Map::printMap(Role* roles, PositionMap& enemyPositionMap, int index) {
	Role& role = roles[index];

	int startX = (std::max)(0, role.position.x - 20);
	int endX = (std::min)(140, role.position.x + 20);
	int startY = (std::max)(0, role.position.y - 10);
	int endY = (std::min)(50, role.position.y + 10);

	if (endX - startX < 40)
	{
		if (startX == 0)
			endX = 40;
		else
			startX = endX - 40;
	}

	if (endY - startY < 20)
	{
		if (startY == 0)
			endY = 20;
		else
			startY = endY - 20;
	}

	std::map<std::pair<int, int>, int> rolePositionMap;

	for (int i = 0; i < 3; i++) {
		Point rolePosition = roles[i].position;
		rolePositionMap[std::make_pair(rolePosition.x, rolePosition.y)] = roles[i].index;
	}

	// make operator on the top
	rolePositionMap[{role.position.x, role.position.y}] = role.index;

	// upper, lower frame
	for (int i = 0; i < 100; i++) PrintString(i, 0, "-");
	for (int i = 0; i < 100; i++) PrintString(i, 21, "-");

	// left, middle, right frame
	for (int i = 1; i <= 20; i++) PrintString(0, i, "|");
	for (int i = 1; i <= 20; i++) PrintString(41, i, "|");
	for (int i = 1; i <= 20; i++) PrintString(99, i, "|");

	// player status
	PrintString(42, 1, "Turn: ");
	PrintString(42, 2, "Player name: ");
	PrintString(42, 3, "Action Point: ");

	// Helper
	std::string wall = "wall: " + BG_GREY + WALL + CLOSE;
	std::string road = "road: " + BG_YELLOW + FG_BLACK + ROAD + CLOSE;
	std::string shop = "shop: " + BG_BLUE + SHOP + CLOSE;
	std::string _event = "event: " + BG_RED + ENEMY + CLOSE;
	std::vector<std::string> helper = {
	"--------------------------Helper-------------------------",
	"adjust focus: (A), (D)",
	"confirm:      (Enter)",
	"move:         (W), (A), (S), (D)",
	"end turn:     (P)",
	"open bag:     (I)",
	wall,
	road,
	shop,
	_event
	};
	for (int y = 11, idx = 0; y - 11 < helper.size(); y++, idx++) PrintString(42, y, helper[idx]);
	
	// map
	int posY = 1;
	for (int y = startY; y < endY; y++) {
		std::string curStr = "";
		for (int x = startX; x < endX; x++) {
			if (rolePositionMap.find({ x, y }) != rolePositionMap.end()) {
				curStr += BG_YELLOW;
				if (rolePositionMap[{x, y}] == role.index) {
					curStr += FG_GREEN;
				}
				curStr += std::to_string(rolePositionMap[{x, y}]) + CLOSE;
			}
			else if (enemyPositionMap.positionMap.find({ x, y }) != enemyPositionMap.positionMap.end()) {
				curStr += BG_RED + ENEMY + CLOSE;
			}
			else if (map[x][y] == WALL)
			{
				curStr += BG_GREY + WALL + CLOSE;
			}
			else if (map[x][y] == ROAD)
			{
				curStr += BG_YELLOW + FG_BLACK + ROAD + CLOSE;
			}
			else if (map[x][y] == SHOP)
			{
				curStr += BG_BLUE + SHOP + CLOSE;
			}
			else if (map[x][y] == EVENT) {
				curStr += BG_YELLOW + EVENT + CLOSE;
			}
		}

		PrintString(1, posY, curStr);
		posY++;
	}
}

