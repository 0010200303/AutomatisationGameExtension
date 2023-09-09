#include "BuildingUtility.hpp"

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void BuildingUtility::prepare_for_instantiating(Node* node) {
    for (int i = node->get_child_count() - 1; i >= 0; i--) {
        Node* child = node->get_child(i);
        if (child->get_name().begins_with("_building") == true)
            child->queue_free();
        else
            prepare_for_instantiating(child);
    }
}