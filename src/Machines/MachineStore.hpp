#ifndef MACHINESTORE_CLASS_H
#define MACHINESTORE_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "Machine.hpp"
#include "Buildings/Building.hpp"

using namespace godot;

class MachineStore : public RefCounted {
    GDCLASS(MachineStore, RefCounted);

    private:
        static void _bind_methods();
    
    public:
        static Array get_machines();

        static Machine* instantiate(const String& machine_name);
        static Building* get_building(const String& machine_name);
};

#endif // MACHINESTORE_CLASS_H