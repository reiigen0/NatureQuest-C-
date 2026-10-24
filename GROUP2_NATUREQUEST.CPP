#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_HEARTS = 3;
const int NUM_MAPS = 5;
const int NUM_PROBLEMS = 10;
const int HEART_COST = 45;
const int UNLOCK_COST = 60;
const int UPGRADE_COST = 40;
const int NUM_UPGRADES = 5;

// results of playing a map
const int MAP_DONE = 0;
const int MAP_END = 1;
const int MAP_RESTART = 2;

// ---------------- game variables ----------------
int hearts = MAX_HEARTS;
int points = 0;
int totalEarned = 0;
int totalSpent = 0;
int upgrades[NUM_UPGRADES] = {0, 0, 0, 0, 0};
int unlocked = 1;
bool completed[NUM_MAPS] = {false, false, false, false, false};

// ---------------- upgrades ----------------
// upgrade number i belongs to map number i
string upgradeName[NUM_UPGRADES] = {
    "Neighborhood: Plant a Tree",
    "Community Park: Add Recycling Bins",
    "Farm: Add Crop Sheds",
    "Forest: Add Animal Safety Fences",
    "Water Areas: Add Fish Reefs"
};
int upgradeCost[NUM_UPGRADES] = {UPGRADE_COST, UPGRADE_COST, UPGRADE_COST, UPGRADE_COST, UPGRADE_COST};
string upgradeImpact[NUM_UPGRADES] = {
    "gives the neighborhood shade, cleaner air, and a home for birds",
    "keeps litter out of the grass and the park pond",
    "protects harvested crops and keeps farm waste from scattering",
    "keeps wildlife safe from trails, roads, and litter",
    "gives fish a place to shelter and breed, helping the water recover"
};

string MAP_NAMES[NUM_MAPS] = {"Neighborhood", "Community Park", "Farm", "Forest", "Water Areas"};

// How a problem works:
//  question 1 (4 choices): right -> solved, wrong -> question 2
//  question 2 (3 choices): the text depends on which wrong answer you gave.
//      choice 0 is right, 1 = "B", 2 = "C". B or C -> question 3
//  question 3 (3 choices): right -> solved, wrong -> lose a heart

string BAD_TEXT = "That choice makes the problem worse for the environment.";

// question 3 choices, if you picked B in question 2
string AFTER_B_OPTIONS[3] = {
    "Continue leaving the problem alone.",
    "Add more waste to the affected area.",
    "Take appropriate action to address the waste and report the larger problem if needed."
};

// question 3 choices, if you picked C in question 2
string AFTER_C_OPTIONS[3] = {
    "Move the waste somewhere else again.",
    "Ignore the new pile because it is away from the original location.",
    "Collect or address the accessible waste properly and inform the appropriate person when necessary."
};

// texts shared by all the problems of one map
struct MapSpec {
    string q2Options[3];   // [0] is the right one
    string q2Texts[7];     // which one is used depends on "var" in Question
    string q3AfterB;
    string q3AfterC;
};

