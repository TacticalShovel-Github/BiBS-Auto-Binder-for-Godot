#ifndef BIBS_H
#define BIBS_H

#ifdef BS_DEV_LOCAL
    //For my development, using my personal logging system.
    namespace Bs {
    using LogBS = Rd::LogAgent<Rd::label("bullshit.txt")>;
    }
#else
namespace Bs
{
    struct PhonyLogAgent
    {
        template<typename ... Ts> static void write(const char * fmt, const Ts & ...args){}
        template<typename ... Ts> static void writePartial(const char * fmt, const Ts & ...args){}
    };
    using LogBS = PhonyLogAgent;
}
#endif

#include "BsDescriptions.h"

/*
    React to godot interactions
*/
#define BSCLASS \
    BSCLASS_DETAILS \
    public: \
        BS_DECLARE_TAG(METHOD,Bs::MemberDescription); \
        BS_DECLARE_TAG(EXPORT,Bs::MemberDescription); \
        BS_DECLARE_TAG(SIGNAL_TX,Bs::SignalOutDescription); \
        BS_DECLARE_TAG(SIGNAL_RX,Bs::SignalInDescription); \
    private: \
    friend class Bs::Gd::Friend; \
    template <auto V> friend struct ::Bs::MemberPointer; \
    protected: \
    static void _bind_methods() { \
        Bs::Gd::Friend::bindAll<BS_SELF_TYPE>(); \
        Bs::Gd::Friend::callUserBindMethods<BS_SELF_TYPE>();\
    } \
    public: \
    void _ready() { Bs::Gd::Friend::ready<BS_SELF_TYPE>(*this); Bs::Gd::Friend::callUserReady<BS_SELF_TYPE>(this); }\
    template<typename SIGNAL_T,typename T> void addSignalSource(T * obj)\
    {\
        SIGNAL_T::bindCallback(obj,this);\
    }\
    template<typename SIGNAL_T,typename ... Ts> void emit(const Ts & ... ARGs)\
    {\
        SIGNAL_T::emit(this,ARGs...);\
    }


    #define BS_EXPORT(IDX,...)    BS_TAG(EXPORT,    BS_WRAP(Bs::symbol<IDX>()                           ),BS_WRAP(__VA_ARGS__))
    #define BS_SIGNAL_RX(IDX,...) BS_TAG(SIGNAL_RX, BS_WRAP(Bs::symbol<IDX>(),Bs::symbol<BS_SELF_TYPE>()),BS_WRAP(__VA_ARGS__))
    #define BS_SIGNAL_TX(IDX)     BS_TAG(SIGNAL_TX, BS_WRAP(Bs::symbol<IDX>()                           ),BS_WRAP(           ))
    #define BS_UNHANDLED_INPUT(FNC_PTR) virtual void _unhandled_input(const godot::Ref<godot::InputEvent> &event) override { (*this.*(FNC_PTR))(event);}
    #define BS_GD_OBJECT_NAME(OBJ) (((godot::String)OBJ->get_name()).ascii().get_data())

#endif