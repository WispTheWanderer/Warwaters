#pragma once
#include "Warship.h"
#include <vector>
#include <memory>
struct BattleInstance {
	std::vector<std::unique_ptr<Warship>> Battlefield;
	bool Debug;
	bool BattleGameState = true;
	int BattleClock = 0;
	void takeAndParseInput();
};