#ifndef BS_META_TYPES
#define BS_META_TYPES

namespace Bs
{
    enum class EMetaType
    {
        FunctionPointer,
        MemberPointer,
        MethodPointer
    };
    /*
        FunctionPointerType - Yield the description of the provided function pointer type
    */
    template<typename Sig> struct FunctionPointerType;

    template<typename RET_T, typename...ARG_Ts>
    struct FunctionPointerType<RET_T (*)(ARG_Ts...)>
    {
        using PointerType = RET_T (*)(ARG_Ts...);
    	using ReturnType = RET_T;
        template<std::size_t I>
        using ParamType = typename GetNthType<I,ARG_Ts ...>::type;
        static constexpr const std::size_t numParams = sizeof...(ARG_Ts);
        static constexpr const EMetaType typeCode = EMetaType::FunctionPointer;
    };

    /*
        MemberPointerType - Yield the description of the provided member pointer type.
    */
    template<typename T> struct MemberPointerType;
    template <typename T,typename C> struct MemberPointerType< T C::* >
    {
        using ValueType = T;
        using ClassType = C;
        using PointerType = ValueType ClassType::*;

        static constexpr const EMetaType typeCode = EMetaType::MemberPointer;
    };

    #define BS_METHOD_POINTER_TYPE_COMMON \
        template<std::size_t I> \
        using ParamType = typename GetNthType<I,ARG_Ts ...>::type; \
        using ClassType = OBJ; \
        using ReturnType = RET_T; \
        static constexpr const std::size_t numParams = sizeof...(ARG_Ts); \
        static constexpr const EMetaType typeCode = EMetaType::MethodPointer; \
        typedef std::tuple<ARG_Ts ...> ArgTupleType;

    template<typename Sig>
    struct MethodPointerType;

    template<typename RET_T, typename OBJ, typename ... ARG_Ts>
    struct MethodPointerType<RET_T (OBJ::*)(ARG_Ts...)>
    {
        using PointerType = RET_T (OBJ::*)(ARG_Ts...);
        BS_METHOD_POINTER_TYPE_COMMON
    };
    template<typename RET_T, typename OBJ, typename ... ARG_Ts>
    struct MethodPointerType<RET_T (OBJ::*)(ARG_Ts...) const>
    {
        using PointerType = RET_T (OBJ::*)(ARG_Ts...) const;
        BS_METHOD_POINTER_TYPE_COMMON
    };



    template <std::size_t N>
    struct Literal
    {
        char label[N];
        constexpr Literal(const char (&str)[N]){std::copy_n(str, N, label);}
        constexpr Literal(const Literal<N> & other){std::copy_n(other.label, N, label);}
        constexpr Literal(Literal<N> && other){std::copy_n(other.label, N, label);}


        constexpr const auto & get() const {return label;}

        template<std::size_t M> requires (M < N)
        constexpr Literal<N-M> deletePrefix() const
        {
            char newLabel[N-M];
            for(std::size_t i = 0;i < N-M;i+=1)
            {
                newLabel[i]=label[M+i];
            }
            return Literal<M>(newLabel);
        }

        template<std::size_t M>
        constexpr Literal<M+N-1> concatenated(const Literal<M> & other) const
        {
            char newLabel[M+N-1];
            std::copy_n(label, N-1, newLabel);
            std::copy_n(other.label, M, &newLabel[N-1]);
            return Literal<M+N-1>(newLabel);
        }
    };

    template<typename T> struct TConstify{using type = T;};
    template<typename T> struct TConstify<const T>{using type = const T;};
    template<typename T> using Constify = TConstify<T>::type;

    template <typename T>
    struct Type
    {
        using type = T;
        constexpr Type(){}
        constexpr Type(const Type<T> & other){}
        constexpr Type(Type<T> && other){}
        template<typename OTHER_T>constexpr bool operator==(const OTHER_T & other) const
        {
            return false;
        }
        constexpr bool operator==(const Type<T> & other) const {return true;}
    };

    /*
        FunctionPointer - Stub
    */
    template<auto V>
    struct FunctionPointer : public FunctionPointerType<decltype(V)> {
        using MetaType = FunctionPointerType<decltype(V)>;
        template<std::size_t I>
        using ParamType = typename MetaType::ParamType<I>;
        using ReturnType = typename MetaType::ReturnType;
        using PointerType = typename MetaType::PointerType;
        static constexpr const std::size_t numParams = MetaType::numParams;
        static constexpr const PointerType value = V;
    };

    /*
        MethodPointer - Given the provided method pointer value, yield the description of that particular method pointer.
    */
    template <auto V> 
    struct MethodPointer : public MethodPointerType<decltype(V)>
    {
        constexpr MethodPointer(){}
        constexpr MethodPointer(const MethodPointer<V> & other){}
        constexpr MethodPointer(MethodPointer<V> && other){}
        template<std::size_t I>
        using ParamType = typename MethodPointerType<decltype(V)>::ParamType<I>;
        using MetaType = MethodPointerType<decltype(V)>;
        using ClassType = typename MetaType::ClassType;
        using ReturnType = typename MetaType::ReturnType;
        using PointerType = typename MetaType::PointerType;
        typedef typename MetaType::ArgTupleType ArgTupleType;
        static constexpr const std::size_t numParams = MethodPointerType<decltype(V)>::numParams;
        static constexpr PointerType value = V;

