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
	float velocityX = 0;
	float velocityY = 0;
	float positionX = 0;
	float positionY = 0;
	float heading = 0;
	float speed = 0;
	float mass = 1;
	int bridgeHealth = 0;
	int engineHealth = 0;
	int steeringHealth = 0;
	int hullIntegrity = 0;
	float flooding = 0;
	RandomNumberGenerator generate;
	std::string name = "";
	ShipType type;
	std::vector<Turret> Guns;
	std::vector<FireParams> fireOrders;
	std::vector<std::unique_ptr<Warship>>* OtherShips;
	int indexPosition = 0;
	bool isEnemy = false;
	bool HasActioned = false;
	bool isDead = false;
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
