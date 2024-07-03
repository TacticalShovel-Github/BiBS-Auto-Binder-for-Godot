#ifndef BS_CONTAINERS_H
#define BS_CONTAINERS_H

namespace Bs
{

    enum class PsuedoValue
    {
        PS_UNDEFINED,
        PS_DEFINED
    };

    template<typename T>
    struct Ternary
    {
    public:
        constexpr operator T &()
        {
            return value;
        }

        constexpr operator const T &() const
        {
            return value;
        }

        constexpr Ternary() : value(), psuedoValue(PsuedoValue::PS_UNDEFINED)
        {
            setPsuedoValue(PsuedoValue::PS_UNDEFINED);
        }

        constexpr Ternary(T && other) : value(other), psuedoValue(PsuedoValue::PS_UNDEFINED)
        {
            setPsuedoValue(PsuedoValue::PS_DEFINED);
        }

        constexpr Ternary(const T & other) : value(other), psuedoValue(PsuedoValue::PS_UNDEFINED)
        {
            setPsuedoValue(PsuedoValue::PS_DEFINED);
        }
        
        constexpr Ternary(const Ternary & other) : value(other.value), psuedoValue(PsuedoValue::PS_UNDEFINED)
        {
            setPsuedoValue(other.psuedoValue);
        }

        constexpr Ternary(const PsuedoValue & other) : psuedoValue(PsuedoValue::PS_UNDEFINED)
        {
            setPsuedoValue(other);
        }

        constexpr void operator=(const T & other)
        {
            setPsuedoValue(PsuedoValue::PS_DEFINED);
            value = other;
        }
        constexpr void operator=(const PsuedoValue & other)
        {
            setPsuedoValue(other);
        }

        constexpr bool operator==(const T & other) const
        {
            return psuedoValue != UNDEFINED && other == this->value;
        }

        constexpr bool operator==(const PsuedoValue & other) const
        {
            return other == this->psuedoValue;
        }

        constexpr void setPsuedoValue(const PsuedoValue & other)
        {
            this->psuedoValue = other;
        }
        constexpr T get() const
        {
            return value;
        }

    public:
        constexpr const static PsuedoValue UNDEFINED = PsuedoValue::PS_UNDEFINED;
        constexpr const static PsuedoValue DEFINED = PsuedoValue::PS_DEFINED;
    private:
        T value = {};
        PsuedoValue psuedoValue = UNDEFINED;

    };

    template<typename T>
    struct ContingentResult
    {
        //Forwarding reference for optimization
        godot::Variant && value;

        ContingentResult(godot::Variant && value) : value(std::forward<godot::Variant>(value)){}

        //Type is known. Do not need to safety check.
        template<typename U=T> requires (Bs::Gd::CGdObject<U>)
        T & getRef()
        {
            return *(godot::Object::cast_to<T>(value));
        }

        template<typename U=T> requires (Bs::Gd::CGdPrimative<U>)
        T & getRef()
        {
            return value;
        }

        template<typename U=T> requires (Bs::Gd::CGdPrimative<U>) operator T *() = delete;
        template<typename U=T> requires (Bs::Gd::CGdPrimative<U>) operator T &(){return (T)value;};

        template<typename U=T> requires (!Bs::Gd::CGdRefCounted<U>) operator T *(){return godot::Object::cast_to<T>(value);}
        template<typename U=T> requires (!Bs::Gd::CGdRefCounted<U>) operator T &(){return godot::Object::cast_to<T>(value);}


        template<typename U>  //requires (Bs::Gd::CGdObject<T>)
        void operator=(U && o)
        {
            getRef()=o;
        }

        template<typename U> requires (Bs::Gd::CGdObject<T>)
        void operator=(T * o)
        {
            &(getRef())=o;
        }

        operator godot::Variant()
        {
            return value;
        }
    };






    
    template <class Parent,class T>
    class ContainerAdaptor : public Parent
    { 
    public:
        using YieldType = ContingentResult<T>; 
        using value_type = typename Bs::GetNthType<
            (
                (Bs::Gd::CGdRefCounted<T><<0)+
                (Bs::Gd::CGdObject<T>    <<1)
            ),
            /*0*/ T,
            /*1*/ godot::Ref<T>,
            /*2*/ T *,
            /*3*/ godot::Ref<T>,
            /*4*/ T *
        >::type;



        ContainerAdaptor(){}// : Parent(){}
        ContainerAdaptor(const ContainerAdaptor & o) : Parent()
        {
            this->put(o);
        }
        ContainerAdaptor(std::initializer_list<T *> values){}

        template<typename U>
        void operator=(const U & o){this->put(o);}

        template<typename U> requires Bs::CSizedContainer<U>
        void put(const U & o)
        {
            for(size_t i=0;i<o.size();i++)
            {
                this->push_back(Parent::operator[](i));
            }
        }

        YieldType operator[](size_t idx)
        {
            return YieldType(
                std::forward<godot::Variant>(Parent::operator[](idx)) 
            );
        }

        template<typename U> requires (Bs::Gd::CGdObject<T>)
        void operator+=(U * o)
        {
            
            this->push_back(o);
        }
            
        class Iterator
        {
        private:
            ContainerAdaptor * arr;
            size_t idx;
        public:
            Iterator(ContainerAdaptor * arr,size_t idx) : arr(arr), idx(idx){}
            Iterator(const Iterator & o) : arr(o.arr), idx(o.idx){}
            Iterator& operator=(const Iterator & o)
            {
                arr = o.arr;
                idx = o.idx;
            }
            Iterator& operator++()
            {
                ++idx;
                return *this;
            }

            bool operator==(const Iterator & o) const { return (this->arr == o.arr) && (this->idx == o.idx); }
            bool operator!=(const Iterator & o) const { return !(*this == o); }

            Iterator operator++(int)
            {
                return Iterator(arr,idx++);
            }

            YieldType operator*() const
            {
                return arr->operator[](idx);
            }
        };

        Iterator begin(){return Iterator(this,0);}
        Iterator end(){return Iterator(this,(size_t)-1);}
    };

    template<typename T>
    using Array = ContainerAdaptor<godot::TypedArray<T>,T>;


} //namespace Bs
#endif