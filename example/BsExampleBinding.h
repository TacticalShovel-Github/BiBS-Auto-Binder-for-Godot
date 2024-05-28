#ifndef BS_EXAMPLE_BINDING_H
#define BS_EXAMPLE_BINDING_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/Area3D.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/ref.hpp>
#include "BsExampleCommon.h"

/*
    BiBS can export any data member whose type is assignable to godot::Variant.

    These include..
        (1.) Any C++ numeric type
        (2.) Any class derived from godot::Object, including
            (a.) nodes
            (b.) resources
            (c.) objects
        (4.) Godot string classes such, including
            (a.) godot::StringName
            (b.) godot::String
            (c.) godot::NodePath
        (5.) Arrays
        
        .. and so forth.

    You may assume this full list from the labels given by the enum, godot::Variant::Type.
*/
    // Any class marked derived from godot::Object can be marked as BSCLASS, not just nodes.
    //     The same rules for what you can and cannot export also apply here.
    class ExampleResourceTuple : public godot::Resource
    {
        GDCLASS(ExampleResourceTuple, godot::Resource)
        BSCLASS;
        
        float first = 0.0, second = 0.0;
        BS_EXPORT(&ExampleResourceTuple::first,.name="first");
        BS_EXPORT(&ExampleResourceTuple::second,.name="second");
    };

class DataMemberBindingExample : public godot::Node
{
     GDCLASS(DataMemberBindingExample, godot::Node) 
     BSCLASS;

    // Most primatives types can be exported.
    float first = 0.0;
    uint64_t second = 0.0;
    bool third = 0.0;
    BS_EXPORT(&DataMemberBindingExample::first,.name="first");
    BS_EXPORT(&DataMemberBindingExample::second,.name="second");
    BS_EXPORT(&DataMemberBindingExample::third,.name="second");

   // Nodes are derived from godot::Object, so they may be exports.
   //     The type must appear as a pointer type.
   //     This member must be default initialized to nullptr.
   //         If you do not do this last part, the editor will crash.
    godot::Area3D * area = nullptr;
    BS_EXPORT(&DataMemberBindingExample::area,.name="Area");


    
    // Our resource tuple is derived from godot::Object, so we may export it.
    ExampleResourceTuple * resourceMember = nullptr;
    BS_EXPORT(&DataMemberBindingExample::resourceMember,.name="resourceMember");

    // You may also export a godot::Array. 
    // Unfortunately, default initializing it is a bit silly.
    godot::Array gdArray = [](){
        godot::Array ret;
        for(int i=0;i<256;i++)
            ret.push_back(i); 
        return ret;
    }();

    // godot::TypedArray is derived from godot::Array, which is convertible to godot::Variant, so we may export it.
    //godot::TypedArray<ExampleResourceTuple> tupleArray;
    //BS_EXPORT(&DataMemberBindingExample::tupleArray,.name="tupleArray");
};



/*
    Likewise, BiBS can export member functions (method).
        This is true, provided that each parameter and return type is assignable to godot::Variant.
        Generally, if a type may be exported, it may also appear as a parameter on a method binding.
*/


class MethodBindingExample : public godot::Node
{
    GDCLASS(MethodBindingExample, godot::Node) 
    BSCLASS;
     
    void inputEventCb(
        godot::Camera3D *camera,
        const godot::Ref<godot::InputEvent> &evt,
        const godot::Vector3 &pos,
        const godot::Vector3 &nrm,
        int32_t shp){}
    BS_EXPORT(&MethodBindingExample::inputEventCb, .name="inputEvent", .args={"cam","evt","pos","nrm","shp"});

    static int namedStaticFunction(int dog, int cat){return 0;}
    BS_EXPORT(&MethodBindingExample::namedStaticFunction, .name="namedStaticFunction", .args={"dog","cat"});
};

#endif