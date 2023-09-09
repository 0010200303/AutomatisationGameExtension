#ifndef MODMANAGER_CLASS_H
#define MODMANAGER_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/array.hpp>

#include <vector>

#include "ModInfo.hpp"

using namespace godot;

class ModManager : public RefCounted {
    GDCLASS(ModManager, RefCounted);

    private:
        static void _bind_methods();

    public:
        static std::vector<ModInfo> mods;

        static void load_mods(const PackedStringArray& mod_paths);

        static Array get_mods();
};

#endif // MODMANAGER_CLASS_H