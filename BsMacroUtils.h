#ifndef BS_MACRO_UTILS_H
#define BS_MACRO_UTILS_H

#define BS_PP_EMPTY
#define BS_PP_GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,RET,...) RET

/*
    COUNT - Give the number of arguments provided as a literal of type int
*/
#define BS_COUNT_IMPL(_8,_7,_6,_5,_4,_3,_2,_1,_0,RET,...) RET
#define BS_COUNT(...) BS_COUNT_IMPL(__VA_ARGS__,8,7,6,5,4,3,2,1)

/*
    TSTR - Convert arguments to string literal
*/
#define BS_TSTR_IMPL(...) #__VA_ARGS__
#define BS_TSTR(...) BS_TSTR_IMPL(__VA_ARGS__)


/*
    Concatenate together the provided arguements
*/
#define BS_PP_CAT_0()
#define BS_PP_CAT_1(x) x
#define BS_PP_CAT_2_IMPL(x,y) x##y
#define BS_PP_CAT_2(x,y) BS_PP_CAT_2_IMPL(x,y)
#define BS_PP_CAT_3(x,...) BS_PP_CAT_2(x,BS_PP_CAT_2(__VA_ARGS__))
#define BS_PP_CAT_4(x,...) BS_PP_CAT_2(x,BS_PP_CAT_3(__VA_ARGS__))
#define BS_PP_CAT_5(x,...) BS_PP_CAT_2(x,BS_PP_CAT_4(__VA_ARGS__))
#define BS_PP_CAT_6(x,...) BS_PP_CAT_2(x,BS_PP_CAT_5(__VA_ARGS__))
#define BS_PP_CAT_7(x,...) BS_PP_CAT_2(x,BS_PP_CAT_6(__VA_ARGS__))
#define BS_PP_CAT_8(x,...) BS_PP_CAT_2(x,BS_PP_CAT_7(__VA_ARGS__))
#define BS_CAT(...) \
    BS_PP_GET_MACRO(__VA_ARGS__ __VA_OPT__(,) \
    BS_PP_CAT_8, \
    BS_PP_CAT_7, \
    BS_PP_CAT_6, \
    BS_PP_CAT_5, \
    BS_PP_CAT_4, \
    BS_PP_CAT_3, \
    BS_PP_CAT_2, \
    BS_PP_CAT_1, \
    BS_PP_CAT_0)(__VA_ARGS__)

/*
    REP - Repeat the given macro once for each provided argument, without using that argument as a parameter
*/
#define BS_PP_REP8(...) __VA_ARGS__ BS_PP_REP7(__VA_ARGS__)
#define BS_PP_REP7(...) __VA_ARGS__ BS_PP_REP6(__VA_ARGS__)
#define BS_PP_REP6(...) __VA_ARGS__ BS_PP_REP5(__VA_ARGS__)
#define BS_PP_REP5(...) __VA_ARGS__ BS_PP_REP4(__VA_ARGS__)
#define BS_PP_REP4(...) __VA_ARGS__ BS_PP_REP3(__VA_ARGS__)
#define BS_PP_REP3(...) __VA_ARGS__ BS_PP_REP2(__VA_ARGS__)
#define BS_PP_REP2(...) __VA_ARGS__ BS_PP_REP1(__VA_ARGS__)
#define BS_PP_REP1(...) __VA_ARGS__ BS_PP_REP0(__VA_ARGS__)
#define BS_PP_REP0(...)
#define BS_PP_REP(action,...) \
    BS_PP_GET_MACRO( \
        __VA_ARGS__ __VA_OPT__(,) \
        BS_PP_REP8, \
        BS_PP_REP7, \
        BS_PP_REP6, \
        BS_PP_REP5, \
        BS_PP_REP4, \
        BS_PP_REP3, \
        BS_PP_REP2, \
        BS_PP_REP1, \
        BS_PP_REP0, \
    )(action,__VA_ARGS__)

/*
    FOR_EACH(M,ARGs) - Repeat the macro M(X,I) over X=ARGs, where I is the index of the X in ARGs.
*/
#define BS_PP_FE_0(action        )
#define BS_PP_FE_1(action, X     ) action(X,0)
#define BS_PP_FE_2(action, X, ...) action(X,1) BS_PP_FE_1(action, __VA_ARGS__)
#define BS_PP_FE_3(action, X, ...) action(X,2) BS_PP_FE_2(action, __VA_ARGS__)
#define BS_PP_FE_4(action, X, ...) action(X,3) BS_PP_FE_3(action, __VA_ARGS__)
#define BS_PP_FE_5(action, X, ...) action(X,4) BS_PP_FE_4(action, __VA_ARGS__)
#define BS_PP_FE_6(action, X, ...) action(X,5) BS_PP_FE_5(action, __VA_ARGS__)
#define BS_PP_FE_7(action, X, ...) action(X,6) BS_PP_FE_6(action, __VA_ARGS__)
#define BS_PP_FE_8(action, X, ...) action(X,7) BS_PP_FE_7(action, __VA_ARGS__)
#define BS_FOR_EACH(action,...)  \
    BS_PP_GET_MACRO( \
    __VA_ARGS__ __VA_OPT__(,) \
    BS_PP_FE_8, \
    BS_PP_FE_7, \
    BS_PP_FE_6, \
    BS_PP_FE_5, \
    BS_PP_FE_4, \
    BS_PP_FE_3, \
    BS_PP_FE_2, \
    BS_PP_FE_1, \
    BS_PP_FE_0)(action,__VA_ARGS__)

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

#endif