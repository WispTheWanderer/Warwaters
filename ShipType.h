#pragma once
#include "turret.h"
#include "nlohmann/json.hpp"

struct ShipComponent {
	int maxHealth;
	float repairSpeed;
	float hitChance;
	ShipComponent() = default;
	ShipComponent(int maxHealth, float repairSpeed, float hitChance) : maxHealth(maxHealth), repairSpeed(repairSpeed), hitChance(hitChance) {};
};
struct ShipType {
	ShipComponent hull;
	ShipComponent bridge;
	ShipComponent engine;
	ShipComponent steering;
	std::vector<Turret> Turrets;
	float enginePower;
	float noHitChance;
	float size;
	std::string name;
	float spawnCost;
	ShipType() = default;
	ShipType(ShipComponent hull, ShipComponent bridge, ShipComponent engine, ShipComponent steering, std::vector<Turret> Turrets, float enginePower, float noHitChance, float size, std::string name, float spawnCost) : hull(hull), engine(engine), bridge(bridge), steering(steering), Turrets(Turrets), enginePower(enginePower), noHitChance(noHitChance), size(size), name(name), spawnCost(spawnCost) {};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShipType, hull, bridge, engine, steering, Turrets, enginePower, noHitChance, size, name, spawnCost)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShipComponent, maxHealth, repairSpeed, hitChance)