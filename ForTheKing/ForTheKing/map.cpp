#include "Map.h"

#include <iostream>
#include <map>
Map::Map() {
	map.assign(140, std::vector<char>(50, '.'));
}

void Map::printMap(Role* roles, int index, std::stringstream& buffer) {
	Role& role = roles[index];
	int startX = std::max(0, role.position.x - 20);
	int endX = std::min(140, role.position.x + 20);
	int startY = std::max(0, role.position.y - 10);
	int endY = std::min(50, role.position.y + 10);

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

	std::map<std::pair<int, int>, int> rolePosition;

	for (int i = 0; i < 3; i++) {
		Role& getPositionRole = roles[i];
		rolePosition[{getPositionRole.position.x, getPositionRole.position.y}] = getPositionRole.index;
	}

	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			auto it = rolePosition.find({ x, y });
			if (it != rolePosition.end()) {
				buffer << BG_YELLOW;
				if (it->second == role.index) {
					buffer << FG_GREEN;
				}
				buffer << it->second << CLOSE;
			}
			else if (map[x][y] == WALL)
			{
				buffer << BG_GREY << WALL << CLOSE;
			}
			else if (map[x][y] == ROAD)
			{
				buffer << BG_YELLOW << FG_BLACK << ROAD << CLOSE;
			}
			else if (map[x][y] == SHOP)
			{
				buffer << BG_BLUE << SHOP << CLOSE;
			}
			else if (map[x][y] == ENEMY)
			{
				buffer << BG_RED << ENEMY << CLOSE;
			}
			else if (map[x][y] == EVENT) {
				buffer << BG_YELLOW << EVENT << CLOSE;
			}
		}
		buffer << '\n';
	}
}