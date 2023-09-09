#ifndef ITEMVISUAL_CLASS_H
#define ITEMVISUAL_CLASS_H

#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include <map>
#include <forward_list>

#include "Item.hpp"

class ItemVisual {
    private:
        Vector3 position;
        Vector3i grid_position;

        static Transform3D xform;
    
    public:
        static float high_distance;
        static float middle_distance;
        static float low_distance;

        static float grid_size;
        static int grid_check_size;
        static std::map<Vector3i, std::forward_list<ItemVisual*>> item_grid;
        static RenderingServer* rs;

        Item* item;

        float progress;
        
        ItemVisual(Item* _item, float _progress = 0.0f) : item(_item), progress(_progress) { }

        static void update_visuals();

        void set_position(const Vector3& new_position);
};

#endif // ITEMVISUAL_CLASS_H