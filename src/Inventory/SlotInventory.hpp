#ifndef SLOTINVENTORY_CLASS_H
#define SLOTINVENTORY_CLASS_H

#include <godot_cpp/variant/array.hpp>

#include "../Interfaces/IInventory.hpp"
#include "../Items/Item.hpp"

class SlotInventory : public IInventory {
    public:
        Item* item = nullptr;
        int amount;

        bool add_single_item(Item* _item) override;
        bool remove_single_item(Item* _item) override;

        int add_item(Item* _item, int _amount = 1) override;
        int remove_item(Item* _item, int _amount = 1) override;

        Array get_items() const override;
};

#endif // SLOTINVENTORY_CLASS_H