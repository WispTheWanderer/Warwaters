#pragma once
#include <vector>
#include "nlohmann/json.hpp"
struct AmmoType {
	int damage = 100;
	float accuracy = 1.0;
	int maxImpactCount = 1;
	int count = 1;
};
struct Turret {
	int CannonCount;
	int range;
	int reloadTime;
	std::vector<AmmoType> AmmoOptions;
	int maxHealth;
	int health = 0;
	int cooldown = 0;
	int repairSpeed;
	float hitChance;
	
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Turret, CannonCount, range, reloadTime, AmmoOptions, maxHealth, hitChance, repairSpeed)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmmoType, damage, accuracy, maxImpactCount, count)