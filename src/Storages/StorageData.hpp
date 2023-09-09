#ifndef STORAGEDATA_CLASS_H
#define STORAGEDATA_CLASS_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include <unordered_map>
#include <vector>

#include "../Interfaces/IInstantiable.hpp"

using namespace godot;

struct StorageData : public IInstantiable {
    public:
        static std::unordered_map<int64_t, StorageData*> storage_instances;

        StorageData(const String& storage_name) : name(storage_name), hash(storage_name.hash()) { }

        int64_t hash;
        String name;

        Node* node = nullptr;

        int slot_count;
        int output_count;
        int input_count;
        
        std::vector<Vector3> output_positions;
        std::vector<Vector3> input_positions;

        static void load_StorageHall(String mod_path);

        Object* instantiate() const override;
};

#endif // STORAGEDATA_CLASS_H