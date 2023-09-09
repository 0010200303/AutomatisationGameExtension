#include "Building.hpp"

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

void Building::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_hash"), &Building::get_hash);
}

void Building::_prepare_for_building(Node* node) {
    for (int i = node->get_child_count() - 1; i >= 0; i--) {
        Node* child = node->get_child(i);
        if (child->get_name().begins_with("_building") == true) {
            if (Area3D* area = Object::cast_to<Area3D>(child); area != nullptr) {
                building_areas.push_back(area);
            }
        }
        else if (MeshInstance3D* mesh_instance = Object::cast_to<MeshInstance3D>(child); mesh_instance != nullptr) {
            mesh_instance->set_material_overlay(hologram_material);
            mesh_instance->set_transparency(0.3f);
            mesh_instance->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);
        }
        else if (child->is_class("CollisionShape3D") == true) {
            Object::cast_to<CollisionShape3D>(child)->set_disabled(true);
        }
        
        _prepare_for_building(child);
    }
}

void Building::prepare_for_building() {
    _prepare_for_building(this);
}