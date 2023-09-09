#ifndef ITEMSTORE_CLASS_H
#define ITEMSTORE_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

class ItemStore : public RefCounted {
    GDCLASS(ItemStore, RefCounted);

    private:
        static void _bind_methods();
    
    public:
        static Array get_items();
};

#endif // ITEMSTORE_CLASS_H