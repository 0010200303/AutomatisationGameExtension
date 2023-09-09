#include "ConveyorBeltData.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "ConveyorBelt.hpp"
#include "../Utils/StreamReader.hpp"

std::unordered_map<int64_t, ConveyorBeltData*> ConveyorBeltData::conveyorbelt_instances;

void ConveyorBeltData::load_ConveyorBeltWarehouse(String mod_path) {
    mod_path += "/";
    String path = mod_path + "ConveyorBeltWarehouse";
    if (FileAccess::file_exists(path) == false) {
        UtilityFunctions::printerr(path, " does not exist!");
        return;
    }

    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    StreamReader reader(file->get_buffer(file->get_length()));
    file->close();

    int conveyorbelt_count = reader.read_int();
    for (int i = 0; i < conveyorbelt_count; i++) {
        ConveyorBeltData* conveyorbelt = memnew(ConveyorBeltData(reader.read_string()));
        ConveyorBeltData::conveyorbelt_instances[conveyorbelt->hash] = conveyorbelt;

        // load Scene
        String scene_path = mod_path + reader.read_string();
        if (FileAccess::file_exists(scene_path) == false) {
            UtilityFunctions::printerr(scene_path + " does not exist");
            continue;
        }

        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(scene_path);
        scene->init_ref();
        conveyorbelt->scene = scene;

        conveyorbelt->speed = reader.read_float();
    }
}

Object* ConveyorBeltData::instantiate() const {
    ConveyorBelt* conveyorbelt = memnew(ConveyorBelt);
    conveyorbelt->base_conveyorbelt = this;

    conveyorbelt->speed = speed;
    return Object::cast_to<Object>(conveyorbelt);
}