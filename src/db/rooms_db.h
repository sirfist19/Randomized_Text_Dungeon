#ifndef rooms_db_h
#define rooms_db_h

#include <string>
#include <unordered_map>
#include <vector>



struct RoomDef {
    std::string id;          // "room.library"
    std::string name;        // "Library"
    std::string description; // flavor text
    std::string tier;        // "start"/"near"/... or "" if missing
    std::string biome;       // e.g. "any"
    // IDs of static objects that should appear in the room
    std::vector<std::string> object_ids; // e.g. {"static.pit", "static.moss"}
};

class RoomsDB {
public:
    // Load once at program start (e.g., in main()).
    static void load(const std::string& path);

    // Lookup by id. Throws if missing.
    static const RoomDef& get(const std::string& id);

    // Optional: access all rooms (useful for filtering / random selection).
    static const std::unordered_map<std::string, RoomDef>& all();

    static std::vector<std::string> ids_for_tier(const std::string& tier);

    static void display();
private:
    static std::unordered_map<std::string, RoomDef> defs;
};
#endif
