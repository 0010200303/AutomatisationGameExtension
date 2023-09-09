#include "World.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../ConveyorBelts/ConveyorBelt.hpp"
#include "../ConveyorBelts/ConveyorBeltData.hpp"
#include "../Machines/Machine.hpp"
#include "../Machines/MachineData.hpp"
#include "../Storages/Storage.hpp"
#include "../Storages/StorageData.hpp"
#include "../Utils/BuildingUtility.hpp"

World* World::world_instance = nullptr;

World::World() {
    if (Engine::get_singleton()->is_editor_hint() == true) {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        return;
    }
    
    world_instance = this;
    rs = RenderingServer::get_singleton();

    // Building::hologram_material.instantiate();

#if USE_ANALYTICS == TRUE
    analytics = memnew(Analytics);
#endif
}

void World::_bind_methods() {
    ClassDB::bind_static_method("World", D_METHOD("get_world_instance"), &World::get_world_instance);

    ClassDB::bind_method(D_METHOD("add_machine", "Machine"), &World::add_machine);
    ClassDB::bind_method(D_METHOD("add_storage", "Storage"), &World::add_storage);
    ClassDB::bind_method(D_METHOD("add_conveyorbelt", "ConveyorBelt"), &World::add_conveyorbelt);
    ClassDB::bind_method(D_METHOD("instantiate_building", "building"), &World::instantiate_building);

    ClassDB::bind_method(D_METHOD("can_build", "building"), &World::can_build);

    ClassDB::bind_method(D_METHOD("set_tick_time"), &World::set_tick_time);
    ClassDB::bind_method(D_METHOD("get_tick_time"), &World::get_tick_time);
    ADD_PROPERTY(PropertyInfo(Variant::Type::FLOAT, "tick_time"), "set_tick_time", "get_tick_time");

    ClassDB::bind_method(D_METHOD("set_tps", "tps"), &World::set_tps);
    ClassDB::bind_method(D_METHOD("get_tps"), &World::get_tps);
    ADD_PROPERTY(PropertyInfo(Variant::Type::FLOAT, "tps"), "set_tps", "get_tps");

    ClassDB::bind_method(D_METHOD("set_local_player", "player"), &World::set_local_player);
    ClassDB::bind_method(D_METHOD("get_local_player"), &World::get_local_player);

#if USE_ANALYTICS == TRUE
    ClassDB::bind_method(D_METHOD("get_analytics"), &World::get_analytics);
#endif
}

void World::_process(float delta) {
    if (local_player == nullptr) {
        UtilityFunctions::printerr("local_player can not be nullptr!");
        queue_free();
        return;
    }

    current_tick_time += delta;
    if (current_tick_time < tick_time)
        return;

    tick(current_tick_time);
    update_grid();
    update_conveyorbelt_item_visuals();

    current_tick_time = 0.0;
}

void World::tick(float delta) {
    for (const auto& updatable : updatables)
        updatable->update(delta);
    
#if USE_ANALYTICS == TRUE
    analytics->update();
#endif

    tick_count++;
}

void World::update_grid() {
    Vector3i player_pos = (Vector3i)(local_player->get_position() / grid_size);

    // load and unload chunks
    for (int x = player_pos.x - chunk_distance ; x <= player_pos.x + chunk_distance; x++) {
        for (int y = player_pos.y - chunk_distance; y <= player_pos.y + chunk_distance; y++) {
            for (int z = player_pos.z - chunk_distance; z <=  player_pos.z + chunk_distance; z++) {
                int dx = player_pos.x - x;
                int dy = player_pos.y - y;
                int dz = player_pos.z - z;

                if (dx * dx + dy * dy + dz * dz > chunk_distance * chunk_distance) {
                    unload_chunk(Vector3i(x, y, z));
                }
                else {
                    load_chunk(Vector3i(x, y, z));
                }
            }
        }
    }
}

