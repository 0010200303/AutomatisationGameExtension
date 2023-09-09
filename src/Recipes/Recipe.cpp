#include "Recipe.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../Utils/StreamReader.hpp"
#include "../Machines/MachineData.hpp"

std::unordered_map<int64_t, Recipe*> Recipe::recipe_instances;

void Recipe::load_RecipeBook(String mod_path) {
    mod_path += "/";
    String path = mod_path + "RecipeBook";
    if (FileAccess::file_exists(path) == false) {
        UtilityFunctions::printerr(path, " does not exist!");
        return;
    }

    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    StreamReader reader(file->get_buffer(file->get_length()));
    file->close();

    int recipe_count = reader.read_int();
    for (int i = 0; i < recipe_count; i++) {
        Recipe* recipe = memnew(Recipe(reader.read_string()));
        Recipe::recipe_instances[recipe->hash] = recipe;

        recipe->produce_time = reader.read_float();

        // link machines
        PackedStringArray machines = reader.read_array_string();
        for (const String& machine : machines) {
            const auto it = MachineData::machine_instances.find(machine.hash());
            if (it == MachineData::machine_instances.end()) {
                UtilityFunctions::printerr("Machine: ", machine, " can not be found!");
                continue;
            }

            recipe->machines.insert(it->second);
        }

        int ingredient_count = reader.read_int();
        for (int j = 0; j < ingredient_count; j++) {
            String item_name = reader.read_string();
            int item_count = reader.read_int();
            
            const auto it = Item::item_instances.find(item_name.hash());
            if (it == Item::item_instances.end()) {
                UtilityFunctions::printerr("Item: ", item_name, " can not be found!");
                break;
            }

            recipe->ingredients[it->second] = item_count;
        }

        int products_count = reader.read_int();
        for (int j = 0; j < products_count; j++) {
            String item_name = reader.read_string();
            int item_count = reader.read_int();

            const auto it = Item::item_instances.find(item_name.hash());
            if (it == Item::item_instances.end()) {
                UtilityFunctions::printerr("Item: ", item_name, " can not be found!");
                break;
            }

            recipe->products[it->second] = item_count;
        }
    }
}