#include "ConveyorBelt.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include "../Items/Item.hpp"
#include "../Machines/Machine.hpp"
#include "../Storages/Storage.hpp"

#define ITEM_SIZE 1.2f

void ConveyorBelt::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_items"), &ConveyorBelt::get_items);

    ClassDB::bind_method(D_METHOD("set_output", "output"), &ConveyorBelt::_set_output);
    ClassDB::bind_method(D_METHOD("get_output"), &ConveyorBelt::_get_output);

    ClassDB::bind_method(D_METHOD("add_point", "point"), &ConveyorBelt::add_point);
}

void ConveyorBelt::update(float delta) {
    bool recalculate = false;
    float progress_increment = speed * delta;

    if (output == nullptr) {
        for (auto& it : items) {
            if (it.reached_stop == true)
                continue;

            it.progress = std::min(it.progress + progress_increment, current_length);
            if (it.progress >= current_length) {
                current_length = std::max(current_length - ITEM_SIZE, 0.0f);
                it.reached_stop = true;
            }
        }
    }
    else {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->reached_stop == true && recalculate == false) {
                if (it->progress >= total_length && output->add_single_item(it->item) == true) {
                    items.erase(it);
                    recalculate = true;
                    current_length = total_length;
                }
                continue;
            }

            it->progress = std::min(it->progress + progress_increment, current_length);

            if (it->progress >= current_length) {
                if (it->progress >= total_length && output->add_single_item(it->item) == true) {
                    items.erase(it);
                    recalculate = true;
                    current_length = total_length;

                    continue;
                }

                current_length = std::max(current_length - ITEM_SIZE, 0.0f);
                it->reached_stop = true;
            }
        }
    }
}

bool ConveyorBelt::append_item(Item* item) {
    if (items.empty() == false && items.back().progress < ITEM_SIZE)
        return false;
    
    items.emplace_back(item, 0.0f);

    return true;
}

Array ConveyorBelt::get_items() const {
    Array item_arr;

    for (const ConveyorBeltItem& item : items) {
        Dictionary item_dict;

        item_dict["name"] = item.item->name;
        item_dict["progress"] = item.progress;

        item_arr.append(item_dict);
    }

    return item_arr;
}

void ConveyorBelt::_set_output(IUpdatable* _output) {
    if (dynamic_cast<Machine*>(_output) != nullptr)
        output = (Machine*)_output;
    else if (dynamic_cast<Storage*>(_output) != nullptr)
        output = (Storage*)_output;
    else
        UtilityFunctions::printerr("Output is not of type Machine or Storage");
}

IUpdatable* ConveyorBelt::_get_output() const {
    return Object::cast_to<IUpdatable>((IUpdatable*)output);
}