void World::load_chunk(const Vector3i& chunk) {
    if (loaded_chunks.insert(chunk).second == false)
        return;

    for (const auto& updatable : grid[chunk]) {
        if (updatable->instance_in_world != nullptr && updatable->instance_in_world->world_references > 0) {
            updatable->instance_in_world->world_references++;
            continue;
        }

        Node3D* node = nullptr;

        switch (updatable->type) {
            case IUpdatable::Type::CONVEYORBELT:
                {
                    ConveyorBelt* conveyorbelt = static_cast<ConveyorBelt*>(updatable);

                    node = Object::cast_to<Node3D>(conveyorbelt->get_base()->scene->instantiate());

                    Path3D* path = Object::cast_to<Path3D>(node->find_child("Path3D"));
                    if (path == nullptr) {
                        UtilityFunctions::printerr("Can't find node \"Path3D\" on Conveyor Belt");
                        break;
                    }
                    path->set_curve(conveyorbelt->curve);
                }
                break;
            case IUpdatable::Type::MACHINE:
                node = Object::cast_to<Node3D>(static_cast<Machine*>(updatable)->get_base()->node->duplicate());
                break;
            case IUpdatable::Type::STORAGE:
                node = Object::cast_to<Node3D>(static_cast<Storage*>(updatable)->get_base()->node->duplicate());
                break;
            default:
                UtilityFunctions::printerr("Updatable not recognized: ", updatable->get_class());
                continue;
        }

        node->set_meta("base", updatable);
        node->set_position(updatable->get_position());

        IInstanceInWorld* instance_in_world = memnew(IInstanceInWorld(node));
        updatable->instance_in_world = instance_in_world;

        // add to loaded_conveyorbelts if instance is conveyorbelt
        loaded_nodes[chunk].insert(instance_in_world);
        if (updatable->type == IUpdatable::Type::CONVEYORBELT)
            loaded_conveyorbelts.insert(instance_in_world);

        add_child(node);
    }
}

void World::unload_chunk(const Vector3i& chunk) {
    if (loaded_chunks.erase(chunk) == 0)
        return;
    
    for (const auto& instance : loaded_nodes[chunk]) {
        instance->world_references--;

        // remove from loaded_conveyorbelts if instance is conveyorbelt
        if (instance->node->has_meta("base") == true)
            if (Object::cast_to<ConveyorBelt>(instance->node->get_meta("base")) != nullptr)
                loaded_conveyorbelts.erase(instance);

        if (instance->world_references <= 0)
            instance->node->queue_free();
    }

    loaded_nodes.erase(chunk);
}

void World::update_conveyorbelt_item_visuals() const {
    // reset item multimeshes
    for (const auto& item : Item::item_instances) {
        item.second->current_multimesh = 0;
        item.second->current_multimesh_middle = 0;
        item.second->current_multimesh_low = 0;

        if (item.second->use_lod == false) {
            for (int i = item.second->multimeshes.size() - 1; i >= 0; i--) {
                Item::MultimeshStruct& multimesh = item.second->multimeshes[i];

                if (multimesh.count == 0) {
                    multimesh.timeout_counter++;
                }
                else {
                    multimesh.timeout_counter = 0;

                    multimesh.count = 0;
                }

                if (multimesh.timeout_counter >= Item::timeout)
                    item.second->remove_multimesh(i);
            }
        }
        else {
            for (int i = item.second->multimeshes.size() - 1; i >= 0; i--) {
                Item::MultimeshStruct& multimesh = item.second->multimeshes[i];

                if (multimesh.count == 0 && multimesh.count_middle == 0 && multimesh.count_low == 0) {
                    multimesh.timeout_counter++;
                }
                else {
                    multimesh.timeout_counter = 0;

                    multimesh.count = 0;
                    multimesh.count_middle = 0;
                    multimesh.count_low = 0;
                }

                if (multimesh.timeout_counter >= Item::timeout)
                    item.second->remove_multimesh(i);
            }
        }
    }

    for (const auto& conveyorbelt : loaded_conveyorbelts) {
        ConveyorBelt* belt = Object::cast_to<ConveyorBelt>(conveyorbelt->node->get_meta("base"));

        for (const auto& _item : belt->items) {
            Item* item = _item.item;
            Vector3 item_position = belt->curve->sample_baked(_item.progress, true);
            float distance = local_player->get_position().distance_squared_to(item_position);

            if (item->use_lod == false) {
                if (distance > low_distance)
                    continue;

                if (item->current_multimesh >= item->multimeshes.size())
                    item->add_multimesh();

                Item::MultimeshStruct& multimesh = item->multimeshes[item->current_multimesh];
                int index = multimesh.count * 12;

                multimesh.item_buffer[index +  3] = item_position.x;
                multimesh.item_buffer[index +  7] = item_position.y;
                multimesh.item_buffer[index + 11] = item_position.z;
                multimesh.count++;

                multimesh.count++;
                if (multimesh.count >= item->max_mesh_count)
                    item->current_multimesh++;
            }
            else {
                if (distance <= high_distance) {
                    if (item->current_multimesh >= item->multimeshes.size())
                        item->add_multimesh();

                    Item::MultimeshStruct& multimesh = item->multimeshes[item->current_multimesh];
                    int index = multimesh.count * 12;

                    multimesh.item_buffer[index +  3] = item_position.x;
                    multimesh.item_buffer[index +  7] = item_position.y;
                    multimesh.item_buffer[index + 11] = item_position.z;
                    multimesh.count++;

                    if (multimesh.count >= item->max_mesh_count)
                        item->current_multimesh++;
                }
                else if (distance <= middle_distance) {
                    if (item->current_multimesh_middle >= item->multimeshes.size())
                        item->add_multimesh();

                    Item::MultimeshStruct& multimesh = item->multimeshes[item->current_multimesh_middle];
                    int index = multimesh.count_middle * 12;

                    multimesh.item_buffer_middle[index +  3] = item_position.x;
                    multimesh.item_buffer_middle[index +  7] = item_position.y;
                    multimesh.item_buffer_middle[index + 11] = item_position.z;
                    multimesh.count_middle++;

                    if (multimesh.count_middle >= item->max_mesh_count_middle)
                        item->current_multimesh_middle++;
                }
                else if (distance <= low_distance) {
                    if (item->current_multimesh_low >= item->multimeshes.size())
                        item->add_multimesh();

                    Item::MultimeshStruct& multimesh = item->multimeshes[item->current_multimesh_low];
                    int index = multimesh.count_low * 12;

                    multimesh.item_buffer_low[index +  3] = item_position.x;
                    multimesh.item_buffer_low[index +  7] = item_position.y;
                    multimesh.item_buffer_low[index + 11] = item_position.z;
                    multimesh.count_low++;

                    if (multimesh.count_low >= item->max_mesh_count_low)
                        item->current_multimesh_low++;
                }
            }
        }
    }

    // finally push updates to RenderingServer
    for (auto it = Item::item_instances.begin(); it != Item::item_instances.end(); ++it) {
        const Item* item = it->second;

        for (Item::MultimeshStruct& multimesh : it->second->multimeshes) {
            rs->multimesh_set_buffer(multimesh.multimesh, multimesh.item_buffer);
            rs->multimesh_set_visible_instances(multimesh.multimesh, multimesh.count);

            if (it->second->use_lod == false)
                break;
            
            rs->multimesh_set_buffer(multimesh.multimesh_middle, multimesh.item_buffer_middle);
            rs->multimesh_set_visible_instances(multimesh.multimesh_middle, multimesh.count_middle);

            rs->multimesh_set_buffer(multimesh.multimesh_low, multimesh.item_buffer_low);
            rs->multimesh_set_visible_instances(multimesh.multimesh_low, multimesh.count_low);
        }
    }
}

