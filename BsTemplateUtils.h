#ifndef BS_TEMPLATE_UTILS_H
#define BS_TEMPLATE_UTILS_H

namespace Bs
{
    /*
        getNthType - Given literal N, and typename variadic Ts, yield the the Nth typename of Ts.
    */
    namespace Dt
    {
        template<std::size_t n, typename T, typename ... Ts> struct GetNthType{typedef typename ::Bs::Dt::GetNthType<n-1,Ts...>::type type;};
        template<typename T, typename ... Ts> struct GetNthType<0,T,Ts...>{typedef T type;};
        template<typename T> struct GetNthType<0,T>{typedef T type;};
    }

    template<std::size_t n,typename ... Ts> struct GetNthType{using type = typename Dt::GetNthType<n,Ts...>::type;};
    template<> struct GetNthType<0>{using type = void;};


    /*
        getNth - Given literal N and auto variadic Vs, yield the Nth value of Vs.
    */
    template<std::size_t n, auto V, auto ... Vs> struct GetNth{constexpr const static auto value = GetNth<n-1,Vs...>::value;};
    template<auto V, auto ... Vs> struct GetNth<0,V,Vs...>{constexpr const static auto value = V;};
    template<auto V> struct GetNth<0,V>{constexpr const static auto value = V;};

    /*
        BaseType - Given typename T, yield T without qualifiers (const, volatile, reference, pointer). Only removes one level of indirection from pointers.
    */
    template<typename T> 
    struct BaseTypeImpl
    {
        using type = typename std::remove_cvref<
            typename std::remove_pointer<T>::type
        >::type;
    };
    template<typename T>
    using BaseType = typename BaseTypeImpl<T>::type;

    /*
        IsSameBaseType - Given typename T1 and typename T2, determine if the two are the same type, disregarding qualifiers per BaseType, described above.
    */
    template<typename T,typename U>
    struct SIsSameBaseType
    {
        const constexpr static bool value = std::is_same_v<BaseType<T>,BaseType<U>>;
    };
    template<typename T, typename U>
    using IsSameBaseType = SIsSameBaseType<T,U>::value;

    /*
        Transform - Given T1 and T2, yield T2 regardless of the input T1.
    */
    namespace Dt
    {
        template<typename T1,typename T2>
        struct Transform { using type = T2; };
    }

    template<typename T1,typename T2>
    using Transform = typename Dt::Transform<T1,T2>::type;


    /*
        GetNthTemplateArg - Given a template, yield the Nth type member of that template.
    */
    template<std::size_t,typename T> struct GetNthTemplateArg;
    template<std::size_t i,template <class ... ARG_Ts> class T,typename ... ARG_Ts> 
    struct GetNthTemplateArg<i,T<ARG_Ts...>>
    {
        using type = typename Bs::GetNthType<i,ARG_Ts...>::type;
    };

    /*
        CompileTimeNumber - Wrap me number, ye maties
    */
    template<int I>
    struct CompileTimeNumber {
        static const constexpr int value = I;
        operator int() { return value; }
    };

    /*
        SameTemplate - Given two types, yield true if they are instances of the same template
    */
    template <class U, template<class...> class M> struct SameTemplate : std::false_type{};
    template <template<class...> class M, class ... Ts> struct SameTemplate<M<Ts...>, M> : std::true_type{};

    /*
        ForEach - Given integer I, Repeat a lambda I times.
            This lambda must have 1 auto params which is set corresponding to the iteration.
    */
    template<int I, typename FNC_PTR_T> struct ForEachHelper { static constexpr void exec(FNC_PTR_T ptr) { ForEachHelper<I - 1, FNC_PTR_T>::exec(ptr); ptr(CompileTimeNumber<I - 1>{}); } };
    template<typename FNC_PTR_T> struct ForEachHelper<0, FNC_PTR_T> { static constexpr void exec(FNC_PTR_T ptr) {}; };

    template<int I, typename FNC_PTR_T>
    constexpr void ForEach(const FNC_PTR_T ptr)
    {
        ForEachHelper<I, FNC_PTR_T>::exec(ptr);
    }


    template<int I, typename FNC_PTR_T, typename T> struct ForEachTupleHelper           { static inline void exec(FNC_PTR_T ptr, T &v1) { ForEachTupleHelper<I-1, FNC_PTR_T, T>::exec(ptr, v1); ptr(v1[CompileTimeNumber<I-1>::value]); } };    
    template<typename FNC_PTR_T, typename T> struct ForEachTupleHelper<0, FNC_PTR_T, T> { static inline void exec(FNC_PTR_T ptr, T &v1) {}; };

    template<typename T, typename FNC_PTR_T>
    inline void ForEach(T &v1,const FNC_PTR_T ptr)
    {
        ForEachTupleHelper<std::tuple_size<T>::value, FNC_PTR_T, T>::exec(ptr, v1);
    }


    namespace Dt
    {
        template<unsigned ... digits>
        struct Num2StrImpl_2 { static const char value[]; };

        template<unsigned... digits>
        constexpr char Num2StrImpl_2<digits...>::value[] = {('0' + digits)..., 0};

        //sumLeft not zero, keep adding digits to the parameter list
        template<unsigned sumLeft, unsigned... digits>
        struct Num2StrImpl_1 : Num2StrImpl_1<sumLeft / 10, sumLeft % 10, digits...> {};

        //sumLeft is zero, convert the integer parameter list into a char array
        template<unsigned... digits>
        struct Num2StrImpl_1<0, digits...> : Num2StrImpl_2<digits...> {};
    }

    template<unsigned num>
    struct Num2Str : Dt::Num2StrImpl_1<num> {};
}


namespace Bs
{
    template<typename T>
    concept NumericType = std::floating_point<T> || std::integral<T>;
}


#endif