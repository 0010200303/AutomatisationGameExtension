#ifndef INVENTORY_CLASS_H
#define INVENTORY_CLASS_H

#include <godot_cpp/variant/array.hpp>

#include <unordered_map>
#include <queue>

#include "../Interfaces/IInventory.hpp"
#include "../Items/Item.hpp"

class Inventory : public IInventory {
    private:
        struct ItemSlot {
            int amount;
            int id;

            ItemSlot() { }
            ItemSlot(int _amount, int _id) : amount(_amount), id(_id) { }
        };

        std::unordered_multimap<Item*, ItemSlot> item_slots;
        std::priority_queue<int, std::vector<int>, std::greater<int>> free_ids;

    public:
        Inventory(int slot_count) : max_slot_count(slot_count) {
            for (int i = 0; i < slot_count; i++)
                free_ids.emplace(i);
        }

        int max_slot_count;

        bool add_single_item(Item* item) override;
        bool remove_single_item(Item* item) override;

        int add_item(Item* item, int amount = 1) override;
        int remove_item(Item* item, int amount = 1) override;

        Array get_items() const override;
};

#endif // INVENTORY_CLASS_H