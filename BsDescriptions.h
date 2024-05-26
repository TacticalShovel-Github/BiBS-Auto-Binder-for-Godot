#ifndef BS_UTILS_H
#define BS_UTILS_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <concepts>
#include <algorithm>
#include <string_view>
#include <format>
#include <cassert>

namespace godot
{
    class Resource;
    class Object;
    class Node;
    class Array;
    template<typename T> class Ref;
}

#include <BsTemplateUtils.h>
#include <BsMacroUtils.h>
#include <BsVariantUtils.h>
#include <BsTagging.h>
#include <BsMetaTypes.h>

#include "BsSignal.h"


namespace Bs
{
    inline int constexpr ceStrLen(const char* str)
    {
        return *str ? 1 + ceStrLen(str + 1) : 0;
    }

    struct Default {};
    struct Override {};
    
    namespace Gd
    {
        class Friend
        {
        public:
            template<typename OBJ_T>
            static void bindAll()
            {
                LogBS::write("%s: Begin binding...",((godot::String)OBJ_T::get_class_static()).ascii().get_data());
                LogBS::write("    Binding methods...");
                OBJ_T::template ForEachTagIndex<OBJ_T::template METHOD>([]<typename T>()
                {
                    T::data.bindMethods();
                });
                LogBS::write("    Binding exports...");
                OBJ_T::template ForEachTagIndex<OBJ_T::template EXPORT>([]<typename T>(){
                    T::data.bindMethods();
                });
                LogBS::write("    Binding signals...");
                OBJ_T::template ForEachTagIndex<OBJ_T::template SIGNAL_TX>([]<typename T>(){
                    using SignalType = typename decltype(T::data)::SignalType;
                    SignalType::template signalOutBind<OBJ_T>();
                });
                LogBS::write("%s: Done binding...",((godot::String)OBJ_T::get_class_static()).ascii().get_data());
            }

            template<typename OBJ_T>
            static void readyTasks(OBJ_T * obj)
            {
                #if 0
                //Ensure exported pointers are zeroed at initialization.
                OBJ_T::template ForEachTagIndex<OBJ_T::template EXPORT>([&]<typename T>(){
                    if constexpr( decltype(T::data)::typeCode == Bs::EMetaType::MemberPointer)
                    {
                        if constexpr (std::is_pointer_v<typename decltype(T::data)::ValueType>)
                        {
                            auto ptr = decltype(T::data)::pointer;
                            obj->*ptr = nullptr;
                        }
                    }
                });
                #endif
            }

            template<typename OBJ_T> static void ready(OBJ_T & obj){}

            template<typename OBJ_T> requires requires(OBJ_T * x){x->ready();} static void callUserReady(OBJ_T * obj)
            {
                readyTasks<OBJ_T>(obj);
                obj->ready();
            }
            template<typename OBJ_T>                                           static void callUserReady(OBJ_T * obj){}

            template<typename OBJ_T> requires requires(){OBJ_T::bindMethods();}  static void callUserBindMethods(){OBJ_T::bindMethods();}
            template<typename OBJ_T> requires requires(){OBJ_T::bind_methods();} static void callUserBindMethods(){OBJ_T::bind_methods();}
            template<typename OBJ_T>                                             static void callUserBindMethods(){}
        };
        
        inline godot::String godotify(const std::string & str)
        {
            godot::String ret = str.c_str();
            return ret;
        }
    } /*namespace Gd*/

    #define BS_GD_DESCRIPTION_COMMON(SYMBOL) template<typename T=Default> consteval bool validate() const {return true;}

    #define BS_GD_MEMBER_COMMON(SYMBOL) \
        typedef decltype(SYMBOL)::ValueType   ValueType  ;\
        typedef decltype(SYMBOL)::ClassType   ClassType  ;\
        typedef decltype(SYMBOL)::PointerType PointerType;\
        static constexpr const Bs::EMetaType typeCode = decltype(SYMBOL)::typeCode;\
        const char * name;\
        godot::PropertyHint hintOverride = godot::PropertyHint::PROPERTY_HINT_NONE;\
        const char * hintStringOverride=""; \
        godot::PropertyUsageFlags additionUsageFlags = godot::PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;\
        static constexpr PointerType pointer = SYMBOL.value;

    #define BS_GD_BIND_METHODS(...)\
            const auto getterName = Bs::Gd::godotify(std::string("get") + name);\
            const auto setterName = Bs::Gd::godotify(std::string("set") + name);\
            godot::ClassDB::bind_method(godot::D_METHOD(getterName),&ClassType::template get<pointer>);\
            godot::ClassDB::bind_method(godot::D_METHOD(setterName),&ClassType::template set<pointer>);\
            godot::ClassDB::add_property(\
                ClassType::get_class_static(),\
                godot::PropertyInfo(\
                    Bs::Gd::VariantCodeFromType<ValueType>::value,\
                    name __VA_OPT__(,) \
                    __VA_ARGS__\
                ),\
                setterName, getterName\
            );
    
