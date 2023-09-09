#include "MachineData.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "Machine.hpp"
#include "../Utils/StreamReader.hpp"
#include "../World/World.hpp"

std::unordered_map<int64_t, MachineData*> MachineData::machine_instances;

void MachineData::load_MachineStore(String mod_path) {
    mod_path += "/";
    String path = mod_path + "MachineStore";
    if (FileAccess::file_exists(path) == false) {
        UtilityFunctions::printerr(path, " does not exist!");
        return;
    }

    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    StreamReader reader(file->get_buffer(file->get_length()));
    file->close();

    int machine_count = reader.read_int();
    for (int i = 0; i < machine_count; i++) {
        MachineData* machine = memnew(MachineData(reader.read_string()));
        MachineData::machine_instances[machine->hash] = machine;

        // load Scene
        String scene_path = mod_path + reader.read_string();
        if (FileAccess::file_exists(scene_path) == false) {
            UtilityFunctions::printerr(scene_path + " does not exist");
            continue;
        }

        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(scene_path);
        scene->init_ref();
        machine->node = scene->instantiate();

        machine->produce_time = reader.read_float();
        machine->output_count = reader.read_int();
        machine->input_count = reader.read_int();
        machine->output_positions = reader.read_array_vector3();
        machine->input_positions = reader.read_array_vector3();
    }
}

Object* MachineData::instantiate() const {
    Machine* machine = memnew(Machine);
    machine->base_machine = this;

    machine->produce_time = produce_time;
    machine->outputs.resize(output_count, nullptr);
    machine->inputs.resize(input_count, nullptr);
    machine->output_slots.resize(output_count);
    machine->input_slots.resize(input_count);

    return Object::cast_to<Object>(machine);
}