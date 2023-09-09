#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include <unordered_set>
#include <map>
#include <deque>
#include <set>

#include "../compiler_settings.hpp"
#include "../Interfaces/IInstanceInWorld.hpp"
#include "../Interfaces/IUpdatable.hpp"
#include "../Buildings/Building.hpp"

#if USE_ANALYTICS == TRUE
#include "../Analytics/Analytics.hpp"
#endif

using namespace godot;

class Machine;
class Storage;
class ConveyorBelt;
class World : public Node3D {
    GDCLASS(World, Node3D);

    private:
        static void _bind_methods();

        int tick_count;
        float tick_time = 0.2f;
        float current_tick_time = 0.0f;
        std::unordered_set<IUpdatable*> updatables;

        float grid_size = 1.0f;
        int chunk_distance = 10;
        std::map<Vector3i, std::deque<IUpdatable*>> grid;

        std::set<Vector3i> loaded_chunks;
        std::map<Vector3i, std::unordered_set<IInstanceInWorld*>> loaded_nodes;
        std::unordered_set<IInstanceInWorld*> loaded_conveyorbelts;

        Node3D* local_player = nullptr;

        RenderingServer* rs = nullptr;

#if USE_ANALYTICS == TRUE
        Analytics* analytics = nullptr;
#endif

        void tick(float delta);
        void update_grid();
        void load_chunk(const Vector3i& chunk);
        void unload_chunk(const Vector3i& chunk);
        void update_conveyorbelt_item_visuals() const;
    
    public:
        static World* world_instance;
        static inline World* get_world_instance() { return world_instance; }

        World();

        // 30.0f^2
        float high_distance = 900.0f;
        // 80.0f^2
        float middle_distance = 6400.0f;
        // 160.0f^2
        float low_distance = 25600.0f;

        void _process(float delta);

        void add_machine(Machine* machine);
        void add_storage(Storage* storage);
        void add_conveyorbelt(ConveyorBelt* conveyorbelt);
        void instantiate_building(Building* building);
        
        bool can_build(Building* building) const;

        inline void set_tick_time(float value) { tick_time = value; }
        inline float get_tick_time() const { return tick_time; }

        inline void set_tps(float value) { tick_time = 1.0f / value; }
        inline float get_tps() const { return 1.0f / tick_time; }

        inline void set_local_player(Node3D* value) { local_player = value; }
        inline Node3D* get_local_player() const { return local_player; }

#if USE_ANALYTICS == TRUE
        inline Analytics* get_analytics() const { return analytics; }
#endif
};

#endif // WORLD_CLASS_H