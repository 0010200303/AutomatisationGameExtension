#ifndef CONVEYORBELTDATA_CLASS_H
#define CONVEYORBELTDATA_CLASS_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/string.hpp>

#include <unordered_map>

#include "../Interfaces/IInstantiable.hpp"

using namespace godot;

class ConveyorBeltData : public IInstantiable {
    public:
        static std::unordered_map<int64_t, ConveyorBeltData*> conveyorbelt_instances;

        ConveyorBeltData(const String& conveyorbelt_name) : name(conveyorbelt_name), hash(conveyorbelt_name.hash()) { }

        int64_t hash;
        String name;

        Ref<PackedScene> scene;

        float speed;

        static void load_ConveyorBeltWarehouse(String mod_path);

        Object* instantiate() const override;
};

#endif // CONVEYORBELTDATA_CLASS_H