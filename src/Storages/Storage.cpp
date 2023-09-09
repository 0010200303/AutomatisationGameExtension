#include "Storage.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include "../ConveyorBelts/ConveyorBelt.hpp"

void Storage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_item", "item_name", "amount"), &Storage::_add_item);
    ClassDB::bind_method(D_METHOD("remove_item", "item_name", "amount"), &Storage::_remove_item);

    ClassDB::bind_method(D_METHOD("get_inventory"), &Storage::_get_inventory);
}

void Storage::update(float delta) {
    
}

int Storage::_add_item(const String& item_name, int amount) {
    const auto it = Item::item_instances.find(item_name.hash());
    if (it == Item::item_instances.end()) {
        UtilityFunctions::printerr("Can't find Item: ", item_name);
        return false;
    }

    return add_item(it->second, amount);
}

int Storage::_remove_item(const String& item_name, int amount) {
    const auto it = Item::item_instances.find(item_name.hash());
    if (it == Item::item_instances.end()) {
        UtilityFunctions::printerr("Can't find Item: ", item_name);
        return false;
    }

    return remove_item(it->second, amount);
}