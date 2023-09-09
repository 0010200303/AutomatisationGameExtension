#ifndef AG_REGISTER_TYPES_H
#define AG_REGISTER_TYPES_H

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "compiler_settings.hpp"

#include "Interfaces/IUpdatable.hpp"

#include "Mods/ModManager.hpp"
#include "Items/ItemStore.hpp"
#include "Machines/MachineStore.hpp"
#include "Recipes/RecipeBook.hpp"
#include "Storages/StorageHall.hpp"
#include "ConveyorBelts/ConveyorBeltWarehouse.hpp"

#include "World/World.hpp"
#include "Machines/Machine.hpp"
#include "Storages/Storage.hpp"
#include "ConveyorBelts/ConveyorBelt.hpp"

#include "Buildings/Building.hpp"

#if USE_ANALYTICS == TRUE
#include "Analytics/Analytics.hpp"
#endif

using namespace godot;

void initialize_AG_library_init_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;
	
	ClassDB::register_abstract_class<IUpdatable>();

	ClassDB::register_abstract_class<ModManager>();
	ClassDB::register_abstract_class<ItemStore>();
	ClassDB::register_abstract_class<MachineStore>();
	ClassDB::register_abstract_class<RecipeBook>();
	ClassDB::register_abstract_class<StorageHall>();
	ClassDB::register_abstract_class<ConveyorBeltWarehouse>();

	ClassDB::register_class<World>();
	ClassDB::register_class<Machine>();
	ClassDB::register_class<Storage>();
	ClassDB::register_class<ConveyorBelt>();

	ClassDB::register_class<Building>();

#if USE_ANALYTICS == TRUE
	ClassDB::register_abstract_class<Analytics>();
#endif
}

void uninitialize_AG_library_init_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;
}

extern "C" {
GDExtensionBool GDE_EXPORT AG_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_AG_library_init_module);
	init_obj.register_terminator(uninitialize_AG_library_init_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}

#endif // AG_REGISTER_TYPES_H