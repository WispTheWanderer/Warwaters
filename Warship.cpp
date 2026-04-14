#include "Warship.h"
#include <algorithm>
Warship::Warship(std::string name, float posX, float posY, ShipType type, std::vector<std::unique_ptr<Warship>>* World) : name(name), positionX(posX), positionY(posY), type(type), OtherShips(World)
{
	Guns = type.Turrets;
	for (int index = 0; index < Guns.size(); ++index)
	{
		Guns[index].health = Guns[index].maxHealth;
	}
	bridgeHealth = type.bridge.maxHealth;
	engineHealth = type.engine.maxHealth;
	steeringHealth = type.steering.maxHealth;
	hullIntegrity = type.hull.maxHealth;
	std::unique_ptr<Warship> selfPtr(this);
	indexPosition = OtherShips->size();
	OtherShips->push_back(std::move(selfPtr));
	std::cout << name << " has appeared on the horizon!\n";
}

void Warship::ChangeHeading(float angle, float power)
{
	heading = angle;
	power = std::max(power, 100.0f);
	power /= 100;
	speed = power * type.enginePower * (engineHealth / type.engine.maxHealth);
	velocityX = cos(angle) * speed;
	velocityY = sin(angle) * speed;

}

void Warship::Fire(int gun, unsigned char ammoType, float targetX, float targetY)
{
	if (gun < Guns.size()) 
	{
		Turret selectedGun = Guns[gun];
		if (selectedGun.health > 0) {
			if (selectedGun.AmmoOptions[ammoType].count >= selectedGun.CannonCount) {
				float distance = sqrt(pow((positionX - targetX), 2) + pow((positionY - targetY), 2));
				distance -= selectedGun.range;
				float actualAccuracy;
				if (distance > 0)
				{
					actualAccuracy = (1 / abs(distance)) * selectedGun.AmmoOptions[ammoType].accuracy;
				}
				else
				{
					actualAccuracy = selectedGun.AmmoOptions[ammoType].accuracy;
				}
				if (generate.getPercentage(1.0) <= actualAccuracy) {
					for (int index = 0; index < OtherShips->size(); ++index) {
						Warship& checkerTarget = *((*OtherShips)[index]);
						if (sqrt(pow((targetX - checkerTarget.getPositionX()), 2) + pow((targetY - checkerTarget.getPositionY()), 2)) <= checkerTarget.type.size) {
							int fireTime = 0;
							for (int index = 0; index < selectedGun.CannonCount; ++index) {
								fireTime += generate.getInt(selectedGun.AmmoOptions[ammoType].maxImpactCount);
							}
							for (int impactCount = 0; impactCount < fireTime; ++impactCount) {
								checkerTarget.Damage(selectedGun.AmmoOptions[ammoType].damage);
							}
							Log("Hit " + checkerTarget.name + "!");
						}
					}
				}
				else {
					Log("Missed!");
				}
				selectedGun.AmmoOptions[ammoType].count -= selectedGun.CannonCount;
				selectedGun.cooldown = selectedGun.reloadTime;
			}
			else Log("There is not enough of that ammunition type!");
		}
		else Log("That turret is destroyed!");
	}
	else Log("That turret does not exist!");
}

void Warship::Teleport(float posX, float posY)
{
	positionX = posX;
	positionY = posY;
}

void Warship::Damage(int amount)
{
	float totalHitChance = type.bridge.hitChance + type.engine.hitChance + type.steering.hitChance + type.noHitChance;
	for (Turret item : Guns) {
		totalHitChance += item.hitChance;
	}
	float hitChance = generate.getPercentage(totalHitChance);
	float currentHitChance = type.bridge.hitChance;
	if (hitChance < currentHitChance) {
		bridgeHealth -= amount;
		bridgeHealth = std::max(bridgeHealth, 0);
		if (bridgeHealth) Log("The bridge has been hit! Current health: " + std::to_string(bridgeHealth));
		else Log("The bridge has been destroyed!");
		hullIntegrity -= amount;
		hullIntegrity = std::max(hullIntegrity, 0);
		if (hullIntegrity) Log("Current hull integrity: " + std::to_string(hullIntegrity));
		else Log("Hull destroyed!");
	}
	else currentHitChance += type.engine.hitChance; if (hitChance < currentHitChance) {
		float originalPower = 0;
		if (engineHealth != 0) {
			originalPower = speed * (float(type.engine.maxHealth) / float(engineHealth));
			originalPower /= type.enginePower;
		}
		engineHealth -= amount;
		engineHealth = std::max(engineHealth, 0);
		if (engineHealth) Log("The engines have been hit! Current health: " + std::to_string(engineHealth));
		else Log("The engines have been destroyed!");
		ChangeHeading(heading, originalPower);
		hullIntegrity -= amount;
		hullIntegrity = std::max(hullIntegrity, 0);
		if (hullIntegrity) Log("Current hull integrity: " + std::to_string(hullIntegrity));
		else Log("Hull destroyed!");
	}
	else currentHitChance += type.steering.hitChance; if (hitChance < currentHitChance) {
		steeringHealth -= amount;
		steeringHealth = std::max(steeringHealth, 0);
		if (steeringHealth) Log("The steering gear has been hit! Current health: " + std::to_string(steeringHealth));
		else Log("The the steering gear has been destroyed!");
		hullIntegrity -= amount;
		hullIntegrity = std::max(hullIntegrity, 0);
		if (hullIntegrity) Log("Current hull integrity: " + std::to_string(hullIntegrity));
		else Log("Hull destroyed!");
	}
	else if (hitChance >= totalHitChance - type.noHitChance) {
		hullIntegrity -= amount;
		hullIntegrity = std::max(hullIntegrity, 0);
		if (hullIntegrity) Log("Current hull integrity: " + std::to_string(hullIntegrity));
		else Log("Hull destroyed!");
	}
	else {
		for (Turret item : Guns)
		{
			currentHitChance += item.hitChance;
			if (hitChance < currentHitChance) {
				item.health -= amount;
				item.health = std::max(item.health, 0);
				if (!item.health) Log("A turret has been hit! Current health: " + std::to_string(item.health));
				else Log("A turret has been destroyed!");
				break;
			}
		}
	}

}

void Warship::Tick()
{
	positionX += velocityX;
	positionY += velocityY;
	if (!fireOrders.empty()) 
	{
		for (int index = fireOrders.size(); index != 0; --index) 
		{
			FireParams item = fireOrders[index-1];
			if (Guns[item.gun].cooldown == 0) 
			{
				Fire(item.gun, item.ammoType, item.targetX, item.targetY);
				Log("Fired at X : " + std::to_string(item.targetX) + ", Y : " + std::to_string(item.targetY));
				if (!item.IsContinuous) 
				{
					fireOrders.erase(fireOrders.begin() + index-1);
				}
			}
			else --Guns[item.gun].cooldown;
		}
	}
	if (hullIntegrity != type.hull.maxHealth) {
		float integrityPercent = hullIntegrity / type.hull.maxHealth;
		flooding += (1 - integrityPercent) * 20;
	}
	if (flooding >= 100.0f) {
		std::cout << name << " has been lost with all hands!\n";
		OtherShips->erase(OtherShips->begin() + indexPosition);
	}
}

void Warship::Log(std::string message)
{
	std::cout << "[" << name << "]: " << message << "\n";
}

void Warship::QueueFire(int gun, unsigned char ammoType, float targetX, float targetY, bool IsContinuous)
{
	fireOrders.push_back(FireParams(targetX, targetY, gun, ammoType, IsContinuous));
}
