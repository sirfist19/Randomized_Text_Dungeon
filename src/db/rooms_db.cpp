#include "rooms_db.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <iostream>
// nlohmann/json single-header library
#include "nlohmann/json.hpp"

std::unordered_map<std::string, RoomDef> RoomsDB::defs;


std::vector<std::string> RoomsDB::ids_for_tier(const std::string& tier) {
    std::vector<std::string> out;
    out.reserve(defs.size());

    for (const auto& [id, def] : defs) {
        // allow "any" tier to be used as a wildcard pool for non-start rooms if you want
        if (def.tier == tier) out.push_back(id);
    }
    return out;
}

void RoomsDB::load(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        throw std::runtime_error("RoomsDB::load failed to open: " + path);
    }

    nlohmann::json j;
    f >> j;

    // rooms.json format should be:
    // {
    //   "room.library": { "name": "...", "description": "...", "tier": "...", "biome": "any" },
    //   ...
    // }
    if (!j.is_object()) {
        throw std::runtime_error("rooms.json must be a JSON object keyed by room id.");
    }

    defs.clear();

    for (auto it = j.begin(); it != j.end(); ++it) {
        const std::string id = it.key();
        const nlohmann::json& obj = it.value();

        if (!obj.is_object()) {
            throw std::runtime_error("Room entry '" + id + "' must be an object.");
        }

        // Name parsing
        if (!obj.contains("name") || !obj.at("name").is_string()) {
            throw std::runtime_error("Room '" + id + "' missing string field: name");
        }
        // Description parsing
        if (!obj.contains("description") || !obj.at("description").is_string()) {
            throw std::runtime_error("Room '" + id + "' missing string field: description");
        }

        RoomDef def;
        def.id = id;
        def.name = obj.at("name").get<std::string>();
        def.description = obj.at("description").get<std::string>();

        // Tier parsing
        if (obj.contains("tier") && obj.at("tier").is_string()) {
            def.tier = obj.at("tier").get<std::string>();
        } else {
            def.tier = "";
        }

        // Biome parsing
        if (obj.contains("biome") && obj.at("biome").is_string()) {
            def.biome = obj.at("biome").get<std::string>();
        } else {
            def.biome = "any"; // default if missing
        }
        
        // Static objects 
        def.object_ids.clear(); 
        if (obj.contains("objects")) {
            const auto& arr = obj.at("objects");
            if (!arr.is_array()) {
                throw std::runtime_error("Room '" + id + "' field 'objects' must be an array.");
            }
            for (const auto& v : arr) {
                if (!v.is_string()) {
                    throw std::runtime_error("Room '" + id + "' objects entries must be strings.");
                }
                def.object_ids.push_back(v.get<std::string>());
            }
        }

        defs.emplace(id, std::move(def));
    }
}

const RoomDef& RoomsDB::get(const std::string& id) {
    auto it = defs.find(id);
    if (it == defs.end()) {
        throw std::runtime_error("RoomsDB::get unknown room id: " + id);
    }
    return it->second;
}

const std::unordered_map<std::string, RoomDef>& RoomsDB::all() {
    return defs;
}

void RoomsDB::display() {
    std::cout << "=========================\n";
    std::cout << " RoomsDB Contents\n";
    std::cout << " Loaded Rooms: " << defs.size() << "\n";
    std::cout << "=========================\n\n";

    for (const auto& [id, def] : defs) {
        std::cout << "ID: " << def.id << "\n";
        std::cout << "Name: " << def.name << "\n";
        std::cout << "Tier: " << (def.tier.empty() ? "(none)" : def.tier) << "\n";
        std::cout << "Biome: " << def.biome << "\n";

        std::cout << "Description:\n";
        std::cout << "  " << def.description << "\n";

        std::cout << "-------------------------\n";
    }

    std::cout << "\nEnd of RoomsDB display.\n";
}
