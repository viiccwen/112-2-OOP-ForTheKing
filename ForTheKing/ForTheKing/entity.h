#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>
#include <string>
#include "equipment.h"
#include "skill.h"
#include "item.h"

class Entity {
private:

public:
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

	std::vector<Skills> skills;
	Weapon weapon;
	Armor armor;
	Accessory accessory;
};

class Role : public Entity {
public:
	Role(std::string _name);
	static int Money;
	static std::vector<Equipment> Bag;
};

class Enemy : public Entity {
public:
	Enemy(std::string _name);
};

#endif // !_ENTITY_H_
