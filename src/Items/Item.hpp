#ifndef ITEM_CLASS_H
#define ITEM_CLASS_H

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_float32_array.hpp>
#include <godot_cpp/variant/rid.hpp>
#include <godot_cpp/classes/mesh.hpp>

#include <unordered_map>
#include <vector>

#include "../compiler_settings.hpp"

using namespace godot;

struct Item {
    public:
        struct MultimeshStruct {
            int count = 0;
            int count_middle = 0;
            int count_low = 0;

            int timeout_counter = 0;

            PackedFloat32Array item_buffer;
            PackedFloat32Array item_buffer_middle;
            PackedFloat32Array item_buffer_low;

            RID multimesh;
            RID multimesh_instance;
            RID multimesh_middle;
            RID multimesh_instance_middle;
            RID multimesh_low;
            RID multimesh_instance_low;
        };
        static int timeout;

        int64_t hash;
        String name;
        int max_stack_size;
        static std::unordered_map<int64_t, Item*> item_instances;

        bool use_lod = false;
        Ref<Mesh> mesh;
        Ref<Mesh> mesh_middle;
        Ref<Mesh> mesh_low;

        std::vector<MultimeshStruct> multimeshes;
        int current_multimesh = 0;
        int current_multimesh_middle = 0;
        int current_multimesh_low = 0;

        int max_mesh_count = 0;
        int max_mesh_count_middle = 0;
        int max_mesh_count_low = 0;

#if USE_ANALYTICS == TRUE
        int produced_this_tick = 0;
        int consumed_this_tick = 0;
#endif

        Item(const String& item_name) : name(item_name), hash(item_name.hash()) { }

        static void load_ItemStore(String mod_path);

        void add_multimesh();
        inline void remove_multimesh(int index) { multimeshes.erase(multimeshes.begin() + index); }
};

#endif // ITEM_CLASS_H