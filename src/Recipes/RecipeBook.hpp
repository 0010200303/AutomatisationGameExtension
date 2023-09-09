#ifndef RECIPEBOOK_CLASS_H
#define RECIPEBOOK_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

class RecipeBook : public RefCounted {
    GDCLASS(RecipeBook, RefCounted);

    private:
        static void _bind_methods();
    
    public:
        static Array get_recipes();
};

#endif // RECIPEBOOK_CLASS_H