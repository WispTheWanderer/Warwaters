#pragma once
#include "ShipType.h"
#include "PercentageGenerator.h"
#include <iostream>
#include <string>
struct FireParams
{
	float targetX;
	float targetY;
	int gun;
	unsigned char ammoType;
	bool IsContinuous;
	FireParams(float targetX, float targetY, int gun, unsigned char ammoType, bool IsContinuous) : targetX(targetX), targetY(targetY), gun(gun), ammoType(ammoType), IsContinuous(IsContinuous) {};
};
class Warship {
private:
	float velocityX;
	float velocityY;
	float positionX;
	float positionY;
	float heading;
	float speed;
	float mass;
	int bridgeHealth;
	int engineHealth;
	int steeringHealth;
	int hullIntegrity;
	float flooding;
	PercentageGenerator generate;
	std::string name;
	ShipType type;
	std::vector<Turret> Guns;
	std::vector<FireParams> fireOrders;
	std::vector<std::unique_ptr<Warship>>* OtherShips;
	int indexPosition;
	bool isEnemy;
	bool HasActioned;
	bool isDead;
public:
	Warship(std::string name, float positionX, float positionY, ShipType type, std::vector<std::unique_ptr<Warship>>* World, bool isEnemy = false);
	void ChangeHeading(float angle, float power);
	void Fire(int gun, unsigned char ammoType, float targetX, float targetY);
	void Teleport(float posX, float posY);
	void Damage(int amount);
	void Tick();
	void Log(const std::string& message);
	void QueueFire(int gun, unsigned char ammoType, float targetX, float targetY, bool IsContinuous);
	float getFloodLevel() const { return flooding; };
	float getPositionX() const { return positionX; };
	float getPositionY() const { return positionY; };
	float getHeading() const { return heading; };
	float getSpeed() const { return speed; };
	float getSize() const { return type.size; };
	bool getIfEnemy() const { return isEnemy; };
	bool getIfActioned() const { return HasActioned; };
	bool getIfDead() const { return isDead; };
	std::string getName() const { return name; };
};
