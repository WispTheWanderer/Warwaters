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
	int CannonCount = 1;
	int range = 100;
	int reloadTime = 1;
	std::vector<AmmoType> AmmoOptions;
	int maxHealth = 100;
	int health = 0;
	int cooldown = 0;
	int repairSpeed = 1;
	float hitChance = 1;
	float timeCreated = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Turret, CannonCount, range, reloadTime, AmmoOptions, maxHealth, hitChance, repairSpeed, timeCreated)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmmoType, damage, accuracy, maxImpactCount, count)