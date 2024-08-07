#ifndef BS_MACRO_UTILS_H
#define BS_MACRO_UTILS_H

#define BS_PP_EMPTY
#define BS_PP_GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,RET,...) RET

/*
    COUNT - Give the number of arguments provided as a literal of type int
*/
#define BS_COUNT_IMPL(_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,_1,RET,...) RET
#define BS_COUNT(...) BS_COUNT_IMPL(__VA_ARGS__ __VA_OPT__(,)15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

/*
    TSTR - Convert arguments to string literal
*/
#define BS_TSTR_IMPL(...) #__VA_ARGS__
#define BS_TSTR(...) BS_TSTR_IMPL(__VA_ARGS__)


/*
    Concatenate together the provided arguements
*/
#define BS_PP_CAT_0(...)
#define BS_PP_CAT_1(x) x
#define BS_PP_CAT_2_IMPL(x,y) x##y
#define BS_PP_CAT_2(x,y) BS_PP_CAT_2_IMPL(x,y)
#define BS_PP_CAT_3(x,...) BS_PP_CAT_2(x,BS_PP_CAT_2(__VA_ARGS__))
#define BS_PP_CAT_4(x,...) BS_PP_CAT_2(x,BS_PP_CAT_3(__VA_ARGS__))
#define BS_PP_CAT_5(x,...) BS_PP_CAT_2(x,BS_PP_CAT_4(__VA_ARGS__))
#define BS_PP_CAT_6(x,...) BS_PP_CAT_2(x,BS_PP_CAT_5(__VA_ARGS__))
#define BS_PP_CAT_7(x,...) BS_PP_CAT_2(x,BS_PP_CAT_6(__VA_ARGS__))
#define BS_PP_CAT_8(x,...) BS_PP_CAT_2(x,BS_PP_CAT_7(__VA_ARGS__))
#define BS_PP_CAT_9(x,...) BS_PP_CAT_2(x,BS_PP_CAT_8(__VA_ARGS__))
#define BS_PP_CAT_10(x,...) BS_PP_CAT_2(x,BS_PP_CAT_9(__VA_ARGS__))
#define BS_PP_CAT_11(x,...) BS_PP_CAT_2(x,BS_PP_CAT_10(__VA_ARGS__))
#define BS_PP_CAT_12(x,...) BS_PP_CAT_2(x,BS_PP_CAT_11(__VA_ARGS__))
#define BS_PP_CAT_13(x,...) BS_PP_CAT_2(x,BS_PP_CAT_12(__VA_ARGS__))
#define BS_PP_CAT_14(x,...) BS_PP_CAT_2(x,BS_PP_CAT_13(__VA_ARGS__))
#define BS_PP_CAT_15(x,...) BS_PP_CAT_2(x,BS_PP_CAT_14(__VA_ARGS__))
#define BS_CAT(...) \
    BS_PP_GET_MACRO(__VA_ARGS__ __VA_OPT__(,) \
    BS_PP_CAT_15, \
    BS_PP_CAT_14, \
    BS_PP_CAT_13, \
    BS_PP_CAT_12, \
    BS_PP_CAT_11, \
    BS_PP_CAT_10, \
    BS_PP_CAT_9, \
    BS_PP_CAT_8, \
    BS_PP_CAT_7, \
    BS_PP_CAT_6, \
    BS_PP_CAT_5, \
    BS_PP_CAT_4, \
    BS_PP_CAT_3, \
    BS_PP_CAT_2, \
    BS_PP_CAT_1, \
    BS_PP_CAT_0 \
    )(__VA_ARGS__)


