#include "ConveyorBeltWarehouse.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "ConveyorBeltData.hpp"

void ConveyorBeltWarehouse::_bind_methods() {
    ClassDB::bind_static_method("ConveyorBeltWarehouse", D_METHOD("get_conveyorbelts"), &ConveyorBeltWarehouse::get_conveyorbelts);

    ClassDB::bind_static_method("ConveyorBeltWarehouse", D_METHOD("instantiate"), &ConveyorBeltWarehouse::instantiate);
}

Array ConveyorBeltWarehouse::get_conveyorbelts() {
    Array conveyorbelt_arr;

    for (const auto& pair : ConveyorBeltData::conveyorbelt_instances) {
        ConveyorBeltData* conveyorbelt = pair.second;
        Dictionary conveyorbelt_dict;

        conveyorbelt_dict["name"] = conveyorbelt->name;
        conveyorbelt_dict["hash"] = conveyorbelt->hash;
        conveyorbelt_dict["scene"] = conveyorbelt->scene->get_path();
        conveyorbelt_dict["speed"] = conveyorbelt->speed;

        conveyorbelt_arr.append(conveyorbelt_dict);
    }

    return conveyorbelt_arr;
}

ConveyorBelt* ConveyorBeltWarehouse::instantiate(const String& conveyorbelt_name) {
    const auto it = ConveyorBeltData::conveyorbelt_instances.find(conveyorbelt_name.hash());
    if (it == ConveyorBeltData::conveyorbelt_instances.end()) {
        UtilityFunctions::printerr("Can't find Conveyor Belt: ", conveyorbelt_name);
        return nullptr;
    }

    return Object::cast_to<ConveyorBelt>(it->second->instantiate());
}