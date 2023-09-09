#include "../compiler_settings.hpp"
#if USE_ANALYTICS == TRUE

#include "Analytics.hpp"

void Analytics::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_produced_in_tick", "item_name", "tick"), &Analytics::_get_produced_in_tick);
    ClassDB::bind_method(D_METHOD("get_consumed_in_tick", "item_name", "tick"), &Analytics::_get_consumed_in_tick);

    ClassDB::bind_method(D_METHOD("get_produced_this_tick", "item_name"), &Analytics::_get_produced_this_tick);
    ClassDB::bind_method(D_METHOD("get_consumed_this_tick", "item_name"), &Analytics::_get_consumed_this_tick);
}

void Analytics::update() {
    std::unordered_map<const Item*, int> produced_this_tick;
    std::unordered_map<const Item*, int> consumed_this_tick;

    for (const auto& pair : Item::item_instances) {
        if (pair.second->produced_this_tick != 0)
            produced_this_tick.emplace(pair.second, pair.second->produced_this_tick);
        
        if (pair.second->consumed_this_tick != 0)
            consumed_this_tick.emplace(pair.second, pair.second->consumed_this_tick);
    }

    produced_items.push_back(produced_this_tick);
    consumed_items.push_back(consumed_this_tick);

    // reset
    for (const auto& pair : Item::item_instances) {
        pair.second->produced_this_tick = 0;
        pair.second->consumed_this_tick = 0;
    }
}
#endif