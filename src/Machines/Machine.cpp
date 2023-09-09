#include "Machine.hpp"

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../compiler_settings.hpp"
#include "../Items/Item.hpp"
#include "../Recipes/Recipe.hpp"
#include "../ConveyorBelts/ConveyorBelt.hpp"

void Machine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("assign_recipe", "recipe_name"), &Machine::_assign_recipe);

    ClassDB::bind_method(D_METHOD("set_output", "output", "index"), &Machine::set_output);
    ClassDB::bind_method(D_METHOD("get_output", "index"), &Machine::get_output);

    ClassDB::bind_method(D_METHOD("get_inventory"), &Machine::get_items);
}

void Machine::update(float delta) {
    try_produce(delta);
    try_eject();
}

void Machine::assign_recipe(int64_t recipe_hash) {
    const auto it = Recipe::recipe_instances.find(recipe_hash);
    if (it == Recipe::recipe_instances.end()) {
        UtilityFunctions::printerr("Can't find Recipe: ", recipe_hash);
        return;
    }

    if (it->second->machines.find(base_machine) == it->second->machines.end())
        return;

    if (it->second->ingredients.size() > input_slots.size()) {
        UtilityFunctions::printerr("Recipe has more ingredients than the Machine has inputs");
        return;
    }

    if (it->second->products.size() > output_slots.size()) {
        UtilityFunctions::printerr("Recipe has more products than the Machine has outputs");
        return;
    }

    for (int i = 0; i < input_slots.size(); i++) {
        input_slots[i].amount = 0;
        input_slots[i].item = nullptr;
    }
    for (int i = 0; i < output_slots.size(); i++) {
        output_slots[i].amount = 0;
        output_slots[i].item = nullptr;
    }

    recipe = it->second;

    int index = 0;
    for (const auto& pair : recipe->ingredients) {
        input_slots[index].item = pair.first;
        index++;
    }

    index = 0;
    for (const auto& pair : recipe->products) {
        output_slots[index].item = pair.first;
        index++;
    }
}

void Machine::_assign_recipe(const String& recipe_name) {
    const auto it = Recipe::recipe_instances.find(recipe_name.hash());
    if (it == Recipe::recipe_instances.end()) {
        UtilityFunctions::printerr("Can't find Recipe: ", recipe_name);
        return;
    }
    assign_recipe(recipe_name.hash());
}

bool Machine::add_single_item(Item* item) {
    for (SlotInventory& slot : input_slots) {
        if (item == slot.item && slot.amount < item->max_stack_size) {
            slot.amount++;
            return true;
        }
    }
    return false;
}

bool Machine::remove_single_item(Item* item) {
    for (SlotInventory& slot : output_slots) {
        if (item == slot.item && slot.amount > 0) {
            slot.amount--;
            return true;
        }
    }
    return false;
}

int Machine::add_item(Item* item, int amount) {
    if (amount == 1)
        return add_single_item(item) == true ? 0 : 1;

    int item_max_stack_size = item->max_stack_size;
    
    for (SlotInventory& slot : input_slots) {
        if (item != slot.item)
            continue;
        
        if (slot.amount + amount <= item_max_stack_size) {
            slot.amount += amount;
            return 0;
        }

        amount -= item_max_stack_size - slot.amount;
        slot.amount = item_max_stack_size;
    }

    return amount;
}

int Machine::remove_item(Item* item, int amount) {
    if (amount == 1)
        return remove_single_item(item) == true ? 0 : 1;

    for (SlotInventory& slot : output_slots) {
        if (item != slot.item)
            continue;
        
        if (slot.amount >= amount) {
            slot.amount -= amount;
            return 0;
        }

        amount -= slot.amount;
    }

    return amount;
}

Array Machine::get_items() const {
    Array item_arr;

    Array input_arr;
    for (SlotInventory si : input_slots) {
        if (si.item == nullptr)
            continue;

        input_arr.append(si.get_items());
    }
    item_arr.append(input_arr);
    
    Array output_arr;
    for (SlotInventory si : output_slots) {
        if (si.item == nullptr)
            continue;

        output_arr.append(si.get_items());
    }
    item_arr.append(output_arr);

    return item_arr;
}

void Machine::try_produce(float delta) {
    if (recipe == nullptr)
        return;

    if (is_producing == false && finished_producing == false) {
        for (const SlotInventory& slot : input_slots)
            if (slot.amount < recipe->ingredients[slot.item])
                return;
        
        for (SlotInventory& slot : input_slots) {
#if USE_ANALYTICS == TRUE
            int temp = recipe->ingredients[slot.item];
            slot.amount -= temp;
            slot.item->consumed_this_tick += temp;
#else
            slot.amount -= recipe->ingredients[slot.item];
#endif
        }
        
        is_producing = true;
    }

    if (is_producing == true) {
        current_production_time += delta;
        if (current_production_time < produce_time)
            return;
        current_production_time = 0.0;
        
        is_producing = false;
        finished_producing = true;
    }

    if (finished_producing == true) {
        for (const SlotInventory& slot : output_slots)
            if (slot.amount + recipe->products[slot.item] > slot.item->max_stack_size)
                return;

        for (SlotInventory& slot : output_slots) {
#if USE_ANALYTICS == TRUE
            int temp = recipe->products[slot.item];
            slot.amount += temp;
            slot.item->produced_this_tick += temp;
#else
            slot.amount += recipe->products[slot.item];
#endif
        }
        
        finished_producing = false;
    }
}

void Machine::try_eject() {
    for (int i = 0; i < outputs.size(); i++) {
        if (outputs[i] == nullptr || output_slots[i].amount <= 0)
            continue;

        if (outputs[i]->append_item(output_slots[i].item) == true)
            output_slots[i].amount--;
    }
}