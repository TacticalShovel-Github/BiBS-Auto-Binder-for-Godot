#ifndef BS_EXAMPLE_H
#define BS_EXAMPLE_H

#define TESTING_BS 1
#define TESTING_METHOD_EXPORT 1&&TESTING_BS
#define TESTING_SIGNALS 1&&TESTING_BS
#define TESTING_MEMBER_EXPORT 1&&TESTING_BS
#define TESTING_ARRAY_MEMBER_EXPORT 1&&TESTING_METHOD_EXPORT&&TESTING_BS
#define TESTING_STATIC_MEMBERS 1&&TESTING_BS


#include <godot_cpp/classes/Area3D.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "RdCommon.h"
#include "BiBS.h"
#include "RdBsSignals.h"

/*
    |===================================================================================================|
    | Declaring Native Signals                                                                          |
    |===================================================================================================|
    | BiBS does not include the definitions for native signals, as it is a maintainence issue to do so. |
    | You can decare them to BiBS yourself though, per the syntax below.                                |
    |                                                                                                   |
    | The signal name must exactly match the signal name shown on the node inspector.                   |
    | If you give the signal a nickname, it won't work.                                                 |
    |                                                                                                   |
    | Parameters can be renamed with no issue.                                                          |
    |                                                                                                   |
    | You do not need to declare every signal, just the one that you intend to use.                     |
    |===================================================================================================|
*/


DECLARE_EXTERNAL_SIGNALS(godot::CollisionObject3D){
    using InputEvt = Bs::Signal<
        Bs::label("input_event"),
        Bs::param<godot::Camera3D *                      >("camera"),
        Bs::param<const godot::Ref<::godot::InputEvent> &>("event"),
        Bs::param<const godot::Vector3 &                 >("position"),
        Bs::param<const godot::Vector3 &                 >("normal"),
        Bs::param<int32_t                                >("shapeId")
    >;

    using MouseExitEvt = Bs::Signal<
        Bs::label("mouse_exited")
    >;

    using MouseEnterEvt = Bs::Signal<
        Bs::label("mouse_entered")
    >;
};

/*
    I am aliasing these templates soley for aesthetic reasons.
    It would work the same if the typenames were pasted directly into the class.
*/
using MouseEnterEvt = Bs::Signals<godot::CollisionObject3D>::MouseEnterEvt;
using MouseExitEvt = Bs::Signals<godot::CollisionObject3D>::MouseExitEvt;

class MouseDetector : public godot::Node3D {
    GDCLASS(MouseDetector, godot::Node3D)
    BSCLASS;
public:
    //Declare a custom signal.
    using MouseEvt = Bs::Signal<
        Bs::label("MouseEvt"),
        Bs::param<godot::Object *>("obj"  ),
        Bs::param<bool           >("entered"),
        Bs::param<int            >("currentValue")
    >;

    int one;
    int two;
    BS_EXPORT(&MouseDetector::one,.name="one");
    BS_EXPORT(&MouseDetector::two,.name="two");

    godot::Area3D * area1 = nullptr;
    godot::Area3D * area2 = nullptr;
    godot::Area3D * area3 = nullptr;
    BS_EXPORT(&MouseDetector::area1,.name="MouseArea1");
    BS_EXPORT(&MouseDetector::area2,.name="MouseArea2");
    BS_EXPORT(&MouseDetector::area3,.name="MouseArea3");

    //Deduplicate code shared by the different callbacks
    void onMouseEnterOrExit(godot::Area3D * area, bool entered)
    {
        if(area == nullptr){return;}
        const std::string msg = 
            std::string("Mouse") + (entered ? " entered" : " exited") +
            " {Area3d: " + BS_GD_OBJECT_NAME(area) + "}" + 
            " while values was {int32_t: " + std::to_string(one+two) + "}";

        godot::UtilityFunctions::print( Bs::Gd::godotify(msg) );
        this->emit<MouseEvt>(area,false,this->one + this->two);
    }

