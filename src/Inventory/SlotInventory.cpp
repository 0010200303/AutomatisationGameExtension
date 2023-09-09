#include "SlotInventory.hpp"

bool SlotInventory::add_single_item(Item* _item) {
    if (_item != item)
        return false;
    
    if (amount >= item->max_stack_size)
        return false;
    
    amount++;
    return true;
}

bool SlotInventory::remove_single_item(Item* _item) {
    if (_item != item)
        return false;
    
    if (amount <= 0)
        return false;
    
    amount--;
    return true;
}

int SlotInventory::add_item(Item* _item, int _amount) {
    if (_item != item)
        return _amount;
    
    if (amount + _amount <= item->max_stack_size)
        return 0;

    amount = item->max_stack_size;
    return _amount - item->max_stack_size;
}

int SlotInventory::remove_item(Item* _item, int _amount) {
    if (_item != item)
        return _amount;
    
    if (amount - _amount >= 0)
        return 0;

    int remaining = _amount - amount;
    amount = 0;
    return remaining;
}

Array SlotInventory::get_items() const {
    Array item_arr;

    Dictionary slot_dict;
    slot_dict["name"] = item->name;
    slot_dict["amount"] = amount;
    item_arr.append(slot_dict);

    return item_arr;
}