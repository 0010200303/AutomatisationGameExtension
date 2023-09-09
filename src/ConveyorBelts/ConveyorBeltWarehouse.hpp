#ifndef CONVEYORBELTWAREHOUSE_CLASS_H
#define CONVEYORBELTWAREHOUSE_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>

#include "ConveyorBelt.hpp"

using namespace godot;

class ConveyorBeltWarehouse : public RefCounted {
    GDCLASS(ConveyorBeltWarehouse, RefCounted);

    private:
        static void _bind_methods();
    
    public:
        static Array get_conveyorbelts();

        static ConveyorBelt* instantiate(const String& conveyorbelt_name);
};

#endif // CONVEYORBELTWAREHOUSE_CLASS_H