        template<typename OTHER_T> constexpr bool operator==(const OTHER_T & other) const
        {
            return false;
        }
        template<auto OV> requires (std::is_same_v<decltype(V),decltype(OV)>) constexpr bool operator==(const MethodPointer<OV> & other) const
        {
            return other.value == this->value;
        }
    };

    /*
        MemberPointer - Given the provided method pointer value, yield the description of that particular member pointer.
            Note member pointer here is non-standard terminology and refers to non-method, non-static class data.
    */
    template <auto V> 
    struct MemberPointer : public MemberPointerType<decltype(V)>
    {
        constexpr MemberPointer(){}
        constexpr MemberPointer(const MemberPointer<V> & other){}
        constexpr MemberPointer(MemberPointer<V> && other){}
        using MetaType = MemberPointerType<decltype(V)>;
        using ClassType = typename MemberPointerType<decltype(V)>::ClassType;
        using ValueType = typename MemberPointerType<decltype(V)>::ValueType;
        using PointerType = ValueType ClassType::*;
        static constexpr PointerType value = V;
        static const ValueType & get(const ClassType & v1){return v1.*value;}
        static void set(ClassType & v1,const ValueType & v2){v1.*value = v2;}
        std::size_t offset()
        {
            return 0;
        }
        template<typename OTHER_T> constexpr bool operator==(const OTHER_T & other) const
        {
            return false;
        }
        template<auto OV> requires (std::is_same_v<decltype(V),decltype(OV)>) constexpr bool operator==(const MemberPointer<OV> & other) const
        {
            return other.value == this->value;
        }
    };

    /*
        Parameter - Symbol, Describes a parameter with a name and type
    */
    template <typename T,std::size_t N>
    struct Parameter
    {
        using type = T;
        char label[N];
        
        constexpr Parameter()=delete;
        constexpr Parameter(const char (&str)[N]) {std::copy_n(str, N, label);}
        constexpr Parameter(const Parameter<T,N> & other){std::copy_n(other.label, N, label);}
        constexpr Parameter(Parameter<T,N> && other){std::copy_n(other.label, N, label);}
    };

    /*
        SymbolList - Describes a list of symbols
    */
    template<auto ... Vs>
    struct SymbolList
    {
        constexpr SymbolList(){}
        constexpr SymbolList(const SymbolList<Vs...> & other){}
        constexpr SymbolList(SymbolList<Vs...> && other){}

        template<auto ... OVs>
        constexpr bool operator==(const SymbolList<OVs...> & other) const
        {
            return 0;
        }
        template<auto ... OVs> requires (sizeof...(Vs) == sizeof...(OVs) && (((Vs == OVs) + ...) == sizeof...(Vs)) )
        constexpr bool operator==(const SymbolList<OVs...> & other) const
        {
            return 1;
        }
    };

    /*
        TypeList - Describes a list of types
    */
    template<typename ... Ts>
    struct TypeList
    {
        template<class RETURN_T, class CLASS_T>
        using MakeMethodPointer = RETURN_T (CLASS_T::*)(Ts ...);
    };

    /*
        Helper to create a label symbol.
    */
    template<std::size_t N>
    constexpr Literal<N> label(const char (&str)[N])
    {
        return Literal<N>(str);
    }

    /*
        Helper to create a param symbol.
    */
    template<typename T,std::size_t N>
    constexpr auto param(const char (&str)[N])
    {
        return Parameter<T,N>(str);
    }

    template<typename T,std::size_t N>
    constexpr auto param(T type, const char (&str)[N])
    {
        return Parameter<typename T::type,N>(str);
    }

    /*
        Helper to create a type symbol.
    */
    template<typename T>
    constexpr auto type()
    {
        return Type<T>();
    }

    /*
        Helper to create symbol, with symbol type inferred from the parameter.
    */
    template<typename T>
    constexpr auto symbol()
    {
        return type<T>();
    }

    template<auto V> requires std::is_member_object_pointer_v<decltype(V)>
    constexpr auto symbol()
    {
        return MemberPointer<V>();
    }

    template<auto V> requires std::is_member_function_pointer_v<decltype(V)>
    constexpr auto symbol()
    {
        return MethodPointer<V>();
    }

    template<auto V> requires std::is_function_v<typename std::remove_pointer<decltype(V)>::type>
    constexpr auto symbol()
    {
        return FunctionPointer<V>();
    }

    template<auto ... SYMBOLS>
    constexpr auto symbols()
    {
        return Bs::SymbolList<SYMBOLS ...>();
    }
}

#endif