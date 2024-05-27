#ifndef BS_EXAMPLE_COMMON_H
#define BS_EXAMPLE_COMMON_H

/*
    //General Usage:
    
    // There is only one header file you need to include. Include 'BiBS.h'.
    #include BiBS.h

    class <CLASS_NAME> : public <GODOT_CLASS_BASE>
    {
        //
        // The BSCLASS macro marks your class as a BiBS class.
        // Use of this macro is mandatory to enable the BiBS feature set.
        // 
        // BSCLASS may appear anywhere in the class before the first BiBS declaration.
        //     This includes before the GDCLASS macro.
        //
        GDCLASS(<CLASS_NAME,<GODOT_CLASS_BASE>) BSCLASS;

        // ...
        // Your Class Definition
        // ...

        //
        // BiBS consumes the _ready() call, so you may not define this method.
        // Your class can declare a method named 'ready' instead.
        //     This method will be called AFTER all BiBS initialization is performed.
        //
        // Do note that declaring this method is OPTIONAL.
        //     BiBS will optimize out the call out if 'ready' is not declared.
        //

        void ready();


        //
        // BiBS consumes the _bind_methods() call, so you may not define this method.
        // Your class can declare a method named 'bind_methods' instead.
        //     This method will be called AFTER all BiBS method binding is performed.
        //
        // Do note that declaring this method is OPTIONAL.
        //     BiBS will optimize out the call out if 'ready' is not declared.
        //     You may alternatively declare this method as 'bindMethods', if you prefer camel case.
        //

        void bind_methods();
        void bindMethods();
    };
*/


//
// BiBS.h is the only header file you need to include.
//     Do not include other BiBS header files manually.
//
#include "BiBS.h"

//
// 'SelfType' is not a BiBS feature, but you may copy this macro to make your declarations nicer.
//
#define BS_SELF_TYPE SelfType
using namespace godot;


#endif