MapSpec MAPSPEC[NUM_MAPS] = {
 { // ===== MAP 1: NEIGHBORHOOD =====
  {"Take action to address the problem appropriately and help prevent the situation from worsening.",
   "Leave the problem for someone else to handle.",
   "Move the waste to another nearby spot."},
  {"The waste remains where it was left, and other people begin adding more litter around it. What will you do when you notice the problem getting worse?",
   "The waste is carried into or trapped inside a drainage area, leaving less space for rainwater to pass through. What will you do when you notice the problem getting worse?",
   "The action leaves more waste in the neighborhood, and the problem begins spreading to nearby areas. What will you do when you notice the problem getting worse?",
   "The waste spreads across a larger part of the neighborhood, making the area harder to keep clean. What will you do when you notice the problem getting worse?",
   "The hidden waste stays in the area and encourages other people to leave more garbage there. What will you do when you notice the problem getting worse?",
   "", ""},
  "The untreated garbage keeps accumulating, and rain or wind begins carrying some of it toward another part of the neighborhood. What will you do when you see the problem spreading?",
  "The waste moved to the new location is scattered by wind or rain and begins collecting near another drainage or public area. What will you do when you notice the waste reaching the new area?"
 },
 { // ===== MAP 2: COMMUNITY PARK =====
  {"Take action to address the problem appropriately and help prevent the situation from worsening.",
   "Ignore the problem because park workers can clean it later.",
   "Hide or move the waste somewhere less noticeable."},
  {"The litter stays in the park, and more visitors begin treating the area as a place where trash can be left. What will you do when you notice the condition of the park worsening?",
   "The hidden litter remains among the plants or underneath park structures, making it harder to notice and collect. What will you do when you notice the condition of the park worsening?",
   "The waste spreads across the lawn or paths, affecting the cleanliness and appearance of the park. What will you do when you notice the condition of the park worsening?",
   "The action contributes to more waste or damage in the park, affecting other visitors and the surrounding environment. What will you do when you notice the condition of the park worsening?",
   "The waste reaches the pond or another wet area in the park, adding to the pollution already present. What will you do when you notice the condition of the park worsening?",
   "", ""},
  "The neglected waste or damage remains until more visitors are affected and additional litter appears nearby. What will you do when you see the problem spreading?",
  "The hidden or relocated waste is later found in another part of the park, where more visitors begin adding litter. What will you do when you notice the waste reaching the new area?"
 },
 { // ===== MAP 3: FARM =====
  {"Take action to address the problem appropriately and help prevent the situation from worsening.",
   "Leave the problem for the farmer or another worker.",
   "Move the waste or problem to another part of the farm."},
  {"The waste stays on the farm, and additional litter begins appearing around the same area. What will you do when you notice the effect becoming more noticeable?",
   "The waste or material enters the irrigation system or nearby water, where it can interfere with water flow or quality. What will you do when you notice the effect becoming more noticeable?",
   "The waste remains around the crops and becomes mixed with soil or other farm materials. What will you do when you notice the effect becoming more noticeable?",
   "The action creates an environmental problem that begins affecting the farm's soil, water, crops, or waste management. What will you do when you notice the effect becoming more noticeable?",
   "Smoke and residue from the burned waste affect the surrounding farm area. What will you do when you notice the effect becoming more noticeable?",
   "The waste spreads across the field, making collection more difficult and increasing the chance that it reaches water. What will you do when you notice the effect becoming more noticeable?",
   "The increased activity puts additional pressure on farm resources and the surrounding environment. What will you do when you notice the effect becoming more noticeable?"},
  "The unresolved problem continues affecting the farm, and more waste or runoff begins reaching another part of the property. What will you do when you see the problem spreading?",
  "The relocated waste reaches another part of the farm and begins interfering with soil, crops, irrigation, or cleanup. What will you do when you notice the waste reaching the new area?"
 },
 { // ===== MAP 4: FOREST =====
  {"Take action to address the problem appropriately and help prevent the situation from worsening.",
   "Ignore the situation and continue on your way.",
   "Move or hide the problem somewhere else in the forest."},
  {"The waste remains on the trail, and other visitors begin leaving additional litter nearby. What will you do when you notice the forest area being affected?",
   "The waste becomes hidden among leaves or plants, where it can be difficult for visitors to notice and remove. What will you do when you notice the forest area being affected?",
   "The waste spreads along the trail or into surrounding vegetation, increasing the area affected. What will you do when you notice the forest area being affected?",
   "The action creates additional disturbance or waste in the forest and begins affecting the surrounding habitat. What will you do when you notice the forest area being affected?",
   "The disturbance affects plants or habitat features that wildlife may depend on. What will you do when you notice the forest area being affected?",
   "", ""},
  "The litter or disturbance remains in the habitat, and wind, rain, or visitors spread its effects farther along the area. What will you do when you see the problem spreading?",
  "The relocated waste becomes mixed with leaves or vegetation and is carried farther along the trail or toward wildlife habitat. What will you do when you notice the waste reaching the new area?"
 },
 { // ===== MAP 5: WATER AREAS =====
  {"Take action to address the problem appropriately and help prevent the situation from worsening.",
   "Leave the problem for someone else to deal with.",
   "Move the waste somewhere farther from where people can see it."},
  {"The waste remains in the area, and more visitors begin leaving similar litter nearby. What will you do when you notice the problem becoming more serious?",
   "The waste enters the water or is carried closer to it, where it can add to pollution and affect aquatic life. What will you do when you notice the problem becoming more serious?",
   "The waste remains hidden near the shore or waterway and can later be uncovered or carried away by wind or rain. What will you do when you notice the problem becoming more serious?",
   "The waste spreads along the shore or toward the water, increasing the area affected. What will you do when you notice the problem becoming more serious?",
   "The increased use of disposable materials creates more waste that can reach the water. What will you do when you notice the problem becoming more serious?",
   "The action adds to the amount of waste around the water area and increases the chance that it will reach the water. What will you do when you notice the problem becoming more serious?",
   ""},
  "The waste stays near the water until waves, wind, or rain begin carrying it closer to the waterway. What will you do when you see the problem spreading?",
  "The relocated waste is carried by wind, rain, or water toward the shoreline or waterway. What will you do when you notice the waste reaching the new area?"
 }
};

struct Question {
    string text;
    string opt[4];
    int right;          // index of the right option
    string good;        // shown when you get it right
    int var[4];         // which q2Texts to use for each wrong option
};

