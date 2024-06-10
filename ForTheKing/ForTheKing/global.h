/* Map
----------------------------------------------------------------------------------------------------------------------------
|........................................|Turn: 1                                                                           |
|........................................|Player name: Role1                                                                |
|........................................|use focus: ***                                                                    |
|........................................|Action Point:                                                                     |
|........................................|                                                                                  |
|........................................|                                                                                  |
|........................................|                                                                                  |
|........................................|                                                                                  |
|........................................|                                                                                  |
|........................................|                                                                                  |
|........................................|--------------------------------------Helper--------------------------------------|
|........................................|adjust focus: (A), (D)                                                            |
|........................................|confirm:      (Enter)                                                             |
|........................................|move:         (W), (A), (S), (D)                                                  |
|........................................|end turn:     (P)                                                                 |
|.................................$......|open bag:     (I)                                                                 |
|........................................|wall: #                                                                           |
|...................................1####|road: .                                                                           |
|....................................#.?.|shop: $                                                                           |
|....................................#...|event: E                                                                          |
|----------------------------------------|----------------------------------------|-----------------------------------------|
|Name: Role1                             |Name: Role2                             |Name: Role3                              |
|HP: 41/41                               |HP: 26/26                               |HP: 26/26                                |
|Focus: 3/3                              |Focus: 3/3                              |Focus: 3/3                               |
|Physical ATK: 11                        |Physical ATK: 24                        |Physical ATK: 24                         |
|Physical DEF: 11                        |Physical DEF: 3                         |Physical DEF: 2                          |
|Magical ATK: 14                         |Magical ATK: 0                          |Magical ATK: 0                           |
|Magical DEF: 8                          |Magical DEF: 3                          |Magical DEF: 4                           |
|Speed: 34                               |Speed: 80                               |Speed: 90                                |
|HitRate: 46                             |HitRate: 80                             |HitRate: 1                               |
|Weapon: None                            |Weapon: None                            |Weapon: None                             |
|Armor: None                             |Armor: None                             |Armor: None                              |
|Accessory: None                         |Accessory: None                         |Accessory: None                          |
|                                        |                                        |                                         |
|----------------------------------------|----------------------------------------|-----------------------------------------|
*/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <iostream>
#include <string>
#include <windows.h>

#define GAME_ALL_WIDTH 124
#define GAME_ALL_HEIGHT 36

#define ROLE_INFO_WIDTH 40
#define ROLE_INFO_HEIGHT 13

#define MAP_WIDTH 40
#define MAP_HEIGHT 20

#define ROAD '.'
#define WALL '#'
#define SHOP '$'
#define ENEMY 'E'
#define EVENT '?'

#define BG_RED static_cast<std::string>("\033[48;2;255;0;0m")
#define BG_BLUE static_cast<std::string>("\033[48;2;0;0;255m")
#define BG_YELLOW static_cast<std::string>("\033[48;2;193;156;0m")
#define BG_GREY static_cast<std::string>("\033[48;2;118;118;118m")

#define FG_RED static_cast<std::string>("\033[38;2;255;0;0m")
#define FG_YELLOW static_cast<std::string>("\033[38;2;193;156;0m")
#define FG_BLUE static_cast<std::string>("\033[38;2;0;0;255m")
#define FG_GREEN static_cast<std::string>("\033[38;2;0;255;0m")
#define FG_BLACK static_cast<std::string>("\033[38;2;0;0;0m")
#define FG_GREY static_cast<std::string>("\033[38;2;118;118;118m")

#define CLOSE static_cast<std::string>("\033[0m")

void SetConsoleSize(int width, int height);
bool checkConsoleSize(int requiredRows, int requiredCols);
void PrintString(int x, int y, std::string s);
std::string ReturnSpace(int number);
void ClearConsole();

#endif // !_GLOBAL_H_

