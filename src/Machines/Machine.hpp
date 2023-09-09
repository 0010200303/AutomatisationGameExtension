#ifndef MACHINE_CLASS_H
#define MACHINE_CLASS_H

#include <godot_cpp/variant/array.hpp>

#include <vector>

#include "../Interfaces/IUpdatable.hpp"
#include "../Interfaces/IInventory.hpp"
#include "../Inventory/SlotInventory.hpp"
#include "MachineData.hpp"

using namespace godot;

class Item;
class Recipe;
class ConveyorBelt;
class Machine : public IUpdatable, public IInventory {
    GDCLASS(Machine, IUpdatable);

    friend class MachineData;

    private:
        static void _bind_methods();
        const MachineData* base_machine = nullptr;

        Recipe* recipe = nullptr;

        float produce_time;
        float current_production_time = 0.0f;

        std::vector<ConveyorBelt*> outputs;
        std::vector<ConveyorBelt*> inputs;

        std::vector<SlotInventory> output_slots;
        std::vector<SlotInventory> input_slots;

        bool is_producing = false;
        bool finished_producing = false;

        void try_produce(float delta);
        void try_eject();

    public:
        Machine() { type = IUpdatable::MACHINE; }

        void update(float delta) override;

        void assign_recipe(int64_t recipe_hash);
        void _assign_recipe(const String& recipe_name);

        bool add_single_item(Item* item) override;
        bool remove_single_item(Item* item) override;

        int add_item(Item* item, int amount = 1) override;
        int remove_item(Item* item, int amount = 1) override;

        Array get_items() const override;

        inline const MachineData* get_base() const { return base_machine; }

        inline void set_output(ConveyorBelt* output, int index) { outputs[index] = output; }
        inline ConveyorBelt* get_output(int index) const { return outputs[index]; }
};

#endif // MACHINE_CLASS_H