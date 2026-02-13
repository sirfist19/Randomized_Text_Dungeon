#ifndef static_objects_db_h
#define static_objects_db_h

#include <string>
#include <unordered_map>

struct StaticObjectDef {
    std::string id;          // e.g. "static.pit"
    std::string name;        // e.g. "Pit"
    std::string description; // base description text

    // Optional messages like "jump_death", "item_on_pedestal"
    std::unordered_map<std::string, std::string> messages;
};

class StaticObjectsDB {
public:
    // Load once at startup (e.g., in main()).
    static void load(const std::string& path);

    // Lookup by id. Throws if missing.
    static const StaticObjectDef& get(const std::string& id);

    // Optional: returns empty string if message key is missing.
    static std::string message_or_empty(const std::string& id, const std::string& msg_key);

    // Debug: print all loaded static objects.
    static void display();

    // Access all definitions.
    static const std::unordered_map<std::string, StaticObjectDef>& all();

private:
    static std::unordered_map<std::string, StaticObjectDef> defs;
};
#endif
