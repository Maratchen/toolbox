#ifndef MONOIDS_HPP
#define MONOIDS_HPP

#include <limits>
#include <type_traits>

namespace monoid
{
    template<typename T>
    struct sum
    {
        constexpr T neutral_element() const
            noexcept (std::is_nothrow_constructible<T>::value)
        {
            return T{};
        }

        constexpr T operator()(T const &a, T const &b) const
        {
            return a + b;
        }
    };

    template<typename T>
    struct min
    {
        constexpr T neutral_element() const noexcept
        {
             return std::numeric_limits<T>::has_infinity
                ? std::numeric_limits<T>::infinity()
                : std::numeric_limits<T>::max();
        }

        constexpr T operator()(T const &a, T const &b) const
        {
            return std::min(a, b);
        }
    };
    
    template<typename T>
    struct max
    {
        constexpr T neutral_element() const noexcept
        {
             return std::numeric_limits<T>::has_infinity
                ? -std::numeric_limits<T>::infinity()
                : std::numeric_limits<T>::min();
        }

        constexpr T operator()(T const &a, T const &b) const
        {
            return std::max(a, b);
        }
    };
}

#endif