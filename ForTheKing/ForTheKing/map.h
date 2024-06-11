#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include <sstream>
#include <map>

#include "entity.h"
#include "global.h"



struct PositionMap {
	std::map<std::pair<int, int>, int> positionMap;

	PositionMap() {}

	void addPosition(int x, int y, int index) {
		positionMap[std::make_pair(x, y)] = index;
	}
};

class Map {
public:
	std::vector<std::vector<char>> map;

	Map();
	void printMap(std::vector<Role>& roles, PositionMap& enemyPositionMap, int index);
};

#endif // !_MAP_H_