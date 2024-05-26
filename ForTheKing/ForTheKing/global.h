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

void SetConsoleSize(int width, int height);
void PrintString(int x, int y, std::string s);
std::string ReturnSpace(int number);



#endif // !_GLOBAL_H_
