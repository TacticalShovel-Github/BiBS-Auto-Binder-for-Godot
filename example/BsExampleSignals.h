#ifndef BS_EXAMPLE_SIGNALS_H
#define BS_EXAMPLE_SIGNALS_H

#include <godot_cpp/classes/Area3D.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "BsExampleCommon.h"
#include "BsExampleExternalSignals.h"

/*
    I am aliasing these templates soley for aesthetic reasons.
    It would work the same if the typenames were pasted directly into the class.
*/
using MouseEnterEvt = Bs::Signals<CollisionObject3D>::MouseEnterEvt;
using MouseExitEvt = Bs::Signals<CollisionObject3D>::MouseExitEvt;

// Declare a 'MouseDetector', which will listen for a signal to detect our mouse.
//     Once detected, it will send a signal to the mouse trap, which then catches the mouse.
class MouseDetector : public Node3D {
    GDCLASS(MouseDetector, Node3D)
    BSCLASS;
public:
    //Export two members to give us something dynamic to print
    int one=0;
    int two=0;
    BS_EXPORT(&MouseDetector::one,.name="one");
    BS_EXPORT(&MouseDetector::two,.name="two");

    //Export an area object so we can assign it a value in the editor.
    //    Signals emitters do not need to be exported or declared as BSCLASS to work.
    //    We are just doing this so that the area can be changed in the editor.
    Area3D * area = nullptr;
    BS_EXPORT(&MouseDetector::area,.name="MouseArea");

    //Declare a custom signal, giving it a name and three parameters
    //    Any type that is assignable to a variant can be a parameter.
    //    See BsExampleBinding.hpp for more information.
    using MouseEvt = Bs::Signal<
        Bs::label("MouseEvt"),
        Bs::param<Object *>("obj"),
        Bs::param<bool    >("entered"),
        Bs::param<int     >("currentValue")
    >;

    //
    // Deduplicate code shared by the different callbacks
    //
    void onMouseEnterOrExit(Area3D * area, bool entered)
    {
        if(area == nullptr){return;}

        const std::string msg = 
            std::string("Mouse") + (entered ? " entered" : " exited") +
            " {Area3d: " + BS_GD_OBJECT_NAME(area) + "}" + 
            " while values was {int32_t: " + std::to_string(one+two) + "}";
        UtilityFunctions::print( Bs::Gd::godotify(msg) );

        this->emit<MouseEvt>(area,false,this->one + this->two);
    }

    //
    // Declare and define our callbacks
    //     Callbacks must be exported, but their arguments can be anonymous.
    //
    void onMouseEnterCb(){onMouseEnterOrExit(area,true);}
    void onMouseExitCb(){onMouseEnterOrExit(area,false);}
    BS_EXPORT(&onMouseEnterCb, .name="onMouseEnterCb");
    BS_EXPORT(&onMouseExitCb, .name="onMouseExitCb");
    
    //
    // Declare our signals and their callback functions
    //
    BS_SIGNAL_RX(MouseEnterEvt, .callback=&onMouseEnterCb);
    BS_SIGNAL_RX(MouseExitEvt, .callback=&onMouseExitCb);
    BS_SIGNAL_TX(MouseEvt);


    void ready()
    {
        UtilityFunctions::print("MouseDetector: Calling user ready...");

        if(area != nullptr)
        {

            this->addSignalSource<MouseEnterEvt>(area);
            this->addSignalSource<MouseExitEvt>(area);
        }
    }

};


// Declare a 'Mouse Trap', which waits for a signal from the 'Mouse Detector'.
class MouseTrap : public Node
{
    GDCLASS(MouseTrap, Node)
    BSCLASS;

    // Export the mouse detector for mutability within the editor.
    Node * mouseDetector = nullptr;
    BS_EXPORT(&MouseTrap::mouseDetector,.name="mouseDetector");

    //Define and export the 'MouseEvt' callback.
    //    Exporting is required for the callback to work.
    //    The callback and its arguments may be anonymous if you find that easier.
    //        The anonymous form is 'BS_EXPORT(&MouseTrap::mouseEvent);'
    void mouseEventCb(Object * obj, bool entered, int value)        
    {
        if(!entered)
        {
            return;
        }
        UtilityFunctions::print(
            "MouseTrap: I have received a message from the MouseDetector and am now closing the mouse trap, thus completing the mouse trap analogy - {",
            obj,entered,value,"}"
        );
    }
    BS_EXPORT(&MouseTrap::mouseEventCb, .name="mouseEvent",.args={"obj","entered","value"});

    //Link the 'mouseEventCb' to the signal.
    BS_SIGNAL_RX(MouseDetector::MouseEvt,.callback=&MouseTrap::mouseEventCb);

    //Add the mouse detector to our list of signal source for 'MouseEvt' events.
    void ready()
    {
        UtilityFunctions::print("MouseTrap: Calling user ready...");
        if(mouseDetector == nullptr)
        {
            godot::UtilityFunctions::print("Error: The member 'mouseDetector' within an instance of type 'MouseTrap' is null.");
            return;
        }
        this->addSignalSource<MouseDetector::MouseEvt>(mouseDetector);
    }
};



#endif



