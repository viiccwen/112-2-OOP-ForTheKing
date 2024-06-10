#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>
#include <string>
#include <memory>
#include "equipment.h"
#include "skill.h"

typedef struct {
	int x;
	int y;
}Point;

class Entity {
public:
	int index;

	int Vitality;
	int Focus;
	int Speed;
	int HitRate;
	int PAttack;
	int MAttack;
	int PDefense;
	int MDefense;

	int MaxVitality;
	int MaxFocus;
	int MaxSpeed;
	int MaxHitRate;
	int MaxPAttack;
	int MaxMAttack;
	int MaxPDefense;
	int MaxMDefense;

	std::string name;

	std::vector<ActiveSkills> actSkills;
	Weapon weapon;
	Armor armor;
	Accessory accessory;

	Point position;
	Entity();
	std::string getAttribute(int attributeIndex);
};

class Role : public Entity {
public:
	Role();
	Role(int _index, std::string _name);
	static int Money;
	static std::vector<std::shared_ptr<Equipment>> Bag;

	friend void PrintRoleInfo(std::vector<Role>& roles);
};

class Enemy : public Entity {
public:
	Enemy();
	Enemy(int _index, std::string _name);
};

#endif // !_ENTITY_H_
