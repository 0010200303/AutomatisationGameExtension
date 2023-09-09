#ifndef IINSTANCE_IN_WORLD_INTERFACE_H
#define IINSTANCE_IN_WORLD_INTERFACE_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class IInstanceInWorld {
    public:
        IInstanceInWorld() { }
        IInstanceInWorld(Node3D* _node) : node(_node) { }

        Node3D* node = nullptr;
        int world_references = 0;
};

#endif // IINSTANCE_IN_WORLD_INTERFACE_H