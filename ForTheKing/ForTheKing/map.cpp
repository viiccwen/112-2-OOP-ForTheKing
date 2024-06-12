#include "map.h"

Map::Map() {
	map.assign(140, std::vector<char>(50, '.'));
}

void Map::printMap(std::vector<Role>& roles, PositionMap& enemyPositionMap, int index, bool teleport_mode) {
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

	std::map<std::pair<int, int>, char> rolePositionMap;

	for (int i = 0; i < 3; i++) {
		Point rolePosition = roles[i].position;
		rolePositionMap[std::make_pair(rolePosition.x, rolePosition.y)] = roles[i].name[0];
	}

	// make operator on the top
	rolePositionMap[{role.position.x, role.position.y}] = role.name[0];


	// map
	int posY = 1;
	for (int y = startY; y < endY; y++) {
		std::string curStr = "";
		for (int x = startX; x < endX; x++) {
			if (teleport_mode && x == role.position.x && y == role.position.y)
			{
				curStr += BG_BLUE + FG_GREEN + TELEPORT + CLOSE;
			}
			else if (rolePositionMap.find({ x, y }) != rolePositionMap.end()) {
				curStr += BG_YELLOW;
				if (rolePositionMap[{x, y}] == role.name[0]) {
					curStr += FG_GREEN;
				}
				curStr += rolePositionMap[{x, y}] + CLOSE;
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
			else if (map[x][y] == EVENT)
			{
				curStr += BG_YELLOW + EVENT + CLOSE;
			}
			else if (map[x][y] == TENT) 
			{
				curStr += BG_BLUE + FG_GREEN + TENT + CLOSE;
			}
		}

		PrintString(1, posY, curStr);
		posY++;
	}
}

