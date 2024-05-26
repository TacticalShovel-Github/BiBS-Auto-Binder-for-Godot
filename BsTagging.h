#ifndef BS_TAGGING_H
#define BS_TAGGING_H


namespace Bs
{
    //////////////////////////
    //Counter implementation//
    //////////////////////////

    template <std::size_t I, class M, template <std::size_t, class> class U>
    struct CtCounterImpl {static constexpr std::size_t value = 0;};

    template <std::size_t I, class M, template <std::size_t, class> class U> requires requires {typename U<I, M>::BS_ARBITRARY;}
    struct CtCounterImpl<I, M, U> {static constexpr std::size_t value = 1 + CtCounterImpl<I + 1, M, U>::value;};

    template <class M, template <std::size_t, class> class U>
    struct CtCounter {static constexpr size_t value = CtCounterImpl<0, M, U>::value;};

    template <template <std::size_t, class> class U>
    constexpr std::size_t CtTagCount = CtCounter<struct MemberTag,U>::value;

    template<typename OBJ_T,template <std::size_t, class> class TAG_T,auto SYMBOL>
    constexpr std::size_t indexOfTagMember()
    {
        bool found=false;
        std::size_t idx = 0;
        std::size_t ret = (std::size_t)-1;
        OBJ_T::template ForEachTagIndex<TAG_T>([&]<typename T>(){
            ret = found ? ret : 
                (SYMBOL == T::tagIndex) ? idx : 
                (std::size_t)-1;
            found = ret != (std::size_t)-1;
            idx++;
        });
        return ret;
    };

    template<typename OBJ_T,template <std::size_t, class> class TAG_T,auto PRED>
    constexpr std::size_t countOfTagIndex()
    {
        std::size_t ret = 0;
        OBJ_T::template ForEachTagIndex<TAG_T>([&]<typename T>(){
            if constexpr(PRED.template operator()<T>())
            {
                ret++;
            }
        });
        return ret;
    };



    /////////////////////////////
    //Preprocessor Syntax Sugar//
    /////////////////////////////
    #define BS_TAG_TEMPLATE_PREAMBLE Bs::Type CONTAINING_CLASS,std::size_t index
    #define BS_TAG_TEMPLATE_PREAMBLE_NAMES CONTAINING_CLASS,index
    #define BS_TAG_COUNT(TAG_NAME) Bs::CtTagCount<TAG_NAME>
    #define BS_QUALIFIED_TAG_COUNT(TAG_NAME,PRED) Bs::CtTagCount<TAG_NAME>

    #define BS_DECLARE_TAG(TAG_NAME,DATA_T) \
        template <size_t I, class T> struct TAG_NAME; \
        template<auto ... TEMPLATE_PARAMs> \
        using TAG_NAME##_DATA = DATA_T<TEMPLATE_PARAMs...>;

    #define BS_TAG_COMMON(TAG_NAME,SYMBOLS,IDX_NAME,ARGS) \
        static constexpr size_t BS_CAT(BS_RF_MBR_IDX_,IDX_NAME,TAG_NAME) = Bs::CtCounter<struct BS_CAT(BS_RF_MBR_TAG_,IDX_NAME,TAG_NAME),TAG_NAME>::value; \
        template <class T> struct TAG_NAME<BS_CAT(BS_RF_MBR_IDX_,IDX_NAME,TAG_NAME), T> \
        { \
            using BS_ARBITRARY = void; \
            using BS_CONTAINING_CLASS = BS_SELF_TYPE; \
            static constexpr Bs::SymbolList<BS_UNWRAP(SYMBOLS)> tagIndex = {}; \
            constexpr const static BS_CAT(TAG_NAME,_DATA)<Bs::symbol<BS_CONTAINING_CLASS>(),BS_CAT(BS_RF_MBR_IDX_,IDX_NAME,TAG_NAME),BS_UNWRAP(SYMBOLS)> data = {BS_UNWRAP(ARGS)}; \
            constexpr const static char * tagName = BS_TSTR(TAG_NAME); \
            constexpr const static size_t index = BS_CAT(BS_RF_MBR_IDX_,IDX_NAME,TAG_NAME); \
            constinit const static bool validated = data.validate<Bs::Override>(); \
        };
    #define BS_TAG(TAG_NAME,SYMBOLS,ARGS) BS_TAG_COMMON( \
        TAG_NAME, \
        SYMBOLS, \
        BS_CAT(TAG_NAME,__LINE__), \
        ARGS \
    )

//            constinit const static bool validated = decltype(data)::validate<Bs::Override>(data); \


    //////////////////////
    //Class Header Stamp//
    //////////////////////

    #define BSCLASS_DETAILS \
    public: \
        void BS_ARBITRARY_METHOD(){} \
        using BS_SELF_TYPE = typename Bs::MemberPointerType<decltype(&BS_ARBITRARY_METHOD)>::ClassType; \
        template<auto MBR_PTR> void set(typename Bs::MemberPointer<MBR_PTR>::ValueType value) { Bs::MemberPointer<MBR_PTR>::set(*this,value); } \
        template<auto MBR_PTR> typename Bs::MemberPointer<MBR_PTR>::ValueType get() { return Bs::MemberPointer<MBR_PTR>::get(*this); } \
        template<template <size_t, class> class TAG_T,typename FNC_PNTR_T> \
        static constexpr void ForEachTagIndex(FNC_PNTR_T fnc) \
        { \
            Bs::ForEach< Bs::CtTagCount<TAG_T> >([&](auto i){ \
                fnc.template operator()<TAG_T<i.value,BS_SELF_TYPE>>(); \
            }); \
        } \
        \
        \
        template<template <size_t, class> class TAG_T,auto V,typename FNC_PNTR_T> \
        static constexpr void ForTag(FNC_PNTR_T fnc) \
        { \
            constexpr const std::size_t tagIndex = Bs::indexOfTagMember<BS_SELF_TYPE,TAG_T,V>(); \
            fnc.template operator()<TAG_T<tagIndex,BS_SELF_TYPE>>(); \
            static_assert(tagIndex != (std::size_t)-1,"Could not locate index of tag member."); \
        } \
    private:

} //namespace Bs

#endif