    //Templated callback
    template<auto MEMBER_POINTER, bool ENTERED>
    void mouseEnterOrExitCb()
    {
        //Member pointers are a seldom used C++ feature.
        //They refer to a class member independent of any particular object.
        godot::Area3D * area = this->*MEMBER_POINTER;
        onMouseEnterOrExit(area,ENTERED);
    }

    //Normal callback
    void onMouseEnterCb(){onMouseEnterOrExit(area3,true);}
    void onMouseExitCb(){onMouseEnterOrExit(area3,false);}

    /*
        Demonstrate RX and TX signalling.

        BiBS doesn't care if you are giving it an normal method, or an instance of a template method.
        However, this is a small difference in syntax due to C++ rules, as you can see below.
    */
    BS_EXPORT(&(mouseEnterOrExitCb<&MouseDetector::area1,false>), .name="onMouseExitArea1");
    BS_EXPORT(&(mouseEnterOrExitCb<&MouseDetector::area2,false>), .name="onMouseExitArea2");
    BS_EXPORT(&(mouseEnterOrExitCb<&MouseDetector::area1,true>), .name="onMouseEnterArea1");
    BS_EXPORT(&(mouseEnterOrExitCb<&MouseDetector::area2,true>), .name="onMouseEnterArea2");
    BS_SIGNAL_RX(MouseExitEvt,  .callback=&(mouseEnterOrExitCb<&MouseDetector::area1,false>));
    BS_SIGNAL_RX(MouseEnterEvt, .callback=&(mouseEnterOrExitCb<&MouseDetector::area1,true>));
    BS_SIGNAL_RX(MouseExitEvt,  .callback=&(mouseEnterOrExitCb<&MouseDetector::area2,false>));
    BS_SIGNAL_RX(MouseEnterEvt, .callback=&(mouseEnterOrExitCb<&MouseDetector::area2,true>));
    BS_SIGNAL_RX(MouseExitEvt,  .callback=&onMouseEnterCb);
    BS_SIGNAL_RX(MouseEnterEvt, .callback=&onMouseExitCb);
    BS_SIGNAL_TX(MouseEvt);

    /*
        Optional ready method

        Equivalent to the _ready() we are used to.
        Since BSClass consumes the call to _ready, it allows you to define ready() for your purposes.
        
        The user defined ready() will run after BSCLASS does its initialization.
        Any variables or methods that have been BS_EXPORT'd should not be rebinded at this step.
    */
    void ready()
    {
        godot::UtilityFunctions::print("MouseDetector: Calling user ready...");

        if(area1 != nullptr)
        {
            this->addSignalSource<MouseExitEvt>(area1);
            this->addSignalSource<MouseEnterEvt>(area1);
        }
        if(area2 != nullptr)
        {
            this->addSignalSource<MouseExitEvt>(area2);
            this->addSignalSource<MouseEnterEvt>(area2);
        }
        if(area3 != nullptr)
        {
            this->addSignalSource<MouseExitEvt>(area3);
            this->addSignalSource<MouseEnterEvt>(area3);
        }
    }

    /*
        For manual binding, you may optionally declare one these methods, but not both.
        The choice between them is entirely aesthetic. It is just for people, like me, who prefer camel case.

        static void bindMethods(){};
        static void bind_methods(){};
    */
}; //Class MouseDetector


class MouseTrap : public godot::Node
{
    GDCLASS(MouseTrap, godot::Node)
    BSCLASS;

    godot::Node * mouseDetector = nullptr;
    BS_EXPORT(&MouseTrap::mouseDetector,.name="mouseDetector");

    void mouseEvent(godot::Object * obj, bool entered, int value)        
    {
        godot::UtilityFunctions::print(
            "MouseTrap: I am completing the mouse trap analogy - {",
            obj,entered,value,"}"
        );
    }
    BS_EXPORT(&MouseTrap::mouseEvent, .name="mouseEvent",.args={"obj","entered","value"});
    BS_SIGNAL_RX(MouseDetector::MouseEvt,.callback=&MouseTrap::mouseEvent);

    void ready()
    {
        if(mouseDetector == nullptr){return;}
        this->addSignalSource<MouseDetector::MouseEvt>(mouseDetector);
    }
}; //Class MouseTrap