void World::add_machine(Machine* machine) {
    updatables.insert(machine);
    grid[(Vector3i)(machine->get_position() / grid_size)].push_back(machine);
}

void World::add_storage(Storage* storage) {
    updatables.insert(storage);
    grid[(Vector3i)(storage->get_position() / grid_size)].push_back(storage);
}

void World::add_conveyorbelt(ConveyorBelt* conveyorbelt) {
    if (conveyorbelt->curve.is_null() == true) {
        UtilityFunctions::printerr("Conveyor Belt can't be added because it's curve is null");
        return;
    }

    updatables.insert(conveyorbelt);

    std::set<Vector3i> checked_chunks;
    for (const auto& point : conveyorbelt->curve->get_baked_points())
        checked_chunks.insert((Vector3i)(point / grid_size));
    
    for (auto& chunk : checked_chunks)
        grid[chunk].push_back(conveyorbelt);
}

void World::instantiate_building(Building* building) {
    if (MachineData* data = dynamic_cast<MachineData*>(building->base); data != nullptr) {
        Machine* machine = Object::cast_to<Machine>(data->instantiate());
        machine->set_position(building->get_position());
        add_machine(machine);
    }
    else if (StorageData* data = dynamic_cast<StorageData*>(building->base); data != nullptr) {
        Storage* storage = Object::cast_to<Storage>(data->instantiate());
        storage->set_position(building->get_position());
        add_storage(storage);
    }
    else if (ConveyorBeltData* data = dynamic_cast<ConveyorBeltData*>(building->base); data != nullptr) {
        UtilityFunctions::printerr("ConveyorBelts are not implemented yet!");
        return;
    }
    else {
        UtilityFunctions::printerr("Trying to add Building of unknown type");
        return;
    }

    // TODO: don't use this hack / find better way
    unload_chunk((Vector3i)(building->get_position() / grid_size));
    load_chunk((Vector3i)(building->get_position() / grid_size));
}

bool World::can_build(Building* building) const {
    for (const auto& area : building->building_areas)
        if (area->has_overlapping_areas() == true || area->has_overlapping_bodies() == true)
            return false;
    return true;
}