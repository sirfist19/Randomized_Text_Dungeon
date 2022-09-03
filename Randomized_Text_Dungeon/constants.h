#ifndef constants_h
#define constant_h

const bool DEBUG_MODE = false;
const int MAX_DUNGEON_SIZE = 35;
const int MIN_DUNGEON_SIZE = 25;
const int MAX_CHAR_PER_LINE = 120;

//depth tiers
const int MIN_NEAR = 1;
const int MAX_NEAR = 3;
const int MIN_MID = MAX_NEAR + 1;//4
const int MAX_MID = 7;
const int MIN_FAR = MAX_MID + 1;//8
const int MAX_FAR = 10;
const int MIN_VERY_FAR = MAX_FAR + 1;//11
const int MAX_VERY_FAR = 100;


//spawn rates (out of 100)
//near 35
const int NEAR_SPAWN_RAT = 25;
const int NEAR_SPAWN_SLIME = 15;

//mid 60
const int MID_SPAWN_RAT = 10;
const int MID_SPAWN_SLIME = 5;
const int MID_SPAWN_SKELETON = 35;
const int MID_SPAWN_GOBLIN = 10;

//far 60
const int FAR_SPAWN_RAT = 3;
const int FAR_SPAWN_SLIME = 1;
const int FAR_SPAWN_SKELETON = 20;
const int FAR_SPAWN_GOBLIN = 6;
const int FAR_SPAWN_ORC = 30;

//very far 56
const int VERY_FAR_SPAWN_RAT = 1;
const int VERY_FAR_SPAWN_PHANTOM_KNIGHT = 25;
const int VERY_FAR_SPAWN_ORC = 30;

//chest spawning 
const int NEAR_SPAWN_WOOD_CHEST = 100;// 10;
const int MID_SPAWN_WOOD_CHEST = 95;//25;
const int MID_SPAWN_GOLD_CHEST = 5;
const int FAR_SPAWN_GOLD_CHEST = 40;
const int VERY_FAR_SPAWN_DRAGON_CHEST = 10;
const int VERY_FAR_SPAWN_GOLD_CHEST = 25;


//chest item spawning (out of 100)
const int CHEST_WEAPON_SPAWN_RATE = 50;
//wooden chest
const int WOOD_CHEST_LESSER_HEALING_SPAWN_RATE = 40;
const int WOOD_CHEST_SPAWN_GOLD_RATE = 30;
const int WOOD_CHEST_MAX_GOLD = 7;
const int WOOD_CHEST_MIN_GOLD = 1;

//gold chest
//dragon chest
const int GOLD_CHEST_LESSER_HEALING_SPAWN_RATE = 10;
const int GOLD_CHEST_COMMON_HEALING_SPAWN_RATE = 50;
const int GOLD_CHEST_SPAWN_GOLD_RATE = 60;
const int GOLD_CHEST_MAX_GOLD = 25;
const int GOLD_CHEST_MIN_GOLD = 10;

//dragon chest
const int DRAGON_CHEST_LESSER_HEALING_SPAWN_RATE = 5;
const int DRAGON_CHEST_COMMON_HEALING_SPAWN_RATE = 20;
const int DRAGON_CHEST_GREATER_HEALING_SPAWN_RATE = 50;
const int DRAGON_CHEST_SPAWN_GOLD_RATE = 90;
const int DRAGON_CHEST_MAX_GOLD = 75;
const int DRAGON_CHEST_MIN_GOLD = 25;

#endif
