#ifndef BUILDING_UTILITY_CLASS_H
#define BUILDING_UTILITY_CLASS_H

#include <godot_cpp/classes/node.hpp>

using namespace godot;

class BuildingUtility {
    public:
        static void prepare_for_instantiating(Node* node);
};

#endif // BUILDING_UTILITY_CLASS_H