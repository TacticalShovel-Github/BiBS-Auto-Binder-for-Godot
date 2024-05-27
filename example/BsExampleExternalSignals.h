#ifndef BS_EXAMPLE_EXTERNAL_SIGNALS_H
#define BS_EXAMPLE_EXTERNAL_SIGNALS_H

#include <godot_cpp/classes/collision_object3d.hpp>
#include "BiBS.h"

//
// Declaring non-BiBS Signals
//     BiBS does not include the definitions for signals defined without BiBS.
//         This includes native signals, as it is a maintainence issue to do so.
//         You may declare these signals to BiBS yourself though, per the syntax below.
//
// The signal name must exactly match the signal name shown on the node inspector.                  
//     If you give the signal a nickname, it won't work.
//     Parameters can be renamed with no issue.
//
// You do not need to declare every signal, just the one that you intend to use.
//

DECLARE_EXTERNAL_SIGNALS(godot::CollisionObject3D){
    using InputEvt = Bs::Signal<
        Bs::label("input_event"),
        Bs::param<godot::Camera3D *                    >("camera"),
        Bs::param<const godot::Ref<godot::InputEvent> &>("event"),
        Bs::param<const godot::Vector3 &               >("position"),
        Bs::param<const godot::Vector3 &               >("normal"),
        Bs::param<int32_t                              >("shapeId")
    >;

    using MouseExitEvt = Bs::Signal<
        Bs::label("mouse_exited")
    >;

    using MouseEnterEvt = Bs::Signal<
        Bs::label("mouse_entered")
    >;
};

#endif