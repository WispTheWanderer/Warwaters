#pragma once
#include <vector>
#include "nlohmann/json.hpp"
struct AmmoType {
	int damage;
	float accuracy;
	int maxImpactCount;
	int count;
	AmmoType() = default;
	AmmoType(int damage, float accuracy, int maxImpactCount, int count) : damage(damage), accuracy(accuracy), maxImpactCount(maxImpactCount), count(count) {};
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
	Turret() = default;
	Turret(int CannonCount, int range, int reloadTime, std::vector<AmmoType> AmmoOptions, int maxHealth, int hitChance, int repairSpeed) : CannonCount(CannonCount), range(range), reloadTime(reloadTime), AmmoOptions(AmmoOptions), maxHealth(maxHealth), hitChance(hitChance), repairSpeed(repairSpeed) { health = maxHealth; };
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Turret, CannonCount, range, reloadTime, AmmoOptions, maxHealth, hitChance, repairSpeed)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AmmoType, damage, accuracy, maxImpactCount, count)