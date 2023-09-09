#include "Inventory.hpp"

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <vector>

#define USE_FILO

bool Inventory::add_single_item(Item* item) {
    auto range = item_slots.equal_range(item);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.amount < item->max_stack_size) {
            it->second.amount += 1;
            return true;
        }
    }

    if (item_slots.size() < max_slot_count) {
        item_slots.insert(std::make_pair(item, ItemSlot(1, free_ids.top())));
        free_ids.pop();
        return true;
    }

    return false;
}

bool Inventory::remove_single_item(Item* item) {
    auto range = item_slots.equal_range(item);

#ifdef USE_FIFO
    // FIFO removing
    for (auto it = range.first; it != range.second; ++it) {
#endif
#ifdef USE_FILO
    // FILO removing
    for (auto it = std::prev(range.second); it != std::prev(range.first); it = std::prev(it)) {
#endif
        it->second.amount--;

        if (it->second.amount == 0) {
            free_ids.emplace(it->second.id);
            item_slots.erase(it);
        }

        return true;
    }

    return false;
}

int Inventory::add_item(Item* item, int amount) {
    if (amount == 1)
        return add_single_item(item) == true ? 0 : 1;

    int item_max_stack_size = item->max_stack_size;

    auto range = item_slots.equal_range(item);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.amount + amount <= item_max_stack_size) {
            it->second.amount += amount;
            return 0;
        }

        amount -= item_max_stack_size - it->second.amount;
        it->second.amount = item_max_stack_size;
    }

    // insert new slots
    while (amount > 0) {
        if (item_slots.size() == max_slot_count)
            return amount;

        int slot_amount = std::min(amount, item_max_stack_size);
        amount -= slot_amount;

        item_slots.insert(std::make_pair(item, ItemSlot(slot_amount, free_ids.top())));
        free_ids.pop();
    }

    return 0;
}

int Inventory::remove_item(Item* item, int amount) {
    if (amount == 1)
        return remove_single_item(item) == true ? 0 : 1;

    auto range = item_slots.equal_range(item);

#ifdef USE_FIFO
    // FIFO removing
    for (auto it = range.first; it != range.second; ++it) {
#endif
#ifdef USE_FILO
    // FILO removing
    for (auto it = std::prev(range.second); it != std::prev(range.first); it = std::prev(it)) {
#endif
        if (it->second.amount >= amount) {
            it->second.amount -= amount;

            if (it->second.amount <= 0) {
                free_ids.emplace(it->second.id);
                item_slots.erase(it);
            }

            return 0;
        }

        amount -= it->second.amount;
        free_ids.emplace(it->second.id);
        item_slots.erase(it);
    }

    return amount;
}

Array Inventory::get_items() const {
    Array item_arr;

    std::vector<std::pair<const Item*, int>> sorted_slots(max_slot_count);
    for (const auto& pair : item_slots)
        sorted_slots[pair.second.id] = std::make_pair(pair.first, pair.second.amount);

    for (const auto& pair : sorted_slots) {
        if (pair.first == nullptr)
            continue;

        Dictionary slot_dict;

        slot_dict["name"] = pair.first->name;
        slot_dict["amount"] = pair.second;

        item_arr.append(slot_dict);
    }

    return item_arr;
}