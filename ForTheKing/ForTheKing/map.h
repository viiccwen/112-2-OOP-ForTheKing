#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include <sstream>

#include "entity.h"

#define BG_YELLOW "\033[48;2;193;156;0m"
#define BG_GREY "\033[48;2;118;118;118m"
#define FG_BLACK "\033[38;2;0;0;0m"
#define CLOSE "\033[0m"

class Map {
public:
	Map();
	std::vector<std::vector<char>> map;
	void printMap(Role* roles, int index, std::stringstream& buffer);
};

#endif // !_MAP_H_