Question QUESTIONS[NUM_MAPS][NUM_PROBLEMS] = {
 { // ===== MAP 1: NEIGHBORHOOD =====
  {"You just finished drinking a bottled drink while walking around your neighborhood. What will you do with the empty bottle?",
   {"Leave it on the sidewalk.","Throw it into a nearby drain.","Put it in the proper waste bin.","Leave it beside a tree."}, 2,
   "Putting waste in the proper bin keeps the street clean and stops litter from reaching drains.", {0,1,0,0}},
  {"You are eating a snack with your friends and accidentally drop the wrapper on the ground. What will you do?",
   {"Leave it because it is small.","Pick up and throw it in the proper bin.","Kick it under a bench.","Ask your friend to ignore it."}, 1,
   "Even a small wrapper counts. Binning it keeps the area clean and sets an example.", {0,0,4,0}},
  {"You notice your friend is about to throw a plastic bottle on the street. What will you do?",
   {"Tell them to dispose of it properly.","Ignore them.","Throw another bottle.","Walk away."}, 0,
   "Speaking up kindly can change habits before litter ever lands on the street.", {0,0,3,0}},
  {"The trash bin near your house is full and you have plastic waste. What will you do?",
   {"Put it beside the bin.","Throw it into the street.","Keep it until an appropriate place is available.","Throw it into the drainage canal."}, 2,
   "Holding on to waste until it can be disposed of properly stops it from scattering.", {2,3,0,1}},
  {"You and your friends plan a small gathering in your neighborhood. Which plan reduces waste?",
   {"Use many disposable items.","Bring reusable containers and provide proper bins.","Let everyone leave their trash.","Avoid using bins."}, 1,
   "Reusable containers and proper bins prevent waste before it starts.", {2,0,0,2}},
  {"Plastic bottles and food wrappers are blocking a drainage canal near your home before heavy rain. What will you do?",
   {"Add more garbage.","Push the garbage farther into the canal.","Report the blockage or help remove it safely.","Ignore it."}, 2,
   "Reporting or safely clearing a blockage early prevents flooding.", {3,1,0,0}},
  {"Someone repeatedly throws household garbage into an empty lot. What is the most responsible thing to do?",
   {"Start throwing garbage there too.","Ignore it.","Report it to the appropriate community authority.","Spread the garbage around the lot."}, 2,
   "Reporting illegal dumping to the authorities stops it from becoming a permanent dump.", {3,0,0,3}},
  {"Your neighborhood frequently floods after heavy rain because garbage blocks drainage. What do you suggest?",
   {"Clean only after flooding.","Improve waste disposal and regularly keep drainage clear.","Throw garbage farther away.","Stop using drainage canals."}, 1,
   "Preventing blockages with good disposal and regular drain upkeep stops floods at the source.", {2,0,3,1}},
  {"Your neighborhood has cleanup activities, but people keep littering. What is a better approach?",
   {"Stop the cleanups.","Only punish litterers.","Combine cleanups with proper bins, education, and community participation.","Move the garbage somewhere else."}, 2,
   "Cleanups work best together with bins, education and community participation.", {0,2,0,3}},
  {"Your community has limited money and must choose between repeated cleanup and a better waste-management system. What do you recommend?",
   {"Only continue cleanup activities.","Invest in prevention while continuing necessary cleanup.","Stop collecting waste.","Let residents handle waste individually."}, 1,
   "Prevention at the source saves money and keeps waste from piling up while essential cleanups continue.", {2,0,0,2}}
 },
 { // ===== MAP 2: COMMUNITY PARK =====
  {"You are eating a snack in the park, but there is no bin nearby. What will you do with the wrapper?",
   {"Leave it on the grass.","Hide it under a bench.","Keep it until you find a proper bin.","Throw it into the bushes."}, 2,
   "Keeping your wrapper until you find a bin keeps the park clean and safe for wildlife.", {0,1,0,2}},
  {"Your friend steps on plants to take a shortcut. What will you do?",
   {"Join them.","Tell them to use the designated path.","Ignore it.","Step on more plants."}, 1,
   "Designated paths protect plants and soil from damage.", {3,0,0,3}},
  {"You notice an empty plastic bottle beside a park bench. What will you do?",
   {"Throw it into the pond.","Leave it there.","Put it in an appropriate bin.","Hide it behind the bench."}, 2,
   "Using a bin keeps litter out of the grass and the pond.", {4,0,0,1}},
  {"You are visiting the park with friends who leave wrappers on the grass. What will you do?",
   {"Leave them because workers clean the park.","Encourage everyone to collect and properly dispose of the wrappers.","Throw more wrappers.","Walk away."}, 1,
   "Encouraging friends to clean up sets a positive example for other visitors.", {0,0,2,0}},
  {"Your school is planning a picnic in the park. Which plan is more environmentally friendly?",
   {"Bring reusable containers and waste-segregation bins.","Use disposable items and leave the waste.","Do not provide bins.","Throw all waste in one area."}, 0,
   "Reusable containers and sorted waste cut down picnic trash.", {0,0,3,2}},
  {"During a park cleanup, you find many plastic bottles around a pond. What will you do?",
   {"Throw them into the pond.","Collect them safely and place them in the appropriate waste container.","Hide them under plants.","Leave them because they are outside the main path."}, 1,
   "Collecting bottles safely keeps plastic out of the water and protects pond life.", {4,0,1,0}},
  {"Visitors keep damaging plants in one part of the park because there is no clear walking path. What could the community do?",
   {"Remove all the plants.","Create or improve designated paths and educate visitors.","Close the park.","Ignore the problem."}, 1,
   "Clear paths and education protect plants while still letting people enjoy the park.", {2,0,3,0}},
  {"The park remains littered even after volunteers clean it. What should the community focus on?",
   {"Clean more without changing anything.","Find the causes and improve prevention, education, and waste collection.","Remove the benches.","Close the park permanently."}, 1,
   "Finding the root causes and improving prevention solves the problem, not just the symptoms.", {3,0,2,3}},
  {"A new recreational area requires some trees to be removed. What should decision-makers consider first?",
   {"The appearance of the area only.","Effects on trees, wildlife, shade, drainage, and the ecosystem.","The cost only.","What visitors want only."}, 1,
   "Considering shade, wildlife, drainage and the ecosystem helps protect what the park provides.", {3,0,3,3}},
  {"The park pond is polluted because visitors throw plastic and food into it. What is the most effective approach?",
   {"Clean it once.","Remove the waste and prevent further littering through education, bins, and monitoring.","Cover the pond with plastic.","Drain the pond permanently."}, 1,
   "Removing waste and preventing new litter keeps the pond healthy over time.", {3,0,4,4}}
 },
 { // ===== MAP 3: FARM =====
  {"While helping on a farm, you notice an empty plastic bottle between the crops. What will you do?",
   {"Leave it there.","Throw it into the irrigation canal.","Pick it up and dispose of it properly.","Bury it beside the crops."}, 2,
   "Proper disposal keeps plastic out of soil, crops and irrigation water.", {0,1,0,2}},
  {"You notice water being wasted from an open hose on a farm. What will you do?",
   {"Ignore it.","Tell the responsible person or turn it off if appropriate.","Use more water.","Block the hose with garbage."}, 1,
   "Saving water protects a limited resource that farms and communities depend on.", {0,0,1,3}},
  {"You see several pieces of plastic waste scattered around the farm. What should you do?",
   {"Collect them and take them to the proper disposal area.","Bury them.","Burn them.","Throw them into a stream."}, 0,
   "Collecting waste for proper disposal keeps farmland and streams clean.", {0,2,4,1}},
  {"Plastic waste is blocking an irrigation channel. What will you do?",
   {"Push it farther into the channel.","Add more waste.","Report the blockage or help remove it safely.","Ignore it."}, 2,
   "Clearing blockages safely keeps irrigation flowing and prevents polluted runoff.", {5,5,0,0}},
  {"Heavy rain is washing soil from the farm into a nearby stream. What would you suggest?",
   {"Remove the plants.","Use appropriate practices to reduce runoff and erosion.","Leave the soil bare.","Cover the soil with plastic waste."}, 1,
   "Reducing runoff and erosion keeps topsoil on the farm and out of the stream.", {5,0,0,2}},
  {"A farmer wants to throw empty agricultural containers beside the field. What would you suggest?",
   {"Dispose of them according to appropriate waste and safety procedures.","Throw them into the river.","Bury them anywhere.","Leave them among the crops."}, 0,
   "Following proper disposal and safety procedures protects soil, water and people.", {0,1,2,0}},
  {"Your school visits a farm and the activity creates a lot of plastic waste. What should students do?",
   {"Leave it for the farmers.","Bring reusable items and ensure the waste is properly collected.","Throw it around the field.","Hide it before leaving."}, 1,
   "Reusable items and proper collection prevent plastic from being left on farmland.", {0,0,5,2}},
  {"A farm is producing more crops, but increased activity is causing erosion and water pollution. What should be prioritized?",
   {"Production only.","Practices that maintain productivity while protecting soil and water.","Increasing waste.","Removing all vegetation."}, 1,
   "Protecting soil and water keeps the farm productive for the long run.", {6,0,3,3}},
  {"Waste from a farm regularly enters a nearby river. What is the best response?",
   {"Move the waste somewhere else.","Identify the source, stop improper disposal, and establish better waste management.","Cover the soil.","Ignore it."}, 1,
   "Stopping pollution at its source protects the river and the farm.", {5,0,2,0}},
  {"The community wants to support a local farm while protecting the environment. What is the best approach?",
   {"Focus only on production.","Use farming practices that conserve water, protect soil, reduce waste, and maintain productivity.","Use more disposable plastics.","Use as many resources as possible."}, 1,
   "Conserving water and soil while reducing waste supports farms and the environment.", {3,0,6,6}}
 },
 { // ===== MAP 4: FOREST =====
  {"While hiking, you accidentally drop a wrapper. What will you do?",
   {"Leave it.","Hide it under leaves.","Pick it up and keep it until you find a proper bin.","Throw it farther into the forest."}, 2,
   "Carrying your litter out keeps the forest clean for wildlife.", {0,1,0,2}},
  {"You see someone breaking tree branches for fun. What will you do?",
   {"Join them.","Tell them to stop and respect the forest.","Break another branch.","Ignore it."}, 1,
   "Respecting trees and plants protects the habitat wildlife depends on.", {3,0,4,0}},
  {"You see an empty bottle along a forest trail. What will you do?",
   {"Throw it deeper into the forest.","Leave it there.","Take it with you and dispose of it properly.","Bury it."}, 2,
   "Taking litter with you prevents animals from swallowing or getting tangled in it.", {2,0,0,1}},
  {"While hiking, you notice people leaving wrappers along the trail. What will you do?",
   {"Leave them there.","Encourage the group to collect and properly dispose of them.","Add your own wrappers.","Hide them under leaves."}, 1,
   "Encouraging others to clean up keeps the trail and habitat healthy.", {0,0,2,1}},
  {"Visitors are feeding wild animals leftover food. What will you do?",
   {"Encourage them.","Explain that feeding wildlife can interfere with natural behavior.","Feed the animals more.","Bring more food for them."}, 1,
   "Wildlife stays healthy and wild when it finds its own natural food.", {3,0,3,3}},
  {"People are creating shortcuts through an area with young plants. What would you suggest?",
   {"Continue using the shortcut.","Remove the young plants.","Create more shortcuts.","Encourage visitors to use designated trails."}, 3,
   "Designated trails protect young plants so the forest can regrow.", {4,2,4,0}},
  {"Several trees have been cut down where animals appear to live. What will you do?",
   {"Cut more trees.","Ignore it.","Report suspected unauthorized clearing to the appropriate authorities.","Remove the remaining plants."}, 2,
   "Reporting unauthorized clearing helps protect habitat and the forest ecosystem.", {4,0,0,2}},
  {"Heavy rain is causing a hiking trail to wash soil downhill. What will you do?",
   {"Report the erosion and encourage appropriate trails.","Widen the trail.","Remove vegetation.","Dig the trail deeper."}, 0,
   "Reporting erosion and using proper trails helps stop soil from washing away.", {0,3,2,3}},
  {"A forest area is being considered for construction, and animals depend on it for food and shelter. What should decision-makers consider?",
   {"Buildings only.","Effects on wildlife, habitats, biodiversity, soil, and water.","Cost only.","Appearance only."}, 1,
   "Considering wildlife, soil and water helps decision-makers protect the whole ecosystem.", {3,0,3,3}},
  {"The community wants tourism in part of a forest. What is a responsible plan?",
   {"Remove trees for development.","Develop without restrictions.","Plan around habitat protection, responsible waste management, and environmental limits.","Remove wildlife from the area."}, 2,
   "Responsible planning lets people enjoy the forest while protecting habitat and limiting waste.", {2,3,0,2}}
 },
 { // ===== MAP 5: WATER AREAS =====
  {"You are at the beach with friends and finish a bottled drink. What will you do with the bottle?",
   {"Throw it into the sea.","Leave it on the sand.","Keep it until you find a proper waste or recycling bin.","Bury it in the sand."}, 2,
   "Keeping your bottle until you find a proper bin keeps plastic out of the sea.", {1,0,0,2}},
  {"Your friend is about to throw a wrapper into a river. What will you do?",
   {"Stop them and suggest proper disposal.","Ignore them.","Throw another wrapper.","Push it farther into the water."}, 0,
   "Speaking up stops waste before it ever enters the water.", {0,0,3,1}},
  {"During a beach trip, you notice several plastic bottles on the sand. What will you do?",
   {"Leave them there.","Throw them into the sea.","Collect them if safe and place them in an appropriate waste container.","Bury them in the sand."}, 2,
   "Collecting bottles safely keeps plastic from being washed out to sea.", {0,1,0,2}},
  {"You are having a picnic beside a river. Before leaving, you notice bottles and wrappers nearby. What will you do?",
   {"Leave them behind.","Collect them and dispose of them properly.","Throw them into the river.","Hide them behind rocks."}, 1,
   "Taking your waste with you keeps the river clean for people and animals.", {0,0,1,2}},
  {"A drainage canal connected to a river is filled with plastic waste. What will you do?",
   {"Push the waste into the river.","Report the blockage or help with an appropriate cleanup.","Add more waste.","Ignore it."}, 1,
   "Reporting or helping clear the blockage keeps plastic from reaching the river.", {1,0,3,0}},
  {"Your friends are using many disposable plastics at the beach even though reusable options are available. What will you suggest?",
   {"Use even more disposable items.","Use reusable alternatives and dispose of remaining waste properly.","Leave the beach.","Throw the plastics into the sea."}, 1,
   "Reusable items cut the plastic that can end up in the water.", {4,0,0,1}},
  {"During a community river cleanup, what should you do with collected bottles?",
   {"Throw them back into the river.","Properly segregate and dispose of or recycle them through appropriate channels.","Bury them.","Leave them in a pile."}, 1,
   "Sorting and recycling collected waste makes sure it does not return to the river.", {1,0,2,0}},
  {"After a storm, plastic is being carried from a river toward the sea. How can the community help prevent this from happening again?",
   {"Only clean the beach.","Create larger piles of garbage.","Reduce waste at the source and improve collection and drainage.","Ignore it because the ocean is large."}, 2,
   "Reducing waste at the source and improving collection and drainage stops plastic before it reaches the sea.", {5,5,0,0}},
  {"The community regularly cleans a river, but plastic keeps returning. What should be the next step?",
   {"Stop cleaning the river.","Identify the source and address the cause of the pollution.","Move the waste downstream.","Clean only the riverbank."}, 1,
   "Finding the pollution source stops plastic from returning again and again.", {1,0,1,1}},
  {"What is the most effective plan for protecting a water area used by people and aquatic animals?",
   {"Organize occasional cleanups only.","Remove only visible plastic.","Allow disposal as long as the water looks clean.","Reduce pollution at the source, improve waste management, monitor water, and educate the community."}, 3,
   "Tackling pollution at the source, with monitoring and education, protects water areas long-term.", {5,3,1,0}}
 }
};

