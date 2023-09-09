#ifndef BUILDING_CLASS_H
#define BUILDING_CLASS_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/material.hpp>

#include <vector>

#include "../Interfaces/IInstantiable.hpp"

using namespace godot;

class Building : public Node3D {
    GDCLASS(Building, Node3D);

    private:
        static void _bind_methods();

        void _prepare_for_building(Node* node);
    
    public:
        int64_t hash;
        IInstantiable* base = nullptr;

        std::vector<Area3D*> building_areas;

        Ref<Material> hologram_material;

        void prepare_for_building();

        inline void set_hash(int64_t value) { hash = value; }
        inline int64_t get_hash() const { return hash; }
};

#endif // BUILDING_CLASS_H