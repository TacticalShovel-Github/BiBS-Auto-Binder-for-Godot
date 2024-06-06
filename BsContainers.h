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

    public:
        constexpr const static PsuedoValue UNDEFINED = PsuedoValue::PS_UNDEFINED;
        constexpr const static PsuedoValue DEFINED = PsuedoValue::PS_DEFINED;
    private:
        T value = {};
        PsuedoValue psuedoValue = UNDEFINED;

    };

} //namespace Bs
#endif