// ---------------- small helper functions ----------------

void printLine() {
    cout << "------------------------------------------------------------\n";
}

string readLine() {
    string s;
    if (!getline(cin, s)) {
        cout << "\nInput closed. Goodbye!\n";
        exit(0);
    }
    return s;
}

void waitForEnter() {
    cout << "\n[Press ENTER to continue]";
    readLine();
}

bool isNumber(string s) {
    if (s.empty() || s.size() > 6) return false;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

// keeps asking until the player types a number between lo and hi
int getChoice(int lo, int hi) {
    while (true) {
        cout << "> ";
        string s = readLine();
        if (isNumber(s)) {
            int n = atoi(s.c_str());
            if (n >= lo && n <= hi) return n;
        }
        cout << "Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

string heartsStr(int h) {
    string s = "";
    for (int i = 0; i < MAX_HEARTS; i++) {
        if (i < h) s += "<3 ";
        else s += "x  ";
    }
    return s;
}

void status() {
    cout << "Hearts: " << heartsStr(hearts) << "  |  Points: " << points << "\n";
}

// problems 1-3 easy, 4-7 medium, 8-10 hard
int difficultyPoints(int idx, string& label) {
    if (idx < 3) { label = "EASY"; return 10; }
    if (idx < 7) { label = "MEDIUM"; return 15; }
    label = "HARD";
    return 30;
}

bool spend(int cost) {
    if (points < cost) {
        cout << "Not enough points.\n";
        return false;
    }
    points -= cost;
    totalSpent += cost;
    return true;
}

// used when the player is at 0 hearts and pays for a heart
void buyHeartAtZero() {
    points -= HEART_COST;
    totalSpent += HEART_COST;
    hearts = 1;
}

void restartGame() {
    hearts = MAX_HEARTS;
    points = 0;
    totalEarned = 0;
    totalSpent = 0;
    for (int i = 0; i < NUM_UPGRADES; i++) upgrades[i] = 0;
    unlocked = 1;
    for (int i = 0; i < NUM_MAPS; i++) completed[i] = false;
    cout << "\nGame restarted! Good luck!\n";
}

// mixes up the numbers in an array
void shuffleArray(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Shows a question with the choices shuffled and returns the choice
// the player picked (as the original index, not the shuffled position).
int askQuestion(string text, string options[], int count, int right, string goodText) {
    int order[4];
    for (int i = 0; i < count; i++) order[i] = i;
    shuffleArray(order, count);

    cout << "\n" << text << "\n\n";
    for (int i = 0; i < count; i++) {
        cout << "  " << (i + 1) << ") " << options[order[i]] << "\n";
    }
    cout << "\nWhat do you do?\n";

    int pick = order[getChoice(1, count) - 1];

    cout << "\n";
    if (pick == right) cout << "*** GOOD DECISION! ***\n" << goodText << "\n";
    else cout << "*** BAD DECISION! ***\n" << BAD_TEXT << "\n";
    return pick;
}

// recovery = true means it's practice and no heart can be lost
void playProblem(int m, int idx, bool recovery) {
    string difficulty;
    int basePoints = difficultyPoints(idx, difficulty);

    Question q = QUESTIONS[m][idx];
    MapSpec spec = MAPSPEC[m];

    cout << "\n";
    printLine();
    if (recovery) cout << "RECOVERY PRACTICE (no hearts at risk)\n";
    cout << MAP_NAMES[m] << " - Problem " << (idx + 1) << "/" << NUM_PROBLEMS
         << "  [" << difficulty << " - " << basePoints << " pts]\n";
    status();
    printLine();

    bool isSolved = false;
    int firstPick = 0;    // wrong answer in question 1
    int secondPick = 0;   // wrong answer in question 2

    for (int stage = 0; stage < 3; stage++) {
        string text;
        string options[4];
        int count;
        int right;

        if (stage == 0) {
            text = q.text;
            count = 4;
            for (int i = 0; i < 4; i++) options[i] = q.opt[i];
            right = q.right;
        } else if (stage == 1) {
            cout << "\n~~~ BUTTERFLY EFFECT (2/3) ~~~\n";
            cout << "Your earlier decision has caused a new problem...\n";
            text = spec.q2Texts[q.var[firstPick]];
            count = 3;
            for (int i = 0; i < 3; i++) options[i] = spec.q2Options[i];
            right = 0;
        } else {
            cout << "\n~~~ BUTTERFLY EFFECT (3/3) ~~~\n";
            cout << "Your earlier decision has caused a new problem...\n";
            count = 3;
            right = 2;
            if (secondPick == 1) {
                text = spec.q3AfterB;
                for (int i = 0; i < 3; i++) options[i] = AFTER_B_OPTIONS[i];
            } else {
                text = spec.q3AfterC;
                for (int i = 0; i < 3; i++) options[i] = AFTER_C_OPTIONS[i];
            }
        }

        int pick = askQuestion(text, options, count, right, q.good);

        if (pick == right) {
            // same points no matter which question is answered correctly
            int gain = basePoints;
            points += gain;
            totalEarned += gain;
            cout << "\n+" << gain << " points! The problem is solved.\n";
            isSolved = true;
            break;
        }

        if (stage == 0) firstPick = pick;
        if (stage == 1) secondPick = pick;
        if (stage == 2) break;
        waitForEnter();
    }

    if (recovery) {
        if (!isSolved) {
            cout << "\nNot quite. The best first move would have been: "
                 << q.opt[q.right] << "\n";
        }
    } else if (!isSolved) {
        hearts--;
        cout << "\nThe problem snowballed out of control after 3 bad decisions...\n";
        cout << "The best first move would have been: " << q.opt[q.right] << "\n";
        cout << "You lost a heart!  " << heartsStr(hearts) << "\n";
    }
}

int endOrRestart() {
    cout << "\n############################################\n";
    cout << "   GAME OVER\n";
    cout << "############################################\n";
    cout << "  1) End the game\n  2) Restart the game\n";
    if (getChoice(1, 2) == 2) {
        restartGame();
        return MAP_RESTART;
    }
    return MAP_END;
}

// Called when hearts hit 0 on problem idx of map m.
// Returns MAP_DONE if the player got a heart back and should retry the problem.
int outOfHearts(int m, int idx) {
    cout << "\n############################################\n";
    cout << "   You have run out of hearts!\n";
    cout << "############################################\n";

    // enough points already, so they can just buy a heart
    if (points >= HEART_COST) {
        cout << "You have " << points << " points - enough to buy a heart ("
             << HEART_COST << " pts).\n";
        cout << "  1) Buy a heart and retry this problem\n";
        cout << "  2) End the game\n";
        cout << "  3) Restart the game\n";
        int choice = getChoice(1, 3);
        if (choice == 1) {
            buyHeartAtZero();
            cout << "Heart bought! " << heartsStr(hearts) << "\n";
            return MAP_DONE;
        }
        if (choice == 3) {
            restartGame();
            return MAP_RESTART;
        }
        return MAP_END;
    }

    // not enough points, so go back and practice older questions
    cout << "You have no hearts and only " << points << " points (a heart costs "
         << HEART_COST << ").\n";
    cout << "Go back to your earlier questions and earn points.\n";
    cout << "Once you reach " << HEART_COST << " points you will automatically buy a heart\n";
    cout << "and return to the problem you were on.\n";

    // questions to practice: all problems of finished maps + earlier ones on this map
    int earlyMap[100];
    int earlyProb[100];
    int total = 0;
    for (int k = 0; k < NUM_MAPS; k++) {
        if (k != m && completed[k]) {
            for (int i = 0; i < NUM_PROBLEMS; i++) {
                earlyMap[total] = k;
                earlyProb[total] = i;
                total++;
            }
        }
    }
    for (int i = 0; i < idx; i++) {
        earlyMap[total] = m;
        earlyProb[total] = i;
        total++;
    }

    // failed the very first problem so nothing is earlier, use the other ones
    if (total == 0) {
        for (int i = 0; i < NUM_PROBLEMS; i++) {
            if (i != idx) {
                earlyMap[total] = m;
                earlyProb[total] = i;
                total++;
            }
        }
    }

    // shuffle the list of questions
    int order[100];
    for (int i = 0; i < total; i++) order[i] = i;
    shuffleArray(order, total);

    int next = 0;
    while (true) {
        waitForEnter();

        // went through all of them, shuffle and start again
        if (next >= total) {
            shuffleArray(order, total);
            next = 0;
        }
        int pickedMap = earlyMap[order[next]];
        int pickedProb = earlyProb[order[next]];
        next++;

        cout << "\n>>> Going back to an earlier question... (" << points << "/"
             << HEART_COST << " points to buy a heart)\n";
        playProblem(pickedMap, pickedProb, true);

        if (points >= HEART_COST) {
            buyHeartAtZero();
            cout << "\n*** You reached " << HEART_COST << " points! A heart was bought for you. ***\n";
            cout << "Hearts: " << heartsStr(hearts) << "  Returning to your current problem...\n";
            return MAP_DONE;
        }

        cout << "\nPoints: " << points << "/" << HEART_COST
             << ".  1) Keep going  2) Give up\n";
        if (getChoice(1, 2) == 2) return endOrRestart();
    }
}

int playMap(int m) {
    cout << "\n============================================================\n";
    cout << "  MAP " << (m + 1) << ": " << MAP_NAMES[m] << "\n";
    cout << "============================================================\n";
    cout << "You will face " << NUM_PROBLEMS << " problems (3 easy, 4 medium, 3 hard).\n";
    cout << "Pick the best decision. Bad choices cause a chain of new problems!\n";
    waitForEnter();

    int i = 0;
    while (i < NUM_PROBLEMS) {
        playProblem(m, i, false);

        if (hearts <= 0) {
            int result = outOfHearts(m, i);
            if (result != MAP_DONE) return result;
            waitForEnter();
            // i stays the same so they retry this problem
        } else {
            waitForEnter();
            i++;
        }
    }

    completed[m] = true;
    cout << "\n*** You completed the " << MAP_NAMES[m] << " map! ***\n";
    status();
    return MAP_DONE;
}

int totalUpgrades() {
    int n = 0;
    for (int i = 0; i < NUM_UPGRADES; i++) n += upgrades[i];
    return n;
}

// only the upgrades of maps the player has completed are shown
void upgradeMenu() {
    while (true) {
        int available[NUM_UPGRADES];
        int count = 0;
        for (int i = 0; i < NUM_UPGRADES; i++) {
            if (completed[i]) {
                available[count] = i;
                count++;
            }
        }

        cout << "\n------------- UPGRADE THE ENVIRONMENT -------------\n";
        status();

        if (count == 0) {
            cout << "Finish a map first to unlock its environment upgrade.\n";
            return;
        }

        for (int k = 0; k < count; k++) {
            int i = available[k];
            cout << "  " << (k + 1) << ") " << upgradeName[i] << " - " << upgradeCost[i]
                 << " pts  (owned: " << upgrades[i] << ")\n";
        }
        cout << "  " << (count + 1) << ") Back\n";

        int choice = getChoice(1, count + 1);
        if (choice == count + 1) return;

        int item = available[choice - 1];
        if (spend(upgradeCost[item])) {
            upgrades[item]++;
            cout << "You bought: " << upgradeName[item] << " - it "
                 << upgradeImpact[item] << ".\n";
        }
    }
}

void shop() {
    while (true) {
        int missing = MAX_HEARTS - hearts;

        cout << "\n=================== SHOP ===================\n";
        status();
        cout << "  1) Restore 1 heart             - " << HEART_COST << " pts\n";
        cout << "  2) Restore all missing hearts  - " << (missing * HEART_COST) << " pts\n";
        cout << "  3) Upgrade the environment     (plant a tree, recycling bins, crop sheds...)\n";
        if (unlocked < NUM_MAPS) {
            cout << "  4) Unlock next map (" << MAP_NAMES[unlocked] << ") - " << UNLOCK_COST << " pts";
            if (upgrades[unlocked - 1] == 0)
                cout << "  [requires: " << upgradeName[unlocked - 1] << "]";
            cout << "\n";
        } else {
            cout << "  4) Unlock next map             - (all maps unlocked)\n";
        }
        cout << "  5) Leave the shop\n";

        int choice = getChoice(1, 5);

        if (choice == 1) {
            if (hearts >= MAX_HEARTS) {
                cout << "Your hearts are already full!\n";
            } else if (spend(HEART_COST)) {
                hearts++;
                cout << "Heart restored! " << heartsStr(hearts) << "\n";
            }
        } else if (choice == 2) {
            if (missing == 0) {
                cout << "Your hearts are already full!\n";
            } else if (spend(missing * HEART_COST)) {
                hearts = MAX_HEARTS;
                cout << "All hearts restored! " << heartsStr(hearts) << "\n";
            }
        } else if (choice == 3) {
            upgradeMenu();
        } else if (choice == 4) {
            if (unlocked >= NUM_MAPS) {
                cout << "Every map is already unlocked.\n";
            } else if (upgrades[unlocked - 1] == 0) {
                cout << "You must first buy \"" << upgradeName[unlocked - 1]
                     << "\" in 'Upgrade the environment' before unlocking the next map.\n";
            } else if (spend(UNLOCK_COST)) {
                cout << "Unlocked: " << MAP_NAMES[unlocked] << "!\n";
                unlocked++;
            }
        } else {
            return;
        }
    }
}

bool allDone() {
    for (int i = 0; i < NUM_MAPS; i++) {
        if (!completed[i]) return false;
    }
    return true;
}

bool hasPlayableMap() {
    for (int i = 0; i < unlocked; i++) {
        if (!completed[i]) return true;
    }
    return false;
}

void showStats() {
    int mapsDone = 0;
    for (int i = 0; i < NUM_MAPS; i++) {
        if (completed[i]) mapsDone++;
    }

    cout << "\n============================================================\n";
    cout << "                 YOUR ENVIRONMENTAL PROGRESS\n";
    cout << "============================================================\n";
    cout << "Maps completed        : " << mapsDone << "/" << NUM_MAPS << "\n";
    for (int i = 0; i < NUM_MAPS; i++) {
        string state;
        if (completed[i]) state = "Completed";
        else if (i < unlocked) state = "Unlocked";
        else state = "Locked";
        cout << "   - " << MAP_NAMES[i] << ": " << state << "\n";
    }
    cout << "Total points earned   : " << totalEarned << "\n";
    cout << "Points spent          : " << totalSpent << "\n";
    cout << "Points saved          : " << points << "\n";
    cout << "Upgrades bought   : " << totalUpgrades() << "\n";
    for (int i = 0; i < NUM_UPGRADES; i++) {
        if (upgrades[i] > 0) {
            cout << "   - " << upgradeName[i] << " x" << upgrades[i] << "\n";
        }
    }
    cout << "Hearts left           : " << hearts << "/" << MAX_HEARTS << "\n";
    cout << "============================================================\n";
    cout << "Thank you for playing! Small choices make a big difference.\n";
}

void intro() {
    cout << "\n============================================================\n";
    cout << "                      N A T U R E   Q U E S T\n";
    cout << "============================================================\n";
    cout << "Every decision has a ripple effect on the environment.\n\n";
    cout << " * You start with " << MAX_HEARTS << " hearts.\n";
    cout << " * Good decisions earn points (Easy 10 / Medium 15 / Hard 30).\n";
    cout << " * A bad decision leads to a new problem (up to 3 in a row).\n";
    cout << "   Get all 3 wrong and you lose a heart.\n";
    cout << " * Spend points in the shop to restore hearts, upgrade the\n";
    cout << "   environment, or unlock the next map.\n";
    cout << " * You must upgrade the environment of a finished map before\n";
    cout << "   you can unlock the next one.\n";
    cout << "============================================================\n";
    waitForEnter();
}

int chooseMap() {
    while (true) {
        cout << "\n------------------- CHOOSE A MAP -------------------\n";
        for (int i = 0; i < NUM_MAPS; i++) {
            cout << "  " << (i + 1) << ") " << MAP_NAMES[i] << "  ";
            if (completed[i]) cout << "[COMPLETED]";
            else if (i < unlocked) cout << "[AVAILABLE]";
            else cout << "[LOCKED]";
            cout << "\n";
        }
        cout << "  " << (NUM_MAPS + 1) << ") Visit the shop\n";
        status();

        int choice = getChoice(1, NUM_MAPS + 1) - 1;

        if (choice == NUM_MAPS) {
            shop();
        } else if (completed[choice]) {
            cout << "You already completed that map.\n";
        } else if (choice >= unlocked) {
            cout << "That map is locked. Buy it in the shop!\n";
        } else {
            return choice;
        }
    }
}

int main() {
    srand((unsigned)time(0));

    intro();

    while (true) {
        if (allDone()) {
            cout << "\nCongratulations! You finished every map!\n";
            break;
        }

        // every unlocked map is finished, need to buy the next one
        if (!hasPlayableMap()) {
            cout << "\nAll unlocked maps are finished. You need to unlock the next one.\n";

            // the upgrade of the last finished map is needed before unlocking
            int needed = UNLOCK_COST;
            if (upgrades[unlocked - 1] == 0) needed += UPGRADE_COST;

            if (points >= needed) {
                shop();
                if (!hasPlayableMap()) {
                    cout << "No new map was unlocked, so the game ends here.\n";
                    break;
                }
            } else {
                cout << "You do not have enough points (" << needed
                     << " needed) to unlock another map.\n";
                break;
            }
        }

        int m = chooseMap();
        int result = playMap(m);
        if (result == MAP_RESTART) continue;
        if (result == MAP_END) break;

        cout << "\nWhat would you like to do with your " << points << " points?\n";
        cout << "  1) Spend them (open the shop)\n  2) Save them\n";
        if (getChoice(1, 2) == 1) shop();

        if (allDone()) {
            cout << "\nYou have completed every map!\n";
            break;
        }

        cout << "\nContinue playing or finish the game?\n";
        cout << "  1) Continue\n  2) Finish and see my stats\n";
        if (getChoice(1, 2) == 2) break;
    }

    showStats();
    return 0;
}
