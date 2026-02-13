#include "static_objects_db.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "nlohmann/json.hpp"

std::unordered_map<std::string, StaticObjectDef> StaticObjectsDB::defs;

void StaticObjectsDB::load(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        throw std::runtime_error("StaticObjectsDB::load failed to open: " + path);
    }

    nlohmann::json j;
    f >> j;

    // Format:
    // {
    //   "static.pit": { "name": "...", "description": "...", "messages": { ... } },
    //   ...
    // }
    if (!j.is_object()) {
        throw std::runtime_error("static_objects.json must be a JSON object keyed by static object id.");
    }

    defs.clear();

    for (auto it = j.begin(); it != j.end(); ++it) {
        const std::string id = it.key();
        const nlohmann::json& obj = it.value();

        if (!obj.is_object()) {
            throw std::runtime_error("Static object entry '" + id + "' must be an object.");
        }

        // Required: name + description
        if (!obj.contains("name") || !obj.at("name").is_string()) {
            throw std::runtime_error("Static object '" + id + "' missing string field: name");
        }
        if (!obj.contains("description") || !obj.at("description").is_string()) {
            throw std::runtime_error("Static object '" + id + "' missing string field: description");
        }

        StaticObjectDef def;
        def.id = id;
        def.name = obj.at("name").get<std::string>();
        def.description = obj.at("description").get<std::string>();

        // Optional: messages object
        if (obj.contains("messages")) {
            const auto& msgs = obj.at("messages");
            if (!msgs.is_object()) {
                throw std::runtime_error("Static object '" + id + "' field 'messages' must be an object.");
            }

            for (auto mit = msgs.begin(); mit != msgs.end(); ++mit) {
                const std::string msg_key = mit.key();
                const nlohmann::json& msg_val = mit.value();

                // We allow empty object {} (like your dining_table) with no entries.
                if (!msg_val.is_string()) {
                    throw std::runtime_error("Static object '" + id + "' message '" + msg_key + "' must be a string.");
                }

                def.messages.emplace(msg_key, msg_val.get<std::string>());
            }
        }

        defs.emplace(id, std::move(def));
    }
}

const StaticObjectDef& StaticObjectsDB::get(const std::string& id) {
    auto it = defs.find(id);
    if (it == defs.end()) {
        throw std::runtime_error("StaticObjectsDB::get unknown id: " + id);
    }
    return it->second;
}

std::string StaticObjectsDB::message_or_empty(const std::string& id, const std::string& msg_key) {
    const StaticObjectDef& def = get(id);
    auto it = def.messages.find(msg_key);
    if (it == def.messages.end()) return "";
    return it->second;
}

const std::unordered_map<std::string, StaticObjectDef>& StaticObjectsDB::all() {
    return defs;
}

void StaticObjectsDB::display() {
    std::cout << "=========================\n";
    std::cout << " StaticObjectsDB Contents\n";
    std::cout << " Loaded Objects: " << defs.size() << "\n";
    std::cout << "=========================\n\n";

    for (const auto& [id, def] : defs) {
        std::cout << "ID: " << def.id << "\n";
        std::cout << "Name: " << def.name << "\n";
        std::cout << "Description:\n";
        std::cout << "  " << def.description << "\n";

        std::cout << "Messages (" << def.messages.size() << "):\n";
        if (def.messages.empty()) {
            std::cout << "  (none)\n";
        } else {
            for (const auto& [k, v] : def.messages) {
                std::cout << "  " << k << " => " << v << "\n";
            }
        }

        std::cout << "-------------------------\n";
    }

    std::cout << "\nEnd of StaticObjectsDB display.\n";
}
