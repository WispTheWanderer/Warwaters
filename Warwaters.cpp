
#include "Warship.h"
#include "BattleInstance.h"
#include <iostream>
#include <sstream>
#include <fstream>
constexpr int WW_MAXTICKCOUNT = 100;
std::vector<ShipType> SpawnableShipTypes;
void BattleInstance::takeAndParseInput() {
    std::vector<std::string> Params;
    std::string InputString;
    std::getline(std::cin, InputString);
    std::stringstream InputStream(InputString);
    std::string tempStr;
    while (std::getline(InputStream, tempStr, ' ')) {
        Params.push_back(tempStr);
    }
    for (int index = Params.size() - 1; index > -1; --index) { // makes anything within a quotation a single token
        if (Params[index].back() == '"' && Params[index][0] != '"') {
            Params[index - 1].append(" " + Params[index]);
            Params.erase(Params.begin() + index);
        }
    }
    for (int index = 0; index < Params.size(); ++index) { // removes quotes
        if (Params[index][0] == '"') {
            Params[index].erase(Params[index].begin());
            Params[index].erase(Params[index].end()-1);
        }
    }
    if (Params[0] == "help") {
        std::cout << "List of commands:\n  * fire: Form \"fire [warship name in quotes] [warship gun number] [ammunition type] [x position of target] [y position of target] [continuous fire Y/n]\", fires at targeted position from the targeted warship with the targeted gun.\n  * tick: Form \"tick\", advances the game clock by one tick.\n  * move: Form \"move [warship name in quotes] [heading] [engine power]\", sets a ship on a heading.\n  * look: Form \"look [warship name in quotes]\", displays statistics of the selected warship.\n  * spawn: Form \"spawn [name] [x position] [y position] [ship type]\"";
    }
    else if (Params[0] == "tick") {
        int tickCount = 0;
        if (Params.size() == 2) {
            try {
                tickCount = stoi(Params[1]);
            }
            catch (const std::exception& e) {
                std::cout << "Incorrect Parameters.\n";
            }
        }
        else {
            tickCount = WW_MAXTICKCOUNT;
        }
        
        bool Ticking = true;
        for (int count = 0; count < tickCount; ++count) {
            for (int index = 0; index < Battlefield.size(); ++index) {
                Battlefield[index]->Tick();
                if (Battlefield[index]->getIfActioned()) {
                    Ticking = false;
                }
                
            }
            ++BattleClock;
            for (int index = Battlefield.size() - 1; index != 0; --index) {
                if (Battlefield[index]->getIfDead()) {
                    Battlefield.erase(Battlefield.begin() + index);
                }
            }
            if (!Ticking) break;
        }
        std::cout << "Game clock: " << BattleClock << "\n";
    }
    else if (Params[0] == "fire") {
        if (Params.size() >= 6) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {
                bool b;
                if (Params.size() > 6) {
                    std::istringstream(Params.at(6)) >> std::boolalpha >> b;
                }
                else {
                    b = false;
                };
                if (!(current->get()->getIfEnemy())) {
                    try {
                        if (std::stoi(Params[2]) < 1) throw;
                        if (std::stoi(Params[3]) < 1) throw;
                        current->get()->QueueFire(std::stoi(Params[2]) - 1, stoi(Params[3]) - 1, stof(Params[4]), stof(Params[5]), b);
                        std::cout << "Fire Order Sent\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Incorrect Parameters.\n";
                    }
                }
                else std::cout << "That isn't your warship!\n";
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!\n";
    }
    else if (Params[0] == "move") {
        if (Params.size() >= 4) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {
                if (!(current->get()->getIfEnemy())) {
                    try {
                        current->get()->ChangeHeading(stof(Params[2]), stof(Params[3]));
                        std::cout << "Move Order Sent\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Incorrect Parameters.\n";
                    }
                }
                else std::cout << "That isn't your warship!\n";
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!\n";
    }
    else if (Params[0] == "projectdebugcombat") Debug = !Debug;
    else if (Params[0] == "look") {
        if (Params.size() >= 2) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {
                Warship* currentPtr = current->get();
                std::cout << "Name: " << currentPtr->getName() << "\nHeading: " << currentPtr->getHeading() << "\nSpeed: " << currentPtr->getSpeed() << "\nPosition X:" << currentPtr->getPositionX() << ", Y: " << currentPtr->getPositionY() << "\nFlood Level: " << currentPtr->getFloodLevel() << "\n";

            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!\n";
    }
    else if (Params[0] == "teleport" && Debug) {
        if (Params.size() >= 4) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {
                try {
                    current->get()->Teleport(stof(Params[2]), stof(Params[3]));
                }
                catch (const std::exception& e) {
                    std::cout << "Incorrect Parameters.\n";
                }
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!\n";
    }
    else if (Params[0] == "damage" && Debug) {
        if (Params.size() >= 2) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {
                try {
                    current->get()->Damage(stoi(Params[2]));
                }
                catch (const std::exception& e) {
                    std::cout << "Incorrect Parameters.\n";
                }
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!\n";
    }
    else if (Params[0] == "list") {
        if (Params.size() < 2) {
            for (int index = 0; index < Battlefield.size(); ++index) {
                Warship& currentPtr = *Battlefield[index];
                std::cout << "Index number: " << index << "\nName: " << currentPtr.getName() << "\nHeading: " << currentPtr.getHeading() << "\nSpeed: " << currentPtr.getSpeed() << "\nPosition X:" << currentPtr.getPositionX() << ", Y: " << currentPtr.getPositionY() << "\nFlood Level: " << currentPtr.getFloodLevel() << "\n";
            }
        }
        else if (Params[1] == "types") {
            for (int index = 0; index < SpawnableShipTypes.size(); ++index) {
                std::cout << "#" << index << ":\n    Name: " << SpawnableShipTypes[index].name << "\n    Engine Power: " << SpawnableShipTypes[index].enginePower << "\n    Spawn Cost: " << SpawnableShipTypes[index].spawnCost << "\n    Size: " << SpawnableShipTypes[index].size << "\n    Turrets:\n";
                for (int jndex = 0; jndex < SpawnableShipTypes[index].Turrets.size(); ++jndex) {
                    std::cout << "        Number of cannons: " << SpawnableShipTypes[index].Turrets[jndex].CannonCount << "\n        Range: " << SpawnableShipTypes[index].Turrets[jndex].range << "\n        Reload Time: " << SpawnableShipTypes[index].Turrets[jndex].reloadTime << "\n        Repair Speed: " << SpawnableShipTypes[index].Turrets[jndex].repairSpeed << "\n";
                }
                std::cout << "    Engine: \n        Repair Speed: " << SpawnableShipTypes[index].engine.repairSpeed << "\n        Maximum Health: " << SpawnableShipTypes[index].engine.maxHealth << "\n";
                std::cout << "    Bridge: \n        Repair Speed: " << SpawnableShipTypes[index].bridge.repairSpeed << "\n        Maximum Health: " << SpawnableShipTypes[index].bridge.maxHealth << "\n";
                std::cout << "    Steering Gear: \n        Repair Speed: " << SpawnableShipTypes[index].steering.repairSpeed << "\n        Maximum Health: " << SpawnableShipTypes[index].steering.maxHealth << "\n";
                std::cout << "    Hull: \n        Repair Speed: " << SpawnableShipTypes[index].hull.repairSpeed << "\n        Maximum Hull Integrity: " << SpawnableShipTypes[index].hull.maxHealth << "\n";
            }
        }
    }
    else if (Params[0] == "exit") {
        Battlefield.clear();
        BattleGameState = false;
        std::cout << "The game has been manually exited.\n";
    }
    else if (Params[0] == "spawn") {
        if (Params.size() >= 5) {
            if (std::any_of(Params[4].begin(), Params[4].end(), [](unsigned char c) { return !isdigit(c); })) {
                auto selectedShipType = std::find_if(SpawnableShipTypes.begin(), SpawnableShipTypes.end(), [Params](ShipType s) { return s.name == Params[4]; });
                if (selectedShipType != SpawnableShipTypes.end()) {
                    try {
                        Battlefield.push_back(std::make_unique<Warship>(Params[1], stof(Params[2]), stof(Params[3]), *selectedShipType, &Battlefield));
                    }
                    catch (const std::exception& e) {
                        std::cout << "Incorrect Parameters.\n";
                    }
                }
                else {
                    std::cout << "That warship type does not exist!\n";
                }
            }
            else {
                if (std::stoi(Params[4]) < SpawnableShipTypes.size()) {
                    try {
                        new Warship(Params[1], stof(Params[2]), stof(Params[3]), SpawnableShipTypes[std::stoi(Params[4])], &Battlefield);
                        std::cout << "Ship Spawned!\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Incorrect Parameters.\n";
                    }
                }
                else {
                    std::cout << "That warship type does not exist!\n";
                }
            }
        }
        else {
            std::cout << "Not Enough Parameters!\n";
        }
    }
    else if (Params[0] == "") {

    }
    else if (Params[0] == "") {

    }
    else {
        std::cout << "Unknown command, type \"help\" to get a list of commands!\n";
    }
}
int main()
{
    BattleInstance Mainbattle;
    std::ifstream ShipTypesDoc("BasicShipTypes.json");
    nlohmann::json BasicShipTypes = nlohmann::json::parse(ShipTypesDoc);
    SpawnableShipTypes = BasicShipTypes.get<std::vector<ShipType>>();
    Mainbattle.Battlefield.push_back(std::make_unique<Warship>("Idiot's Vessel",100,100,SpawnableShipTypes[0],&Mainbattle.Battlefield));
    Mainbattle.Battlefield.push_back(std::make_unique<Warship>("The Other Idiot's Vessel", -100, -100, SpawnableShipTypes[1],&Mainbattle.Battlefield,true));
    while (Mainbattle.BattleGameState) {
        Mainbattle.takeAndParseInput();
        if (!(std::any_of(Mainbattle.Battlefield.begin(), Mainbattle.Battlefield.end(), [](const std::unique_ptr<Warship>& n) { return n->getIfEnemy(); })) && (std::any_of(Mainbattle.Battlefield.begin(), Mainbattle.Battlefield.end(), [](const std::unique_ptr<Warship>& n) { return !(n->getIfEnemy()); })))
        { 
            Mainbattle.BattleGameState = false; 
            std::cout << "Congratulations! You have destroyed all enemy ships in the area!\n";
        }
        if (!(std::any_of(Mainbattle.Battlefield.begin(), Mainbattle.Battlefield.end(), [](const std::unique_ptr<Warship>& n) { return !(n->getIfEnemy()); })) && (std::any_of(Mainbattle.Battlefield.begin(), Mainbattle.Battlefield.end(), [](const std::unique_ptr<Warship>& n) { return n->getIfEnemy(); })))
        {
            std::cout << "Unfortunately, you have lost all ships. Womp womp!\n";
        }

    }
    
}

