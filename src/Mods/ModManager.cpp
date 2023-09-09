#include "ModManager.hpp"

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>

#include <string>

#include "../Items/Item.hpp"
#include "../Machines/MachineData.hpp"
#include "../Recipes/Recipe.hpp"
#include "../Storages/StorageData.hpp"
#include "../ConveyorBelts/ConveyorBeltData.hpp"

std::vector<ModInfo> ModManager::mods;

void ModManager::_bind_methods() {
    ClassDB::bind_static_method("ModManager", D_METHOD("load_mods", "mod_paths"), &ModManager::load_mods);

    ClassDB::bind_static_method("ModManager", D_METHOD("get_mods"), &ModManager::get_mods);
}

void ModManager::load_mods(const PackedStringArray& mod_paths) {
    // load PCK
    for (String path : mod_paths)
        ProjectSettings::get_singleton()->load_resource_pack(path);
    
    PackedStringArray mod_names = DirAccess::get_directories_at("res://mods/");

    // load ModInfos
    for (String mod_name : mod_names)
        mods.emplace_back("res://mods/" + mod_name);
    
    // load Items
    for (String mod_name : mod_names)
        Item::load_ItemStore("res://mods/" + mod_name);

    // load Machines
    for (String mod_name : mod_names)
        MachineData::load_MachineStore("res://mods/" + mod_name);

    // load Recipes
    for (String mod_name : mod_names)
        Recipe::load_RecipeBook("res://mods/" + mod_name);

    // load Storages
    for (String mod_name : mod_names)
        StorageData::load_StorageHall("res://mods/" + mod_name);
    
    // load ConveyorBelts
    for (String mod_name : mod_names)
        ConveyorBeltData::load_ConveyorBeltWarehouse("res://mods/" + mod_name);
}

Array ModManager::get_mods() {
    Array mods_arr;

    for (ModInfo modinfo : mods) {
        Dictionary mod_dict;
        mod_dict["path"] = modinfo.path;
        mod_dict["name"] = modinfo.name;
        mod_dict["description"] = modinfo.description;
        mod_dict["authors"] = modinfo.authors;
        mod_dict["version"] = modinfo.version;

        mods_arr.append(mod_dict);
    }

    return mods_arr;
}