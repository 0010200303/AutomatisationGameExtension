#include "MachineStore.hpp"

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "MachineData.hpp"
#include "../Utils/BuildingUtility.hpp"

void MachineStore::_bind_methods() {
    ClassDB::bind_static_method("MachineStore", D_METHOD("get_machines"), &MachineStore::get_machines);

    ClassDB::bind_static_method("MachineStore", D_METHOD("instantiate", "machine_name"), &MachineStore::instantiate);
    ClassDB::bind_static_method("MachineStore", D_METHOD("get_building", "machine_name"), &MachineStore::get_building);
}

Array MachineStore::get_machines() {
    Array machines_arr;

    for (const auto& pair : MachineData::machine_instances) {
        MachineData* machine = pair.second;
        Dictionary machine_dict;

        machine_dict["name"] = machine->name;
        machine_dict["hash"] = machine->hash;
        machine_dict["produce_time"] = machine->produce_time;

        Array output_positions;
        for (Vector3 pos : machine->output_positions)
            output_positions.append(pos);
        machine_dict["output_positions"] = output_positions;

        machines_arr.append(machine_dict);
    }

    return machines_arr;
}

Machine* MachineStore::instantiate(const String& machine_name) {
    const auto it = MachineData::machine_instances.find(machine_name.hash());
    if (it == MachineData::machine_instances.end()) {
        UtilityFunctions::printerr("Can't find Machine: ", machine_name);
        return nullptr;
    }

    return Object::cast_to<Machine>(it->second->instantiate());
}

Building* MachineStore::get_building(const String& machine_name) {
    const auto it = MachineData::machine_instances.find(machine_name.hash());
    if (it == MachineData::machine_instances.end()) {
        UtilityFunctions::printerr("Can't find Machine: ", machine_name);
        return nullptr;
    }

    Building* building = memnew(Building());
    building->add_child(it->second->node->duplicate());

    building->hash = it->second->hash;
    building->base = it->second;

    building->prepare_for_building();
    return building;
}