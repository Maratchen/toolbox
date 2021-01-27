#ifndef UTILITIES_POWER_OF_TWO_HPP
#define UTILITIES_POWER_OF_TWO_HPP

#include <limits>

namespace utilities
{
    template<typename Integer>
    constexpr bool is_power_of_two(Integer value) noexcept
    {
        return (value & (value - 1)) == 0;
    }

    template<typename Integer>
    Integer round_up_to_power_of_two(Integer value) noexcept
    {
        if (is_power_of_two(value))
            return value;

        constexpr int digits = std::numeric_limits<Integer>::digits;

        for (int i = 1; i < digits; i += i)
            value |= value >> i;

        return value + 1;
    }
}

#endif