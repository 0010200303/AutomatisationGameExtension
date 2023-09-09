#include "Item.hpp"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../Utils/StreamReader.hpp"
#include "../World/World.hpp"

std::unordered_map<int64_t, Item*> Item::item_instances;
int Item::timeout = 1200;

void Item::add_multimesh() {
    MultimeshStruct multimesh;
    RenderingServer* rs = RenderingServer::get_singleton();

    // normal mesh
    multimesh.item_buffer.resize(max_mesh_count * 12);
    for (int i = 0; i < max_mesh_count; i++) {
        int index = i * 12;
        multimesh.item_buffer[index +  0] = 1.0f;
        multimesh.item_buffer[index +  1] = 0.0f;
        multimesh.item_buffer[index +  2] = 0.0f;
        multimesh.item_buffer[index +  4] = 0.0f;
        multimesh.item_buffer[index +  5] = 1.0f;
        multimesh.item_buffer[index +  6] = 0.0f;
        multimesh.item_buffer[index +  8] = 0.0f;
        multimesh.item_buffer[index +  9] = 0.0f;
        multimesh.item_buffer[index + 10] = 1.0f;
    }

    multimesh.multimesh = rs->multimesh_create();
    multimesh.multimesh_instance = rs->instance_create2(multimesh.multimesh, World::world_instance->get_world_3d()->get_scenario());
    rs->multimesh_allocate_data(multimesh.multimesh, max_mesh_count, RenderingServer::MultimeshTransformFormat::MULTIMESH_TRANSFORM_3D, false, false);
    rs->multimesh_set_mesh(multimesh.multimesh, mesh->get_rid());
    rs->instance_geometry_set_lod_bias(multimesh.multimesh_instance, 0.0f);
    rs->multimesh_set_buffer(multimesh.multimesh, multimesh.item_buffer);

    if (use_lod == false) {
        multimeshes.push_back(multimesh);
        return;
    }
    
    // middle mesh
    multimesh.item_buffer.resize(max_mesh_count_middle * 12);
    for (int i = 0; i < max_mesh_count_middle; i++) {
        int index = i * 12;
        multimesh.item_buffer_middle[index +  0] = 1.0f;
        multimesh.item_buffer_middle[index +  1] = 0.0f;
        multimesh.item_buffer_middle[index +  2] = 0.0f;
        multimesh.item_buffer_middle[index +  4] = 0.0f;
        multimesh.item_buffer_middle[index +  5] = 1.0f;
        multimesh.item_buffer_middle[index +  6] = 0.0f;
        multimesh.item_buffer_middle[index +  8] = 0.0f;
        multimesh.item_buffer_middle[index +  9] = 0.0f;
        multimesh.item_buffer_middle[index + 10] = 1.0f;
    }

    multimesh.multimesh_middle = rs->multimesh_create();
    multimesh.multimesh_instance_middle = rs->instance_create2(multimesh.multimesh_middle, World::world_instance->get_world_3d()->get_scenario());
    rs->multimesh_allocate_data(multimesh.multimesh_middle, max_mesh_count_middle, RenderingServer::MultimeshTransformFormat::MULTIMESH_TRANSFORM_3D, false, false);
    rs->multimesh_set_mesh(multimesh.multimesh_middle, mesh_middle->get_rid());
    rs->instance_geometry_set_lod_bias(multimesh.multimesh_instance_middle, 0.0f);
    rs->multimesh_set_buffer(multimesh.multimesh_middle, multimesh.item_buffer_middle);

    // low mesh
    multimesh.item_buffer_low.resize(max_mesh_count_low * 12);
    for (int i = 0; i < max_mesh_count_low; i++) {
        int index = i * 12;
        multimesh.item_buffer_low[index +  0] = 1.0f;
        multimesh.item_buffer_low[index +  1] = 0.0f;
        multimesh.item_buffer_low[index +  2] = 0.0f;
        multimesh.item_buffer_low[index +  4] = 0.0f;
        multimesh.item_buffer_low[index +  5] = 1.0f;
        multimesh.item_buffer_low[index +  6] = 0.0f;
        multimesh.item_buffer_low[index +  8] = 0.0f;
        multimesh.item_buffer_low[index +  9] = 0.0f;
        multimesh.item_buffer_low[index + 10] = 1.0f;
    }

    multimesh.multimesh_low = rs->multimesh_create();
    multimesh.multimesh_instance_low = rs->instance_create2(multimesh.multimesh_low, World::world_instance->get_world_3d()->get_scenario());
    rs->multimesh_allocate_data(multimesh.multimesh_low, max_mesh_count_low, RenderingServer::MultimeshTransformFormat::MULTIMESH_TRANSFORM_3D, false, false);
    rs->multimesh_set_mesh(multimesh.multimesh_low, mesh_low->get_rid());
    rs->instance_geometry_set_lod_bias(multimesh.multimesh_instance_low, 0.0f);
    rs->multimesh_set_buffer(multimesh.multimesh_low, multimesh.item_buffer_low);

    multimeshes.push_back(multimesh);
}

void Item::load_ItemStore(String mod_path) {
    mod_path += "/";
    String path = mod_path + "ItemStore";
    if (FileAccess::file_exists(path) == false) {
        UtilityFunctions::printerr(path, " does not exist!");
        return;
    }

    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    StreamReader reader(file->get_buffer(file->get_length()));
    file->close();

    int item_count = reader.read_int();
    for (int i = 0; i < item_count; i++) {
        Item* item = memnew(Item(reader.read_string()));
        Item::item_instances[item->hash] = item;

        item->max_stack_size = reader.read_int();

        // load mesh
        item->max_mesh_count = reader.read_int();
        String mesh_path = mod_path + reader.read_string();
        if (FileAccess::file_exists(mesh_path) == false) {
            UtilityFunctions::printerr(mesh_path + " does not exist");
            continue;
        }

        Ref<Mesh> mesh = ResourceLoader::get_singleton()->load(mesh_path);
        mesh->init_ref();
        item->mesh = mesh;

        item->use_lod = reader.read_bool();

        if (item->use_lod == false)
            continue;
        
        // load middle mesh
        item->max_mesh_count_middle = reader.read_int();
        mesh_path = mod_path + reader.read_string();
        if (FileAccess::file_exists(mesh_path) == false) {
            UtilityFunctions::printerr(mesh_path + " does not exist");
            continue;
        }

        Ref<Mesh> mesh_middle = ResourceLoader::get_singleton()->load(mesh_path);
        mesh_middle->init_ref();
        item->mesh_middle = mesh_middle;

        // load low mesh
        item->max_mesh_count_low = reader.read_int();
        mesh_path = mod_path + reader.read_string();
        if (FileAccess::file_exists(mesh_path) == false) {
            UtilityFunctions::printerr(mesh_path + " does not exist");
            continue;
        }

        Ref<Mesh> mesh_low = ResourceLoader::get_singleton()->load(mesh_path);
        mesh_low->init_ref();
        item->mesh_low = mesh_low;
    }
}