/*
    FOR_EACH(M,ARGs) - Repeat the macro M(X,I) over X=ARGs, where I is the index of the X in ARGs.
*/
#define BS_PP_FE_00(action)             
#define BS_PP_FE_01(action, A)                                                                             action(0,A)
#define BS_PP_FE_02(action, A,B)                           BS_PP_FE_01(action,A)                           action(1,B)
#define BS_PP_FE_03(action, A,B,C)                         BS_PP_FE_02(action,A,B)                         action(2,C)
#define BS_PP_FE_04(action, A,B,C,D)                       BS_PP_FE_03(action,A,B,C)                       action(3,D)
#define BS_PP_FE_05(action, A,B,C,D,E)                     BS_PP_FE_04(action,A,B,C,D)                     action(4,E)
#define BS_PP_FE_06(action, A,B,C,D,E,F)                   BS_PP_FE_05(action,A,B,C,D,E)                   action(5,F)
#define BS_PP_FE_07(action, A,B,C,D,E,F,G)                 BS_PP_FE_06(action,A,B,C,D,E,F)                 action(6,G)
#define BS_PP_FE_08(action, A,B,C,D,E,F,G,H)               BS_PP_FE_07(action,A,B,C,D,E,F,G)               action(7,H)
#define BS_PP_FE_09(action, A,B,C,D,E,F,G,H,I)             BS_PP_FE_08(action,A,B,C,D,E,F,G,H)             action(8,I)
#define BS_PP_FE_10(action, A,B,C,D,E,F,G,H,I,J)           BS_PP_FE_09(action,A,B,C,D,E,F,G,H,I)           action(9,J)
#define BS_PP_FE_11(action, A,B,C,D,E,F,G,H,I,J,K)         BS_PP_FE_10(action,A,B,C,D,E,F,G,H,I,J)         action(10,K)
#define BS_PP_FE_12(action, A,B,C,D,E,F,G,H,I,J,K,L)       BS_PP_FE_11(action,A,B,C,D,E,F,G,H,I,J,K)       action(11,L)
#define BS_PP_FE_13(action, A,B,C,D,E,F,G,H,I,J,K,L,M)     BS_PP_FE_12(action,A,B,C,D,E,F,G,H,I,J,K,L)     action(12,M)
#define BS_PP_FE_14(action, A,B,C,D,E,F,G,H,I,J,K,L,M,N)   BS_PP_FE_13(action,A,B,C,D,E,F,G,H,I,J,K,L,M)   action(13,N)
#define BS_PP_FE_15(action, A,B,C,D,E,F,G,H,I,J,K,L,M,N,O) BS_PP_FE_14(action,A,B,C,D,E,F,G,H,I,J,K,L,M,N) action(14,O)

#define BS_FOR_EACH(action,...)  \
    BS_PP_GET_MACRO( \
    __VA_ARGS__ __VA_OPT__(,) \
    BS_PP_FE_15, \
    BS_PP_FE_14, \
    BS_PP_FE_13, \
    BS_PP_FE_12, \
    BS_PP_FE_11, \
    BS_PP_FE_10, \
    BS_PP_FE_09, \
    BS_PP_FE_08, \
    BS_PP_FE_07, \
    BS_PP_FE_06, \
    BS_PP_FE_05, \
    BS_PP_FE_04, \
    BS_PP_FE_03, \
    BS_PP_FE_02, \
    BS_PP_FE_01, \
    BS_PP_FE_00)(action,__VA_ARGS__)

/*
    WRAP(ARGs) - Wrap the provided arguments, creating the preprocessing equivalent of an array.
    Once wrapped, ARGs will not expand to a variadic in subsequent macro calls. 
*/
#define BS_WRAP(...) (__VA_ARGS__)

/*
    UNWRAP(ARG) - Unwrap the provided argument, yielding the list that was originally provided.
    Inverse operation of WRAP.
*/
#define BS_PP_UNWRAP_IMPL_1(...) __VA_ARGS__
#define BS_PP_UNWRAP_IMPL_0(X,...) X __VA_ARGS__
#define BS_UNWRAP(X) BS_PP_UNWRAP_IMPL_0(BS_PP_UNWRAP_IMPL_1,X)

/*
    BS_PP_ASSIGN_RHS_TYPE(X) - Get the type of the right hand side of an (non-initializing) assignment expression.
        Requires that RHS is copy constructible
*/
#define BS_PP_ASSIGN_RHS_TYPE(...) decltype([](){auto __VA_ARGS__; auto ret = __VA_ARGS__; return ret;}())

/*
    BS_UNPACK(ARGs) - Honestly, I forgot
*/
#define BS_UNPACK(...) (__VA_ARGS__)...


/*
    Generate an enum-like class with the accompanying strings.
    Used to declare enum types capable of binding with BiBS.
*/