class ExportTester : public godot::Node
{
    GDCLASS(ExportTester, godot::Node)
    BSCLASS;
    
#if TESTING_MEMBER_EXPORT
    int one;
    int two;
    BS_EXPORT(&ExportTester::one,.name="one");
    BS_EXPORT(&ExportTester::two,.name="two");

    //Some PropertyUsageFlags are not supported automatically, but you can add them in yourself.
    float readOnlyProperty = 3.14;
    BS_EXPORT(&ExportTester::readOnlyProperty,.name="readOnlyProperty",.additionUsageFlags = PROPERTY_USAGE_READ_ONLY);

    godot::Area3D * area1 = nullptr;
    godot::Area3D * area2 = nullptr;
    godot::Area3D * area3 = nullptr;
    BS_EXPORT(&ExportTester::area1,.name="MouseArea1");
    BS_EXPORT(&ExportTester::area2,.name="MouseArea2");
    BS_EXPORT(&ExportTester::area3,.name="MouseArea3");
#endif

#if TESTING_ARRAY_MEMBER_EXPORT
    //Declare a data tuple to be used to demonstrate POD arrays
    class ExampleTuple : public godot::Resource
    {
        GDCLASS(ExampleTuple, godot::Resource)
        BSCLASS;
        
        float first = 0.0, second = 0.0;
        BS_EXPORT(&ExampleTuple::first,.name="first");
        BS_EXPORT(&ExampleTuple::second,.name="second");
        /*Implicit _ready() and _bind_methods*/
    };

    //Export a tuple of objects
    godot::TypedArray<ExampleTuple> tupleArray;
    BS_EXPORT(&ExportTester::tupleArray,.name="tupleArray");
#endif

#if TESTING_METHOD_EXPORT
    //Export a simple method
    void exampleMethod(int a){}
    BS_EXPORT(&exampleMethod, .name="exampleMethod", .args={"a"});

    //Export an anonymous method
    void exampleAnonymousMethod(int a){}
    BS_EXPORT(&exampleAnonymousMethod, .args={"a"});
    
    //Export a method with anonymous parameters
    void exampleAnonymousParamsMethod(int a){}
    BS_EXPORT(&exampleAnonymousParamsMethod, .name="exampleAnonymousParamsMethod");

    //Exporting overloaded methods - You need to explicitly type the member pointer to disambiguate the binding expression
    void exampleOverloadedMethod(int a,int b){ godot::UtilityFunctions::print("Called ExportTester::exampleOverloadedMethod(int a=",a,",int b=",b,")"); }
    void exampleOverloadedMethod(int a      ){ godot::UtilityFunctions::print("Called ExportTester::exampleOverloadedMethod(int a=",a            ,")"); }
    BS_EXPORT((void (ExportTester::*)(int,int))&ExportTester::exampleOverloadedMethod, .name="exampleOverloadedMethod1", .args={"a","b"});
    BS_EXPORT((void (ExportTester::*)(int    ))&ExportTester::exampleOverloadedMethod, .name="exampleOverloadedMethod2", .args={"a"    });
#endif

#if TESTING_STATIC_MEMBERS
    static int voidStaticFunction(){return 0;}
    BS_EXPORT(&ExportTester::voidStaticFunction, .name="voidStaticFunction");
    
    static int voidAnonymousStaticFunction(){return 0;}
    BS_EXPORT(&ExportTester::voidAnonymousStaticFunction);

    static int namedStaticFunction(int dog, int cat){return 0;}
    BS_EXPORT(&ExportTester::namedStaticFunction, .name="namedStaticFunction", .args={"dog","cat"});

    static int anonymousParamsStaticFunction(int dog, int cat){return 0;}
    BS_EXPORT(&ExportTester::anonymousParamsStaticFunction, .name="anonymousParamsStaticFunction");

    static int anonymousStaticFunction(int dog, int cat){return 0;}
    BS_EXPORT(&ExportTester::anonymousStaticFunction, .name="anonymousStaticFunction");
#endif
};


#endif //BS_EXAMPLE_H