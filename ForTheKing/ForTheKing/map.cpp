#include "Map.h"

#include <iostream>

Map::Map() {
	map.assign(140, std::vector<char>(50, '.'));
}

void Map::printMap(Role& role, std::stringstream& buffer) {
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
	for (int i = startY; i < endY; i++)
	{
		for (int j = startX; j < endX; j++)
		{
			if (j == role.position.x && i == role.position.y)
			{
				buffer << BG_YELLOW << role.index << CLOSE;
			}
			else if (map[j][i] == '#')
			{
				buffer << BG_GREY << ' ' << CLOSE;
			}
			else if (map[j][i] == '.')
			{
				buffer << BG_YELLOW << FG_BLACK << '.' << CLOSE;
			}
		}
		buffer << '\n';
	}
}