    template<typename ClassType, auto pointer>
    void bindMethods(
        const char * propertyName,
        const godot::PropertyHint & propertyHint,
        const char * hintString,
        const godot::PropertyUsageFlags usageFlags)
    {
        using ValueType = typename decltype(Bs::symbol<pointer>())::ValueType;
        const auto getterName = Bs::Gd::godotify(std::string("get") + propertyName);
        const auto setterName = Bs::Gd::godotify(std::string("set") + propertyName);
        godot::ClassDB::bind_method(godot::D_METHOD(getterName),&ClassType::template get<pointer>);
        godot::ClassDB::bind_method(godot::D_METHOD(setterName),&ClassType::template set<pointer>);
        godot::ClassDB::add_property(
            ClassType::get_class_static(),
            godot::PropertyInfo(
                Bs::Gd::VariantCodeFromType<ValueType>::value,
                propertyName,
                propertyHint,
                hintString,
                usageFlags
            ),
            setterName, getterName
        );
    }

    template<BS_TAG_TEMPLATE_PREAMBLE, auto SYMBOL>
    struct MemberDescription;

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL>
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        void bindMethods() const {
            BS_GD_BIND_METHODS();
            //bindMethods<ClassType,pointer>(name,hintOverride,hintStringOverride,additionUsageFlags);
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (Bs::Gd::CGdNode<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        void bindMethods() const {
            BS_GD_BIND_METHODS(
                godot::PropertyHint::PROPERTY_HINT_NODE_TYPE,
                this->hintStringOverride,
                this->additionUsageFlags
            );
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (Bs::Gd::CGdResource<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        void bindMethods() const
        {
            BS_GD_BIND_METHODS(
                godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
                Bs::Gd::BaseType<ValueType>::get_class_static()//,
//                this->additionUsageFlags
            );
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (Bs::Gd::CGdArray<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        using ArrayType = ValueType;
        using ArrayValueType = typename Bs::GetNthTemplateArg<0,ArrayType>::type;
        void bindMethods() const {
            BS_GD_BIND_METHODS(
            godot::PropertyHint::PROPERTY_HINT_ARRAY_TYPE,
            godot::vformat(
                "%s/%s:%s", 
                godot::Variant::OBJECT, 
                godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
                ArrayValueType::get_class_static()
            ),
            this->additionUsageFlags
        );}
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (std::is_arithmetic_v<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        ValueType max = std::numeric_limits<ValueType>::max();
        ValueType min = std::numeric_limits<ValueType>::min();
        void bindMethods() const
        {
            BS_GD_BIND_METHODS(
                this->hintOverride,
                this->hintStringOverride,
                this->additionUsageFlags
            );
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MethodPointer SYMBOL>
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        static constexpr const Bs::EMetaType typeCode = decltype(SYMBOL)::typeCode;
        static constexpr const auto MethodLabel = label("Method");
        static constexpr const auto DefaultName = MethodLabel.concatenated(label(Num2Str<index>::value));
        static constexpr const auto numParams = SYMBOL.numParams;
        static constexpr decltype(SYMBOL)::PointerType pointer = SYMBOL.value;

        const char * name = DefaultName.label;
        const std::array<const std::string_view,numParams> args;
        typename decltype(SYMBOL)::ArgTupleType defaultValues;

        template<typename T>
        consteval bool validate() const
        {
            std::size_t namedArguments = 0;
            Bs::ForEach<numParams>([&](auto i){
                if(args[i.value].size())
                {
                    namedArguments++;
                }
            });
            
            if(namedArguments > 0 && numParams == 0)
            {
                throw "A method with no parameters was given parameter names.";
            }
            if(namedArguments != 0 && namedArguments != numParams)
            {
                throw "A method is missing names for one or more of its parameters.";
            }
            return true;
        }
        
        void bindMethods() const 
        {
            if constexpr(numParams > 0)
            {
                [&]<typename T, T ... Is>(std::integer_sequence<T, Is...>){
                    godot::ClassDB::bind_method(godot::D_METHOD(name,args[Is].data() ...), pointer);
                }(std::make_integer_sequence<std::size_t,numParams>{});
            }
            else
            {
                godot::ClassDB::bind_method(godot::D_METHOD(name), pointer);
            }
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::FunctionPointer SYMBOL>
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        static constexpr decltype(SYMBOL)::PointerType pointer = SYMBOL.value;
        static constexpr const auto numParams = SYMBOL.numParams;
        using CONTAINING_CLASS_T = typename decltype(CONTAINING_CLASS)::type;
        static constexpr const auto MethodLabel = label("Method");
        static constexpr const auto DefaultName = MethodLabel.concatenated(label(Num2Str<index>::value));
        const char * name = DefaultName.label;
        const std::array<const std::string_view,numParams> args;
        void bindMethods() const
        {

            if constexpr(numParams > 0)
            {
                [&]<typename T, T ... Is>(std::integer_sequence<T, Is...>){
                    godot::ClassDB::bind_static_method(
                        CONTAINING_CLASS_T::get_class_static(),
                        godot::D_METHOD(name,args[Is].data() ...),
                        pointer
                    );
                }(std::make_integer_sequence<std::size_t,numParams>{});
            }
            else
            {
                godot::ClassDB::bind_static_method(
                    CONTAINING_CLASS_T::get_class_static(),
                    godot::D_METHOD(name),
                    pointer
                );
            }
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE,auto SYMBOL, auto CLASS_SYMBOL>
    struct SignalInDescription
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        using ClassType = typename decltype(CLASS_SYMBOL)::type;
        using SignalType = typename decltype(SYMBOL)::type;
        typename SignalType::TypeList::template MakeMethodPointer<void,ClassType> callback;
    };

    template<BS_TAG_TEMPLATE_PREAMBLE,auto SYMBOL>
    struct SignalOutDescription
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        using SignalType = typename decltype(SYMBOL)::type;
    };

    


} /*namespace Bs*/




#endif