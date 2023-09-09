#ifndef RECIPE_CLASS_H
#define RECIPE_CLASS_H

#include <godot_cpp/variant/string.hpp>

#include <unordered_map>
#include <unordered_set>

#include "../Items/Item.hpp"

using namespace godot;

struct MachineData;
struct Recipe {
    public:
        static std::unordered_map<int64_t, Recipe*> recipe_instances;

        Recipe(String recipe_name) : name(recipe_name), hash(recipe_name.hash()) { }

        int64_t hash;
        String name;

        float produce_time;

        std::unordered_set<const MachineData*> machines;

        std::unordered_map<Item*, int> ingredients;
        std::unordered_map<Item*, int> products;

        static void load_RecipeBook(String mod_path);
};

#endif // RECIPE_CLASS_H