#define BS_ENUM_BIND_CONSTANTS(idx,ENUMERATION) \
    godot::ClassDB::bind_integer_constant( \
        ContainingClass::get_class_static(), \
        godot::_gde_constant_get_enum_name( \
            ENUMERATION, \
            BS_TSTR(ENUMERATION) \
        ), \
        BS_TSTR(ENUMERATION), \
        ENUMERATION \
    );

#define BS_ENUM_OPERATORS(ENUM_TYPE_NAME) \
                ENUM_TYPE_NAME():value(0){} \
                ENUM_TYPE_NAME(const ENUM_TYPE_NAME & o):value(o.value){} \
                ENUM_TYPE_NAME(const size_t & o){value = o;} \
                operator size_t &(){ return value; } \
                operator const size_t &() const{ return value; } \
                bool operator==(const size_t & o        ) const{return value == o;      } \
                bool operator==(const ENUM_TYPE_NAME & o) const{return value == o.value;} \
                ENUM_TYPE_NAME & operator=(const size_t &o        ){value = o;return *this;} \
                ENUM_TYPE_NAME & operator=(const ENUM_TYPE_NAME &o){value = o;return *this;} \
                ENUM_TYPE_NAME   operator+(const ENUM_TYPE_NAME &o) const {return value + o;} \
                ENUM_TYPE_NAME   operator-(const ENUM_TYPE_NAME &o) const {return value - o;} \
                ENUM_TYPE_NAME   operator*(const ENUM_TYPE_NAME &o) const {return value * o;} \
                ENUM_TYPE_NAME   operator/(const ENUM_TYPE_NAME &o) const {return value / o;} \
                size_t           operator+(const size_t &o        ) const {return value + o;} \
                size_t           operator-(const size_t &o        ) const {return value - o;} \
                size_t           operator*(const size_t &o        ) const {return value * o;} \
                size_t           operator/(const size_t &o        ) const {return value / o;} \
                ENUM_TYPE_NAME & operator+=(const ENUM_TYPE_NAME &o){value += o;return *this;} \
                ENUM_TYPE_NAME & operator-=(const ENUM_TYPE_NAME &o){value -= o;return *this;} \
                ENUM_TYPE_NAME & operator*=(const ENUM_TYPE_NAME &o){value *= o;return *this;} \
                ENUM_TYPE_NAME & operator/=(const ENUM_TYPE_NAME &o){value /= o;return *this;} \
                size_t &         operator+=(const size_t &o        ){value += o;return *this;} \
                size_t &         operator-=(const size_t &o        ){value -= o;return *this;} \
                size_t &         operator*=(const size_t &o        ){value *= o;return *this;} \
                size_t &         operator/=(const size_t &o        ){value /= o;return *this;} \
                template<typename T> T & operator<<(T & os){return value << os;} \
                template<typename T> T & operator>>(T & os){return value >> os;}

#define BS_ENUM_INT_DECL(IDX,ENUMERATION) constexpr const static size_t ENUMERATION = IDX;
#define BS_ENUM_STR_DECL(IDX,ENUMERATION) ,BS_TSTR(ENUMERATION)
#define BS_ENUM(ENUM_TYPE_NAME,FIRST, ...) \
            class ENUM_TYPE_NAME : public BsEnum \
            { \
                size_t value; \
            public: \
                BS_ENUM_OPERATORS(ENUM_TYPE_NAME) \
                BS_FOR_EACH(BS_ENUM_INT_DECL , FIRST __VA_OPT__(,) __VA_ARGS__) \
                static constexpr const size_t size = BS_COUNT(FIRST,__VA_ARGS__); \
                static constexpr const std::array<const char *,size> Strings = { BS_TSTR(FIRST) __VA_OPT__(BS_FOR_EACH(BS_ENUM_STR_DECL, __VA_ARGS__)) }; \
                static constexpr const std::array<size_t      ,size> Values  = { FIRST __VA_OPT__(,) __VA_ARGS__ }; \
                using ContainingClass = BS_SELF_TYPE; \
                template<typename T=ContainingClass> \
                void bindMethods() \
                { \
                    if constexpr(!std::is_same_v<T,void>) \
                    { \
                        BS_FOR_EACH(BS_ENUM_BIND_CONSTANTS,FIRST __VA_OPT__(,) __VA_ARGS__ ) \
                    } \
                } \
            };


#endif