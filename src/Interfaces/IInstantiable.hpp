#ifndef INSTANTIABLE_INTERFACE_H
#define INSTANTIABLE_INTERFACE_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class IInstantiable {
    public:
        virtual Object* instantiate() const = 0;
};

#endif // INSTANTIABLE_INTERFACE_H