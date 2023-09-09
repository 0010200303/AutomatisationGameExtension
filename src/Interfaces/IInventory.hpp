#ifndef INVENTORY_INTERFACE_H
#define INVENTORY_INTERFACE_H

#include <godot_cpp/variant/array.hpp>

#include "../Items/Item.hpp"

class IInventory {
    public:
        virtual bool add_single_item(Item*) = 0;
        virtual bool remove_single_item(Item*) = 0;

        virtual int add_item(Item*, int amount = 1) = 0;
        virtual int remove_item(Item*, int amount = 1) = 0;
        virtual Array get_items() const = 0;
};

#endif // INVENTORY_INTERFACE_H