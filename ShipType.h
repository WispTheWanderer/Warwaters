#pragma once
#include "turret.h"
#include "nlohmann/json.hpp"

struct ShipComponent {
	int maxHealth = 100;
	float repairSpeed = 1;
	float hitChance = 1;
	float timeCreated = 0;
};
struct ShipType {
	ShipComponent hull;
	ShipComponent bridge;
	ShipComponent engine;
	ShipComponent steering;
	std::vector<Turret> Turrets;
	float enginePower = 10;
	float noHitChance = 1;
	float size = 5;
	std::string name = "Default Name";
	float spawnCost = 10;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShipType, hull, bridge, engine, steering, Turrets, enginePower, noHitChance, size, name, spawnCost)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShipComponent, maxHealth, repairSpeed, hitChance, timeCreated)