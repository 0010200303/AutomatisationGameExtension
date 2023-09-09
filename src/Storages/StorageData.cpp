#include "StorageData.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "Storage.hpp"
#include "../Utils/StreamReader.hpp"

std::unordered_map<int64_t, StorageData*> StorageData::storage_instances;

void StorageData::load_StorageHall(String mod_path) {
    mod_path += "/";
    String path = mod_path + "StorageHall";
    if (FileAccess::file_exists(path) == false) {
        UtilityFunctions::printerr(path, " does not exist!");
        return;
    }

    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    StreamReader reader(file->get_buffer(file->get_length()));
    file->close();

    int storage_count = reader.read_int();
    for (int i = 0; i < storage_count; i++) {
        StorageData* storage = memnew(StorageData(reader.read_string()));
        StorageData::storage_instances[storage->hash] = storage;

        // load Scene
        String scene_path = mod_path + reader.read_string();
        if (FileAccess::file_exists(scene_path) == false) {
            UtilityFunctions::printerr(scene_path + " does not exist");
            continue;
        }

        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(scene_path);
        scene->init_ref();
        storage->node = scene->instantiate();

        storage->slot_count = reader.read_int();
        storage->output_count = reader.read_int();
        storage->input_count = reader.read_int();
        storage->output_positions = reader.read_array_vector3();
        storage->input_positions = reader.read_array_vector3();
    }
}

Object* StorageData::instantiate() const {
    Storage* storage = memnew(Storage(slot_count));
    storage->base_storage = this;

    storage->outputs.resize(output_count);
    storage->inputs.resize(input_count);

    return Object::cast_to<Object>(storage);
}