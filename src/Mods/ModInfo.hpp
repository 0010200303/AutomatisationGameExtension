#ifndef MODINFO_CLASS_H
#define MODINFO_CLASS_H

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../Utils/StreamReader.hpp"

using namespace godot;

struct ModInfo {
    String path;

    String name;
    String description;
    PackedStringArray authors;
    String version;

    ModInfo(String mod_path) {
        path = mod_path;

        mod_path += "/ModInfo";
        if (FileAccess::file_exists(mod_path) == false) {
            UtilityFunctions::printerr(mod_path, " does not exist!");
            return;
        }

        Ref<FileAccess> file = FileAccess::open(mod_path, FileAccess::READ);
        StreamReader reader(file->get_buffer(file->get_length()));
        file->close();

        name = reader.read_string();
        description = reader.read_string();
        authors = reader.read_array_string();
        version = reader.read_string();
    }
};

#endif // MODINFO_CLASS_H