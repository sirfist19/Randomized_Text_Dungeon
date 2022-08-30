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
const int NEAR_SPAWN_SLIME = 10;

//mid 42
const int MID_SPAWN_RAT = 10;
const int MID_SPAWN_SLIME = 2;
const int MID_SPAWN_SKELETON = 20;
const int MID_SPAWN_GOBLIN = 10;

//far 35
const int FAR_SPAWN_RAT = 3;
const int FAR_SPAWN_SLIME = 1;
const int FAR_SPAWN_SKELETON = 10;
const int FAR_SPAWN_GOBLIN = 6;
const int FAR_SPAWN_ORC = 15;

//very far 41
const int VERY_FAR_SPAWN_RAT = 1;
const int VERY_FAR_SPAWN_PHANTOM_KNIGHT = 20;
const int VERY_FAR_SPAWN_ORC = 20;


#endif
