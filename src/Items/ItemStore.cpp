#include "ItemStore.hpp"

#include <godot_cpp/variant/dictionary.hpp>

#include "Item.hpp"

void ItemStore::_bind_methods() {
    ClassDB::bind_static_method("ItemStore", D_METHOD("get_items"), &ItemStore::get_items);
}

Array ItemStore::get_items() {
    Array items_arr;

    for (const auto& pair : Item::item_instances) {
        Item* item = pair.second;
        Dictionary item_dict;

        item_dict["name"] = item->name;
        item_dict["hash"] = item->hash;
        item_dict["max_stack_size"] = item->max_stack_size;
        item_dict["use_lod"] = item->use_lod;
        item_dict["mesh"] = item->mesh->get_path();
        item_dict["max_mesh_count"] = item->max_mesh_count;

        if (item->use_lod == false)
            continue;
        
        item_dict["mesh_middle"] = item->mesh_middle->get_path();
        item_dict["max_mesh_count_middle"] = item->max_mesh_count_middle;
        item_dict["mesh_low"] = item->mesh_low->get_path();
        item_dict["max_mesh_count_low"] = item->max_mesh_count_low;

        items_arr.append(item_dict);
    }

    return items_arr;
}