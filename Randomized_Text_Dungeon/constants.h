#ifndef constants_h
#define constant_h
#include "helper_fxns.h"

const bool DEBUG_MODE = false;
const bool DISABLE_CLEARING_SCREEN = false;
const bool DISPLAY_PARSED_INFO = false;
const int MAX_DUNGEON_SIZE = 50;//50
const int MIN_DUNGEON_SIZE = 40;//40
const int MAX_CHAR_PER_LINE = 120;//120
const int CONNECT_ROOMS_CHANCE = 15;//15
const int MIN_DEEPEST_DEPTH = 17;//17
const int PLAYER_START_GOLD_AMT = 0;

const int RUN_AWAY_CHANCE = 50;
const int BORDER_MAP_SIZE = 45;
const int ENEMY_DROP_ITEM_CHANCE = 25;
const int ENEMY_DROP_GOLD_CHANCE = 100;
const int ENEMY_PICKS_HEAVY_ATTACK_CHANCE = 55;
const int COMPASS_SPAWN_DEPTH = 5;
const int TELEPORTER_SPAWN_DEPTH = 5;//x less than the dragon room depth
const double DEFENSE_MULTIPLIER_ATTACK_CALCULATION = 1.8;

const int GOLD_SPAWN_IN_MOSS = 50;


//Store
const int STORE_SPAWN_DEPTH = 12;
const int STORE_NUM_DASHES = 100;
//attacking 
const double HEAVY_DAMAGE_MULTIPLIER = 1.5;
const int HEAVY_HIT_RATE_REDUCER = 40;

//depth tiers
const int MIN_START_AREA = 0;
const int MAX_START_AREA = 1;

const int MIN_NEAR = 2;
const int MAX_NEAR = 4;

const int MIN_MID = 5;
const int MAX_MID = 9;

const int MIN_FAR = 10;
const int MAX_FAR = 15;

const int MIN_VERY_FAR = 16;
const int MAX_VERY_FAR = 100;

//spawn rates (out of 100)
//near 35
const int NEAR_SPAWN_RAT = 25;
const int NEAR_SPAWN_SLIME = 50;

//mid 60
const int MID_SPAWN_RAT = 10;
const int MID_SPAWN_SLIME = 10;
const int MID_SPAWN_SKELETON = 30;
const int MID_SPAWN_GOBLIN = 10;

//far 65
const int FAR_SPAWN_RAT = 5;
const int FAR_SPAWN_SLIME = 5;
const int FAR_SPAWN_SKELETON = 20;
const int FAR_SPAWN_GOBLIN = 5;
const int FAR_SPAWN_ORC = 30;

//very far 60
const int VERY_FAR_SPAWN_RAT = 5;
const int VERY_FAR_SPAWN_PHANTOM_KNIGHT = 15;
const int VERY_FAR_SPAWN_ORC = 40;

//chest spawning 
//start (rooms of depth 1)
const int START_SPAWN_WOOD_CHEST = 90;
const int NEAR_SPAWN_WOOD_CHEST = 15;// 30;
const int MID_SPAWN_WOOD_CHEST = 10;//25;
const int MID_SPAWN_GOLD_CHEST = 5;
const int FAR_SPAWN_GOLD_CHEST = 15;
const int VERY_FAR_SPAWN_DRAGON_CHEST = 20;
const int VERY_FAR_SPAWN_GOLD_CHEST = 2;


//chest item spawning (out of 100)
const int CHEST_WEAPON_SPAWN_RATE = 70;
const int CHEST_ARMOR_SPAWN_RATE = 90;

//wooden chest
const int WOOD_CHEST_LESSER_HEALING_SPAWN_RATE = 60;
const int WOOD_CHEST_SPAWN_GOLD_RATE = 75;
const int WOOD_CHEST_MAX_GOLD = 7;
const int WOOD_CHEST_MIN_GOLD = 3;

//gold chest
//dragon chest
const int GOLD_CHEST_LESSER_HEALING_SPAWN_RATE = 20;
const int GOLD_CHEST_COMMON_HEALING_SPAWN_RATE = 50;
const int GOLD_CHEST_SPAWN_GOLD_RATE = 85;
const int GOLD_CHEST_MAX_GOLD = 25;
const int GOLD_CHEST_MIN_GOLD = 12;

//dragon chest
const int DRAGON_CHEST_LESSER_HEALING_SPAWN_RATE = 5;
const int DRAGON_CHEST_COMMON_HEALING_SPAWN_RATE = 20;
const int DRAGON_CHEST_GREATER_HEALING_SPAWN_RATE = 60;
const int DRAGON_CHEST_SPAWN_GOLD_RATE = 99;
const int DRAGON_CHEST_MAX_GOLD = 75;
const int DRAGON_CHEST_MIN_GOLD = 40;


//armor defense
const int CHAINMAIL_HELMET_DEFENSE = 1;
const int CHAINMAIL_CHESTPLATE_DEFENSE = 2;
const int CHAINMAIL_BOOTS_DEFENSE = 1;

const int BRONZE_HELMET_DEFENSE = 3;
const int BRONZE_CHESTPLATE_DEFENSE = 5;
const int BRONZE_BOOTS_DEFENSE = 4;

const int SILVER_HELMET_DEFENSE = 5;
const int SILVER_CHESTPLATE_DEFENSE = 7;
const int SILVER_BOOTS_DEFENSE = 6;

const int DRAGON_SCALE_HELMET_DEFENSE = 10;
const int DRAGON_SCALE_CHESTPLATE_DEFENSE = 15;
const int DRAGON_SCALE_BOOTS_DEFENSE = 13;

#endif
