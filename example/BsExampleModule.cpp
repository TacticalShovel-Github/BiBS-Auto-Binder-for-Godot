
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>


#include "BsExampleBinding.h"
#include "BsExampleSignals.h"
#include "BsExampleTestCases.h"

using namespace godot;

void bsExampleInit(ModuleInitializationLevel initType) {
    if (initType != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    Bs::LogBS::write("MouseTrap - Start");
    ClassDB::register_class<MouseTrap>();
    Bs::LogBS::write("MouseTrap - End");

    Bs::LogBS::write("MouseDetector - Start");
    ClassDB::register_class<MouseDetector>();
    Bs::LogBS::write("MouseDetector - End");

    Bs::LogBS::write("DataMemberBindingExample - Start");
    ClassDB::register_class<DataMemberBindingExample>();
    Bs::LogBS::write("DataMemberBindingExample - End");

    Bs::LogBS::write("ExampleResourceTuple - Start");
    ClassDB::register_class<ExampleResourceTuple>();
    Bs::LogBS::write("ExampleResourceTuple - End");

    Bs::LogBS::write("MethodBindingExample - Start");
    ClassDB::register_class<MethodBindingExample>();
    Bs::LogBS::write("MethodBindingExample - End");

    Bs::LogBS::write("ExportTester - Start");
    ClassDB::register_class<ExportTester>();
    Bs::LogBS::write("ExportTester - End");

    Bs::LogBS::write("ExportTester::ExampleTuple - Start");
    ClassDB::register_class<ExportTester::ExampleTuple>();
    Bs::LogBS::write("ExportTester::ExampleTuple - End");

}

void bsExampleDeinit(ModuleInitializationLevel initType)
{
    if (initType != MODULE_INITIALIZATION_LEVEL_SCENE) {return;}
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT bsExampleModule(
        GDExtensionInterfaceGetProcAddress procAddressPtr,
        GDExtensionClassLibraryPtr libraryPtr,
        GDExtensionInitialization *initializationPtr) 
    {
        godot::GDExtensionBinding::InitObject initObj(procAddressPtr, libraryPtr, initializationPtr);
        initObj.register_initializer(bsExampleInit);
        initObj.register_terminator(bsExampleDeinit);
        initObj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        return initObj.init();
    }
}