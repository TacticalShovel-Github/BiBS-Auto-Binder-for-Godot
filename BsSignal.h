#ifndef BS_SIGNAL_H
#define BS_SIGNAL_H


#ifdef _MSC_VER
#define MAYBE_TYPENAME
#else
#define MAYBE_TYPENAME typename
#endif

namespace Bs
{
    /*
        Declare Signals.

        These typedefs are outside of the class to be referenced in by their multiple users.
    */

    template<typename T> struct Signals{};

#define DECLARE_EXTERNAL_SIGNALS(CLASS_T) template<> struct Bs::Signals<CLASS_T>


    template<Bs::Literal NAME, auto ... ARGs>
    struct Signal
    {
        template<auto V>
        struct VisualCMinusMinusHelper{using type = typename decltype(V)::type;};

        template<std::size_t N>
        using ParamType = typename GetNthType<N,typename decltype(ARGs)::type ...>::type;
        using TypeList = TypeList<typename decltype(ARGs)::type ...>;

        static constexpr const char * name = NAME.label;
        template<typename OBJ_T>
        static void signalOutBind()
        {
            LogBS::writePartial("        (%s:%s): Binding output signal ... ",((godot::String)OBJ_T::get_class_static()).ascii().get_data(),NAME.label);
            [&]<typename T, T ... Is>(std::integer_sequence<T, Is...>){

                const auto methodInfo = godot::MethodInfo(
                    NAME.label,
                    BS_UNPACK(
                        godot::PropertyInfo(
                            Bs::Gd::VariantCodeFromType<MAYBE_TYPENAME decltype(ARGs)::type>::value, 
                            Bs::GetNth<Is,BS_UNPACK(ARGs)>::value.label
                        )
                    )
                );
                

                godot::ClassDB::add_signal(
                    OBJ_T::get_class_static(),
                    methodInfo
                );
            }(std::make_integer_sequence<std::size_t,sizeof...(ARGs)>{});
            LogBS::write("done");
        }
        


        template<typename OBJ_T, typename ... Ts>
        static void emit(OBJ_T * tx, typename VisualCMinusMinusHelper<ARGs>::type ... args)
        {
            tx->emit_signal(name,args...);
        }

        template<typename SEND_OBJ_T,typename RECV_OBJ_T>
        static void bindCallback(SEND_OBJ_T * tx, RECV_OBJ_T * rx)
        {
            constexpr const Bs::SymbolList<Bs::symbol<Signal<NAME,ARGs...>>(),Bs::symbol<RECV_OBJ_T>()> SignalRxTagIndex;

            LogBS::writePartial("        (%s:%s:%s): Binding callback ... ",((godot::String)SEND_OBJ_T::get_class_static()).ascii().get_data(),((godot::String)RECV_OBJ_T::get_class_static()).ascii().get_data(),NAME.label);
            RECV_OBJ_T::template ForTag<RECV_OBJ_T::template SIGNAL_RX,SignalRxTagIndex>([&]<typename T>(){
                
            constexpr auto fncPtr = T::data.callback;
            constexpr const Bs::SymbolList<Bs::symbol<fncPtr>()> methodTagIndex;

#ifdef _MSC_VER
            RECV_OBJ_T::template ForTag<typename RECV_OBJ_T::EXPORT,Bs::SymbolList<Bs::symbol<fncPtr>()>{}>([&]<typename U>(){
#else
            RECV_OBJ_T::template ForTag<RECV_OBJ_T::template EXPORT,Bs::SymbolList<Bs::symbol<fncPtr>()>{}>([&]<typename U>(){
#endif
                    const auto callable = godot::Callable(rx, U::data.name).bind();
                    tx->connect(NAME.label,callable);
                    Bs::ForEach<sizeof...(ARGs)>([](auto i){
                        using ACTUAL_ARG_T = typename Bs::MethodPointer<fncPtr>::template ParamType<i.value>;
                        using EXPECTED_ARG_T = typename Bs::GetNthType<i.value,typename decltype(ARGs)::type...>::type;
                        static_assert(std::is_same_v<Bs::Gd::BaseType<ACTUAL_ARG_T>,Bs::Gd::BaseType<EXPECTED_ARG_T>>,"Parameter types for callback do not match the signal");
                    });
                });
#ifdef _MSC_VER
            });
#else
            });
#endif
            LogBS::write("done");
        }
    }; /*class Signal*/


//    template<Bs::Literal NAME, auto ... ARGs>
//    template<typename OBJ_T, typename ... Ts>
//    Signal<Name,ARGs>::emit<OBJ_T,Ts ...>(OBJ_T * tx, Ts ... args){}


}

#endif