#ifndef MACHINEDATA_CLASS_H
#define MACHINEDATA_CLASS_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include <unordered_map>
#include <vector>

#include "../Interfaces/IInstantiable.hpp"

using namespace godot;

class Machine;
struct MachineData : public IInstantiable {
    public:
        static std::unordered_map<int64_t, MachineData*> machine_instances;

        MachineData(String machine_name) : name(machine_name), hash(machine_name.hash()) { }

        int64_t hash;
        String name;

        Node* node = nullptr;

        float produce_time;
        int output_count;
        int input_count;
        
        std::vector<Vector3> output_positions;
        std::vector<Vector3> input_positions;

        static void load_MachineStore(String mod_path);

        Object* instantiate() const override;
};

#endif // MACHINEDATA_CLASS_H