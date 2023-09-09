#ifndef STORAGE_CLASS_H
#define STORAGE_CLASS_H

#include <godot_cpp/variant/array.hpp>

#include <vector>

#include "../Interfaces/IUpdatable.hpp"
#include "../Inventory/Inventory.hpp"
#include "StorageData.hpp"

using namespace godot;

class ConveyorBelt;
class Storage : public IUpdatable, public Inventory {
    GDCLASS(Storage, IUpdatable);

    friend class StorageData;

    private:
        static void _bind_methods();
        const StorageData* base_storage;

        std::vector<ConveyorBelt*> outputs;
        std::vector<ConveyorBelt*> inputs;

    public:
        Storage() : Inventory(0) { type = IUpdatable::Type::STORAGE; }
        Storage(int slot_count) : Inventory(slot_count) { type = IUpdatable::Type::STORAGE; }

        void update(float delta);

        int _add_item(const String& item_name, int amount = 1);
        int _remove_item(const String& item_name, int amount = 1);
        Array _get_inventory() { return get_items(); }

        inline const StorageData* get_base() const { return base_storage; }
};

#endif // STORAGE_CLASS_H