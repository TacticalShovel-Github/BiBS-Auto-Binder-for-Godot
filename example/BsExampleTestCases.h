#ifndef BS_EXAMPLE_TEST_CASES_H
#define BS_EXAMPLE_TEST_CASES_H

#define TESTING_BS 1
#define TESTING_METHOD_EXPORT       1&&TESTING_BS
#define TESTING_SIGNALS             1&&TESTING_BS
#define TESTING_MEMBER_EXPORT       1&&TESTING_BS
#define TESTING_ARRAY_MEMBER_EXPORT 1&&TESTING_METHOD_EXPORT&&TESTING_BS
#define TESTING_STATIC_MEMBERS      1&&TESTING_BS


#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "BsExampleCommon.h"
#include "BsExampleExternalSignals.h"


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
    void intMethod(int a){}
    BS_EXPORT(&intMethod, .name="intMethod", .args={"a"});

    //Export a simple method
    void voidMethod(){}
    BS_EXPORT(&voidMethod, .name="voidMethod");

    //Export an anonymous method with anonymous parameters
    void fullyAnonymousVoidMethod(){}
    BS_EXPORT(&fullyAnonymousVoidMethod);
    
    //Export a method with anonymous parameters
    void partiallyAnonymousMethod(int a){}
    BS_EXPORT(&partiallyAnonymousMethod, .name="partiallyAnonymousMethod");

    //Exporting overloaded methods - You need to explicitly type the member pointer to disambiguate the binding expression
    void exampleOverloadedMethod(int a,int b){ godot::UtilityFunctions::print("Called ExportTester::exampleOverloadedMethod(int a=",a,",int b=",b,")"); }
    void exampleOverloadedMethod(int a      ){ godot::UtilityFunctions::print("Called ExportTester::exampleOverloadedMethod(int a=",a            ,")"); }
    BS_EXPORT((void (ExportTester::*)(int,int))&ExportTester::exampleOverloadedMethod, .name="exampleOverloadedMethod1", .args={"a","b"});
    BS_EXPORT((void (ExportTester::*)(int    ))&ExportTester::exampleOverloadedMethod, .name="exampleOverloadedMethod2", .args={"a"    });
#endif

#if TESTING_STATIC_MEMBERS
    static int voidStaticFunction(){return 0;}
    BS_EXPORT(&ExportTester::voidStaticFunction, .name="voidStaticFunction");
    
    void voidAnonymousStaticFunction(){}
    BS_EXPORT(&voidAnonymousStaticFunction);

    static int namedStaticFunction(int dog, int cat){return 0;}
    BS_EXPORT(&ExportTester::namedStaticFunction, .name="namedStaticFunction", .args={"dog","cat"});

    static int anonymousParamsStaticFunction(int dog, int cat){return 0;}
    BS_EXPORT(&ExportTester::anonymousParamsStaticFunction, .name="anonymousParamsStaticFunction");

    static int partiallyAnonymousStaticIntIntFunction(int dog, int cat){return 0;}
    BS_EXPORT(&ExportTester::partiallyAnonymousStaticIntIntFunction, .name="partiallyAnonymousStaticIntIntFunction");
#endif

};


#endif //BS_EXAMPLE_H