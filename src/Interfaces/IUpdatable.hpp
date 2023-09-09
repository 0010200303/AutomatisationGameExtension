#ifndef IUPDATABLE_INTERFACE_H
#define IUPDATABLE_INTERFACE_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include "IInstanceInWorld.hpp"

using namespace godot;

class IUpdatable : public Object {
    GDCLASS(IUpdatable, Object);

    private:
        static void _bind_methods() {
            ClassDB::bind_method(D_METHOD("set_position", "position"), &IUpdatable::set_position);
            ClassDB::bind_method(D_METHOD("get_position"), &IUpdatable::get_position);
            ADD_PROPERTY(PropertyInfo(Variant::Type::VECTOR3, "position"), "set_position", "get_position");
        }
        Vector3 position;

    public:
        enum Type {
            NONE,
            CONVEYORBELT,
            MACHINE,
            STORAGE,
        } type = Type::NONE;

        IInstanceInWorld* instance_in_world = nullptr;

        virtual void update(float delta) { };

        inline void set_position(const Vector3& value) { position = value; }
        inline Vector3 get_position() const { return position; }
};

#endif // IUPDATABLE_INTERFACE_H