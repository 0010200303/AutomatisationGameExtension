#include "RecipeBook.hpp"

#include <godot_cpp/variant/dictionary.hpp>

#include "Recipe.hpp"
#include "../Machines/MachineData.hpp"

void RecipeBook::_bind_methods() {
    ClassDB::bind_static_method("RecipeBook", D_METHOD("get_recipes"), &RecipeBook::get_recipes);
}

Array RecipeBook::get_recipes() {
    Array recipe_arr;

    for (const auto& pair : Recipe::recipe_instances) {
        Recipe* recipe = pair.second;
        Dictionary recipe_dict;

        recipe_dict["name"] = recipe->name;
        recipe_dict["hash"] = recipe->hash;
        recipe_dict["produce_time"] = recipe->produce_time;

        Array machines;
        for (const MachineData* machine : recipe->machines)
            machines.append(machine->name);
        recipe_dict["machines"] = machines;
        
        Array ingredients;
        for (const auto& pair : recipe->ingredients) {
            Dictionary _pair;
            _pair[pair.first->name] = pair.second;
            ingredients.append(_pair);
        }
        recipe_dict["ingredients"] = ingredients;

        Array products;
        for (const auto& pair : recipe->products) {
            Dictionary _pair;
            _pair[pair.first->name] = pair.second;
            products.append(_pair);
        }
        recipe_dict["products"] = products;

        recipe_arr.append(recipe_dict);
    }

    return recipe_arr;
}