#ifndef STORAGEHALL_CLASS_H
#define STORAGEHALL_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "Storage.hpp"
#include "../Buildings/Building.hpp"

using namespace godot;

class StorageHall : public RefCounted {
    GDCLASS(StorageHall, RefCounted);

    private:
        static void _bind_methods();
    
    public:
        static Array get_storages();

        static Storage* instantiate(const String& storage_name);
        static Building* get_building(const String& storage_name);
};

#endif // STORAGEHALL_CLASS_H