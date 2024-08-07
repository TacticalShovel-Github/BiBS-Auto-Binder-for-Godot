#ifndef BS_DESCRIPTION_H
#define BS_DESCRIPTION_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include <concepts>
#include <algorithm>
#include <string_view>
#include <format>
#include <cassert>
#include <fstream>
#include <sstream>

namespace godot
{
    class Resource;
    class Object;
    class Node;
    class Array;
    template<typename T> class Ref;
}


#include <BsTemplateUtils.h>
#include <BsConcepts.h>
#include <BsMacroUtils.h>
#include <BsMetaTypes.h>
#include <BsLogging.h>
#include <BsVariantUtils.h>
#include <BsTagging.h>
#include <BsSignal.h>
#include <BsContainers.h>


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
    inline std::string toStd(const godot::String & str)
    {
        std::string ret = str.ascii().get_data();
        return ret;
    }
    inline std::string toStd(godot::String && str)
    {
        std::string ret = str.ascii().get_data();
        return ret;
    }
    
    #define BS_GD_DESCRIPTION_COMMON(SYMBOL) template<typename T=Default> consteval bool validate() const {return true;}

    #define BS_GD_MEMBER_COMMON(SYMBOL) \
        typedef typename decltype(SYMBOL)::ValueType   ValueType  ;\
        typedef typename decltype(CONTAINING_CLASS)::type   ClassType  ;\
        typedef typename decltype(SYMBOL)::PointerType PointerType;\
        const char * name="";\
        godot::PropertyHint hintOverride = godot::PropertyHint::PROPERTY_HINT_NONE;\
        const char * hintStringOverride=""; \
        godot::PropertyUsageFlags additionUsageFlags = godot::PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;\
        static constexpr PointerType pointer = SYMBOL.value;

    template<typename ClassType, auto pointer>
    void bindPropertyShared(
        const char * propertyName,
        const godot::PropertyHint & propertyHint,
        const char * hintString,
        const godot::PropertyUsageFlags usageFlags)
    {
        using PtrClassType = typename decltype(Bs::symbol<pointer>())::ClassType;
        using ValueType = typename decltype(Bs::symbol<pointer>())::ValueType;
        static_assert(Bs::CIsSameBaseType<ClassType,PtrClassType>,"Member argument is of the wrong class type. Did you forget to change the class in the scope resolution operator?");
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

    template<typename ClassType, auto pointer, typename GET_FNC_T, typename SET_FNC_T>
    void bindPropertySharedGetSet(
        const char * propertyName,
        const godot::PropertyHint & propertyHint,
        const char * hintString,
        const godot::PropertyUsageFlags usageFlags,
        GET_FNC_T getFnPtr,
        SET_FNC_T setFnPtr) 
    {
        using ValueType = typename decltype(Bs::symbol<pointer>())::ValueType;
        const auto getterName = Bs::Gd::godotify(std::string("get") + propertyName);
        const auto setterName = Bs::Gd::godotify(std::string("set") + propertyName);
        godot::ClassDB::bind_method(godot::D_METHOD(getterName),getFnPtr);
        godot::ClassDB::bind_method(godot::D_METHOD(setterName),setFnPtr);
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
    
    template<typename ClassType, auto pointer, typename GET_TERN_T, typename SET_TERN_T>
    void bindPropertySharedTern(
        const char * propertyName,
        const godot::PropertyHint & propertyHint,
        const char * hintString,
        const godot::PropertyUsageFlags usageFlags,
        GET_TERN_T getPtr,
        SET_TERN_T setPtr)
    {
        if(getPtr == PsuedoValue::PS_DEFINED && setPtr == PsuedoValue::PS_DEFINED)
        {
            bindPropertySharedGetSet<ClassType,pointer>(
                propertyName,
                propertyHint,
                hintString,
                usageFlags,
                getPtr.get(),setPtr.get());
        }
        else if(getPtr == PsuedoValue::PS_DEFINED)
        {
            bindPropertySharedGetSet<ClassType,pointer>(
                propertyName,
                propertyHint,
                hintString,
                usageFlags,
                getPtr.get(),
                &ClassType::template set<pointer>);
        }
        else if(setPtr == PsuedoValue::PS_DEFINED)
        {
            bindPropertySharedGetSet<ClassType,pointer>(
                propertyName,
                propertyHint,
                hintString,
                usageFlags,
                &ClassType::template get<pointer>,
                setPtr.get());
        }
        else
        {
            bindPropertySharedGetSet<ClassType,pointer>(
                propertyName,
                propertyHint,
                hintString,
                usageFlags,
                &ClassType::template get<pointer>,
                &ClassType::template set<pointer>
            );
        }
    }


    template<BS_TAG_TEMPLATE_PREAMBLE, auto SYMBOL>
    struct MemberDescription;

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL>
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        Ternary<decltype(&ClassType::template get<pointer>)> getPtr;
        Ternary<decltype(&ClassType::template set<pointer>)> setPtr;
        void bindMethods() const {
            bindPropertySharedTern<ClassType,SYMBOL.value>(
                name,hintOverride,
                hintStringOverride,
                additionUsageFlags,
                getPtr,setPtr);
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (Bs::Gd::CGdNode<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        using NodeValueType = typename Bs::BaseType< typename decltype(SYMBOL)::ValueType >;
        void bindMethods() const {
            bindPropertyShared<ClassType,SYMBOL.value>(
                name,
                godot::PropertyHint::PROPERTY_HINT_NODE_TYPE,
                Bs::toStd(NodeValueType::get_class_static()).c_str(),
                additionUsageFlags
            );
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (Bs::Gd::CGdResource<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        Ternary<decltype(&ClassType::template get<pointer>)> getPtr;
        Ternary<decltype(&ClassType::template set<pointer>)> setPtr;
        void bindMethods() const
        {
//            bindPropertyShared<ClassType,SYMBOL.value>(
//                name,
//                godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
//                Bs::toStd(Bs::Gd::BaseType<ValueType>::get_class_static()).c_str(),
//                this->additionUsageFlags
//            );
            bindPropertySharedTern<ClassType,SYMBOL.value>(
                name,
                godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
                Bs::toStd(Bs::Gd::BaseType<ValueType>::get_class_static()).c_str(),
                this->additionUsageFlags,
                getPtr,
                setPtr);
        }
    };

//    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (
//        Gd::CGdArray<typename decltype(SYMBOL)::ValueType>)
//    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
//    {
//        BS_GD_DESCRIPTION_COMMON(SYMBOL);
//        BS_GD_MEMBER_COMMON(SYMBOL);
//        void bindMethods() const {}
//    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (
        Gd::CGdTypedArray<typename decltype(SYMBOL)::ValueType> || 
        Gd::CGdVector<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        using ArrayType = ValueType;

        void bindMethods() const {
            if constexpr (Bs::CIsSameBaseType<ArrayType,godot::Array> != true /*We are a typed array*/)
            {
                using ArrayValueType = typename Bs::GetNthTemplateArg<0,ArrayType>::type;
                if constexpr(Bs::Gd::CGdResource<ArrayValueType>)
                {
                    LogBS::writePartial("    Binding Resource Array Member (ArrayVT: %s, Member Id %llu).. ",
                        Bs::toStd(ArrayValueType::get_class_static()).c_str(),
                        index);
                        
                    bindPropertyShared<ClassType,SYMBOL.value>(
                        name,
                        godot::PropertyHint::PROPERTY_HINT_ARRAY_TYPE,
                        Bs::toStd(godot::vformat(
                        "%s/%s:%s", 
                            godot::Variant::OBJECT,
                            godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
                            Bs::toStd(ArrayValueType::get_class_static()).c_str()
                        )).c_str(),
                        this->additionUsageFlags
                    );
                    LogBS::write("done");
                }
                else if constexpr(Bs::Gd::CGdNode<ArrayValueType>)
                {
                    LogBS::writePartial("    Binding Node Array Member (ArrayVT: %s, Member Id %llu).. ",
                        Bs::toStd(ArrayValueType::get_class_static()).c_str(),
                        index);
                    
                    bindPropertyShared<ClassType,SYMBOL.value>(
                        name,
                        godot::PropertyHint::PROPERTY_HINT_TYPE_STRING,
                        Bs::toStd(
                            godot::vformat(
                                "%s/%s:%s", 
                                godot::Variant::OBJECT,
                                godot::PropertyHint::PROPERTY_HINT_NODE_TYPE,
                                Bs::toStd(ArrayValueType::get_class_static()).c_str())
                        ).c_str(),
                        this->additionUsageFlags
                    );
                    LogBS::write("done");
                }
                return;
            }

            LogBS::writePartial("    Binding Variant Array Member (ArrayVT: Variant, Member Id %llu).. ",
                index);

            //Default case for arrays
            bindPropertyShared<ClassType,SYMBOL.value>(
                name,
                godot::PropertyHint::PROPERTY_HINT_ARRAY_TYPE,
                Bs::toStd(godot::vformat(
                    "%s/%s:%s", 
                    Bs::Gd::VariantCodeFromType<ValueType>::value,
                    godot::PropertyHint::PROPERTY_HINT_NONE,
                    "value"
                )).c_str(),
                this->additionUsageFlags
            );
        }
    };
    
    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (CBsEnumType<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        
        void bindMethods() const
        {
            using PtrClassType = typename decltype(Bs::symbol<pointer>())::ClassType;
            using ValueType = typename decltype(SYMBOL)::ValueType;
            const char * propertyName = name;
            std::string hintString;
            if(!ValueType::Strings.empty())
            {
                for(const auto & enumName : ValueType::Strings)
                {
                    hintString += enumName;
                    hintString += ",";
                }
                hintString.pop_back();
            }
            
            constexpr const auto pointer = SYMBOL.value;
            static_assert(Bs::CIsSameBaseType<ClassType,PtrClassType>,"Member argument is of the wrong class type. Did you forget to change the class in the scope resolution operator?");
            const auto getterName = Bs::Gd::godotify(std::string("get") + propertyName);
            const auto setterName = Bs::Gd::godotify(std::string("set") + propertyName);
            godot::ClassDB::bind_method(godot::D_METHOD(getterName),&ClassType::template get2<pointer,size_t>);
            godot::ClassDB::bind_method(godot::D_METHOD(setterName),&ClassType::template set2<pointer,size_t>);
            godot::ClassDB::add_property(
                ClassType::get_class_static(),
                godot::PropertyInfo(
                    Bs::Gd::VariantCodeFromType<ValueType>::value,
                    propertyName,
                    godot::PropertyHint::PROPERTY_HINT_ENUM,
                    Gd::godotify(hintString),
                    this->additionUsageFlags
                ),
                setterName, getterName
            );
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MemberPointer SYMBOL> requires (std::is_arithmetic_v<typename decltype(SYMBOL)::ValueType>)
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        BS_GD_DESCRIPTION_COMMON(SYMBOL);
        BS_GD_MEMBER_COMMON(SYMBOL);
        static constexpr const auto defaultIncrement = GetNth<0,0.01,1>::value;
        const Ternary<ValueType> min = Ternary<ValueType>::UNDEFINED;
        const Ternary<ValueType> max = Ternary<ValueType>::UNDEFINED;
        const typename GetNthType<std::is_floating_point_v<ValueType>,float,float>::type increment = GetNth<std::is_floating_point_v<ValueType>,0.1f,0.01f>::value;
        Ternary<decltype(&ClassType::template get<pointer>)> getPtr;
        Ternary<decltype(&ClassType::template set<pointer>)> setPtr;

        void bindMethods() const
        {
            if(max == max.DEFINED || min == min.DEFINED)
            {
                const ValueType propMax = (max == max.DEFINED) ? (ValueType)max : std::numeric_limits<ValueType>::max();
                const ValueType propMin = (min == min.DEFINED) ? (ValueType)min : std::numeric_limits<ValueType>::min();
                
                bindPropertySharedTern<ClassType,SYMBOL.value>(
                    name,hintOverride,
                    std::format(
                        "{},{},{},or_greater,or_less", 
                        propMin,
                        propMax,
                        increment
                    ).data(),
                    additionUsageFlags,
                    getPtr,setPtr);
            }
            else
            {
                bindPropertySharedTern<ClassType,SYMBOL.value>(
                    name,hintOverride,
                    hintStringOverride,
                    additionUsageFlags,
                    getPtr,setPtr);
            }
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE, Bs::MethodPointer SYMBOL>
    struct MemberDescription<BS_TAG_TEMPLATE_PREAMBLE_NAMES,SYMBOL>
    {
        static constexpr const auto MethodLabel = label("Method");
        static constexpr const auto DefaultName = MethodLabel.concatenated(label(Num2Str<index>::value));
        static constexpr const auto numParams = SYMBOL.numParams;
        static constexpr typename decltype(SYMBOL)::PointerType pointer = SYMBOL.value;

        const char * name = DefaultName.label;
#ifdef _MSC_VER
        const std::array<const std::string_view,numParams> args={""};
#else
        const std::array<const std::string_view,numParams> args;
#endif

        template<typename T>
        consteval bool validate() const
        {
            using ContainingClass = typename decltype(CONTAINING_CLASS)::type;
            using MemberClass = typename decltype(SYMBOL)::ClassType;
            static_assert(Bs::CIsSameBaseType<MemberClass,ContainingClass>,"Member argument is of the wrong class type. Did you forget to change the class in the scope resolution operator?");

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
        static constexpr const auto MethodLabel = label("Method");
        static constexpr const auto DefaultName = MethodLabel.concatenated(label(Num2Str<index>::value));
        static constexpr const auto numParams = SYMBOL.numParams;
        static constexpr typename decltype(SYMBOL)::PointerType pointer = SYMBOL.value;

        const std::string_view name = DefaultName.label;
#ifdef _MSC_VER
        const std::array<const std::string_view,numParams> args={""};
#else
        const std::array<const std::string_view,numParams> args;
#endif


        using CONTAINING_CLASS_T = typename decltype(CONTAINING_CLASS)::type;
        template<typename T>
        consteval bool validate() const
        {
            return true;
        }
        void bindMethods() const
        {

            if constexpr(numParams > 0)
            {
                [&]<typename T, T ... Is>(std::integer_sequence<T, Is...>){
                    godot::ClassDB::bind_static_method(
                        CONTAINING_CLASS_T::get_class_static(),
                        godot::D_METHOD(name.data(),args[Is].data() ...),
                        pointer
                    );
                }(std::make_integer_sequence<std::size_t,numParams>{});
            }
            else
            {
                godot::ClassDB::bind_static_method(
                    CONTAINING_CLASS_T::get_class_static(),
                    godot::D_METHOD(godot::StringName(name.data())),
                    pointer
                );
            }
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE,auto SYMBOL, auto CLASS_SYMBOL>
    struct SignalInDescription
    {
        using ClassType = typename decltype(CLASS_SYMBOL)::type;
        using SignalType = typename decltype(SYMBOL)::type;
        using MethodPtrType = typename SignalType::TypeList::template MakeMethodPointer<void,ClassType>;
        MethodPtrType callback;

        using PtrClassType = typename decltype(Bs::symbol<MethodPtrType{}>())::ClassType;
        using ContainingClassType = typename decltype(CONTAINING_CLASS)::type;

        template<typename T>
        consteval bool validate() const
        {
            static_assert(Bs::CIsSameBaseType<PtrClassType,ContainingClassType>,
                "Type mismatch on callback for BS_SIGNAL_IN declaration.");
            return true;
        }
    };

    template<BS_TAG_TEMPLATE_PREAMBLE,auto SYMBOL>
    struct SignalOutDescription
    {
        using SignalType = typename decltype(SYMBOL)::type;

        template<typename T>
        consteval bool validate() const
        {
            return true;
        }
    };

    


} /*namespace Bs*/



#endif