#include "StorageHall.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "StorageData.hpp"
#include "../Utils/BuildingUtility.hpp"

void StorageHall::_bind_methods() {
    ClassDB::bind_static_method("StorageHall", D_METHOD("get_storages"), &StorageHall::get_storages);

    ClassDB::bind_static_method("StorageHall", D_METHOD("instantiate", "storage_name"), &StorageHall::instantiate);
    ClassDB::bind_static_method("StorageHall", D_METHOD("get_building", "storage_name"), &StorageHall::get_building);
}

Array StorageHall::get_storages() {
    Array storages_arr;

    for (const auto& pair : StorageData::storage_instances) {
        StorageData* storage = pair.second;
        Dictionary storage_dict;

        storage_dict["name"] = storage->name;
        storage_dict["hash"] = storage->hash;
        storage_dict["slot_count"] = storage->slot_count;

        Array output_positions;
        for (Vector3 pos : storage->output_positions)
            output_positions.append(pos);
        storage_dict["output_positions"] = output_positions;

        storages_arr.append(storage_dict);
    }

    return storages_arr;
}

Storage* StorageHall::instantiate(const String& storage_name) {
    const auto it = StorageData::storage_instances.find(storage_name.hash());
    if (it == StorageData::storage_instances.end()) {
        UtilityFunctions::printerr("Can't find Storage: ", storage_name);
        return nullptr;
    }

    return Object::cast_to<Storage>(it->second->instantiate());
}

Building* StorageHall::get_building(const String& storage_name) {
    const auto it = StorageData::storage_instances.find(storage_name.hash());
    if (it == StorageData::storage_instances.end()) {
        UtilityFunctions::printerr("Can't find Storage: ", storage_name);
        return nullptr;
    }

    Building* building = memnew(Building());
    building->add_child(it->second->node->duplicate());

    building->hash = it->second->hash;
    building->base = it->second;

    building->prepare_for_building();
    return building;
}