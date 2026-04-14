// Warwaters.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Warship.h"
#include <iostream>
#include <sstream>
#include <fstream>
std::vector<std::unique_ptr<Warship>> Battlefield;
bool Debug;
bool BattleGameState = 1;
void takeAndParseInput() {
    std::vector<std::string> Params;
    std::string tempStr;
    while (std::cin >> tempStr) {
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
        std::cout << "List of commands:\n  * fire: Form \"fire [warship name in quotes] [warship gun number] [ammunition type] [x position of target] [y position of target] [continuous fire Y/n]\", fires at targeted position from the targeted warship with the targeted gun.\n  * tick: Form \"tick\", advances the game clock by one tick.\n  * move: Form \"move [warship name in quotes] [heading] [engine power]\", sets a ship on a heading.\n  * look: Form \"look [warship name in quotes]\", displays statistics of the selected warship.\n";
    }
    else if (Params[0] == "tick") {
        for (int index = 0; index < Battlefield.size(); ++index) {
            Battlefield[index]->Tick();
        }
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
                current->get()->QueueFire(std::stoi(Params[2]) - 1, stoi(Params[3]) - 1, stof(Params[4]), stof(Params[5]), b);
                std::cout << "Fire Order Sent\n";
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!";
    }
    else if (Params[0] == "move") {
        if (Params.size() >= 4) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {
                current->get()->ChangeHeading(stof(Params[2]), stof(Params[3]));
                std::cout << "Move Order Sent\n";
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!";
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
        else std::cout << "Not Enough Parameters!";
    }
    else if (Params[0] == "teleport" && Debug) {
        if (Params.size() >= 4) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {

                current->get()->Teleport(stof(Params[1]), stof(Params[2]));
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
        else std::cout << "Not Enough Parameters!";
    }
    else if (Params[0] == "damage" && Debug) {
        if (Params.size() >= 2) {
            auto current = std::find_if(Battlefield.begin(), Battlefield.end(), [Params](const std::unique_ptr<Warship>& n) { return n->getName() == Params[1]; });
            if (current != Battlefield.end()) {

                current->get()->Damage(stoi(Params[1]));
            }
            else {
                std::cout << "That warship does not exist!\n";
            }
        }
    }
    else if (Params[0] == "list") {
        for (int index = 0; index < Battlefield.size(); ++index) {
            Warship* currentPtr = Battlefield[index].get();
            std::cout << "Index number: " << index << "\nName: " << currentPtr->getName() << "\nHeading: " << currentPtr->getHeading() << "\nSpeed: " << currentPtr->getSpeed() << "\nPosition X:" << currentPtr->getPositionX() << ", Y: " << currentPtr->getPositionY() << "\nFlood Level: " << currentPtr->getFloodLevel() << "\n";
        }
    }
    else if (Params[0] == "exit") {
        Battlefield.clear();
        BattleGameState = false;
    }
    else if (Params[0] == "") {

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
    std::ifstream ShipTypesDoc("BasicShipTypes.json");
    nlohmann::json BasicShipTypes = nlohmann::json::parse(ShipTypesDoc);
    std::vector<ShipType> ShipList = BasicShipTypes.get<std::vector<ShipType>>();
    new Warship("Idiot's Vessel",100,100,ShipList[0],&Battlefield);
    new Warship("The Other Idiot's Vessel", -100, -100, ShipList[1],&Battlefield);
    while (BattleGameState) {
        takeAndParseInput();
        if (Battlefield.empty()) BattleGameState = false;
    }
    std::cout << "All ships have been destroyed or the game has been manually exited.\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
