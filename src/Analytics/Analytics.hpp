#include "../compiler_settings.hpp"
#if USE_ANALYTICS == TRUE

#ifndef ANALYTICS_CLASS_H
#define ANALYTICS_CLASS_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <vector>
#include <unordered_map>

#include "../Items/Item.hpp"

using namespace godot;

class Analytics : public Object {
    GDCLASS(Analytics, Object);

    private:
        static void _bind_methods();

        std::vector<std::unordered_map<const Item*, int>> produced_items;
        std::vector<std::unordered_map<const Item*, int>> consumed_items;

        std::vector<int> kek;

    public:
        Analytics() { }

        void update();

        inline int get_produced_in_tick(const Item* item, int tick = -1) const {
            tick = tick == -1 ? produced_items.size() - 1 : tick;

            auto search = produced_items[tick].find(item);
            if (search == produced_items[tick].end())
                return 0;
            return search->second;
        }
        inline int get_consumed_in_tick(const Item* item, int tick = -1) const {
            tick = tick == -1 ? produced_items.size() - 1 : tick;

            const auto& search = consumed_items[tick].find(item);
            if (search == consumed_items[tick].end())
                return 0;
            return search->second;
        }

        inline int get_produced_this_tick(const Item* item) const { return get_produced_in_tick(item, -1); }
        inline int get_consumed_this_tick(const Item* item) const { return get_consumed_in_tick(item, -1); }

        inline int _get_produced_in_tick(const String& item_name, int tick = -1) const {
            const auto& search = Item::item_instances.find(item_name.hash());
            if (search == Item::item_instances.end()) {
                UtilityFunctions::printerr("Can't find Item: ", item_name);
                return -1;
            }
            return get_produced_in_tick(search->second, tick);
        }
        inline int _get_consumed_in_tick(const String& item_name, int tick = -1) const { 
            const auto& search = Item::item_instances.find(item_name.hash());
            if (search == Item::item_instances.end()) {
                UtilityFunctions::printerr("Can't find Item: ", item_name);
                return -1;
            }
            return get_consumed_in_tick(search->second, tick);
        }

        inline int _get_produced_this_tick(const String& item_name) const { return _get_produced_in_tick(item_name, -1); }
        inline int _get_consumed_this_tick(const String& item_name) const { return _get_consumed_in_tick(item_name, -1); }
};

#endif // ANALYTICS_CLASS_H
#endif