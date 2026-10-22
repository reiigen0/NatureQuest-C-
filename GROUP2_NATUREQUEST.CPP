#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main () {
    // basis
   const int MAX_HEARTS = 3;
   const int NUM_MAPS = 5;
   const int NUM_PROBLEMS = 10;   
   const int HEART_COST = 45;
   const int TREE_COST = 50;
   const int UNLOCK_COST = 60;
   const int NUM_UPGRADES = 5;

   // results
   const int MAP_DONE = 0;
   const int MAP_END = 1;
   const int MAP_RESTART = 2;
    
   // current game variables
   int hearts = MAX_HEARTS;
   int points = 0;
   int totalEarned = 0;
   int totalSpent = 0;
   int trees = 0;
   int heartsBought = 0;
   int comebacks = 0;
   int upgrades[NUM_UPGRADES] = {0, 0, 0, 0, 0};
   int unlocked = 1;
   bool completed[NUM_MAPS] = {false, false, false, false, false};
   int solved = 0;
   int failed = 0;
   int rightAns = 0;
   int wrongAns = 0;

   string upgradeName[NUM_UPGRADES] = {
    "Recycling bins", "Compost bin", "Rain barrel", "Reusable bag pack", "Solar lamp"
};

int upgradeCost[NUM_UPGRADES] = {30, 35, 45, 25, 60};
string upgradeImpact[NUM_UPGRADES] = {
    "keeps recyclables out of the trash and out of the drains",
    "turns food scraps and leaves into free fertilizer",
    "collects rainwater so less clean water is wasted",
    "replaces hundreds of single-use plastic bags",
    "lights a path using clean energy instead of fuel"
};

string MAP_NAMES[NUM_MAPS] = {"Neighborhood", "Community Park", "Farm", "Forest", "Water Areas"};


    return 0;
}