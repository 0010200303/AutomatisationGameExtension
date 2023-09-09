#ifndef CONVEYORBELT_CLASS_H
#define CONVEYORBELT_CLASS_H

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/variant/array.hpp>

#include <deque>

#include "../Interfaces/IUpdatable.hpp"
#include "../Interfaces/IInventory.hpp"
#include "../Items/Item.hpp"
#include "ConveyorBeltData.hpp"

using namespace godot;

class Item;
class Machine;
class Storage;
class ConveyorBelt : public IUpdatable {
    GDCLASS(ConveyorBelt, IUpdatable);

    friend class ConveyorBeltData;

    private:
        struct ConveyorBeltItem {
            Item* item = nullptr;
            float progress;
            bool reached_stop = false;

            ConveyorBeltItem(Item* _item, float _progress = 0.0f) : item(_item), progress(_progress) { }
        };

        static void _bind_methods();
        const ConveyorBeltData* base_conveyorbelt;

        float total_length;
        float current_length; // current length to stopped items

        IInventory* output = nullptr;

    public:
        ConveyorBelt() { type = IUpdatable::Type::CONVEYORBELT; curve.instantiate(); };

        std::deque<ConveyorBeltItem> items;

        float speed;
        Ref<Curve3D> curve;

        void update(float delta) override;

        bool append_item(Item* item);

        Array get_items() const;

        inline const ConveyorBeltData* get_base() const { return base_conveyorbelt; }

        inline void set_output(IInventory* _output) { output = _output; }
        inline IInventory* get_output() const { return output; }

        inline void _set_output(IUpdatable* _output);
        inline IUpdatable* _get_output() const;

        inline void add_point(const Vector3& point) { curve->add_point(point); total_length = curve->get_baked_length(); current_length = total_length; }
};

#endif // CONVEYORBELT_CLASS_H