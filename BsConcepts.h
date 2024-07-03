#ifndef BS_CONCEPTS_H
#define BS_CONCEPTS_H

#include <godot_cpp/classes/ref_counted.hpp>

namespace Bs
{
    namespace Gd {
        template<typename T> struct SRemoveGdRef{using Type = T;};
        template<typename T> struct SRemoveGdRef<godot::Ref<T>>{using Type = T;};
        template<typename T> struct SRemoveGdRef<const godot::Ref<T>>{using Type = T;};
        template<typename T> struct SRemoveGdRef<godot::Ref<T> &>{using Type = T;};
        template<typename T> struct SRemoveGdRef<const godot::Ref<T> &>{using Type = T;};
        template<typename T> struct SRemoveGdRef<godot::Ref<T> &&>{using Type = T;};
        template<typename T> struct SRemoveGdRef<const godot::Ref<T> &&>{using Type = T;};
        template<typename T> using RemoveGodotRef = SRemoveGdRef<T>::Type;
        template<typename T> using BaseType = ::Bs::BaseType<RemoveGodotRef<T>>;
        template<typename T, typename U> concept CGdSameBaseType = CIsSameBaseType<BaseType<T>,U>;
        template<typename T, typename U> concept CGdHasType = CIsSameBaseType<BaseType<T>,U>;

        template<typename T> concept CGdObject = std::derived_from<BaseType<T>,godot::Object>;
        template<typename T> concept CGdNode = std::derived_from<BaseType<T>,godot::Node>;
        template<typename T> concept CGdResource = std::derived_from<BaseType<T>,godot::Resource>;
        template<typename T> concept CGdRefCounted = std::derived_from<BaseType<T>,::godot::RefCounted>;

        template<typename T> concept CGdArray = CGdSameBaseType<T,godot::Array>;
        template<typename T> concept CGdTypedArray = SameTemplate<T,godot::TypedArray>::value;
        template<typename T> concept CGdVector = SameTemplate<T,godot::Vector>::value;
        template<typename T> concept CGdPrimative = (!CGdVector<T> && !CGdObject<T>);

    } //namespace Gd

    template<typename T> concept CBsUserType = requires(T x){x.BS_ARBITRARY_METHOD();};
    template<typename T> concept CBsEnumType = std::derived_from<T,BsEnum>;

} //namespace Bs
#endif