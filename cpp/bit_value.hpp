// =============================== BIT VALUE ================================ //
// Project:         The C++ Bit Library
// Name:            bit_value.hpp
// Description:     A class representing an independent, non-referenced bit
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_VALUE_HPP_INCLUDED
#define _BIT_VALUE_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit_details.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ******************************* BIT VALUE ******************************** */
// Bit value class definition
class bit_value
{
    // Types
    public:
    using size_type = std::size_t;
    
    // Lifecycle
    public:
    bit_value() noexcept = default;
    template <class T> 
    constexpr bit_value(bit_reference<T> ref) noexcept;
    template <class UIntType> 
    explicit constexpr bit_value(UIntType val) noexcept;
    template <class UIntType> 
    constexpr bit_value(UIntType val, size_type pos);
    
    // Assignment
    public:
    template <class T> 
    bit_value& operator=(bit_reference<T> ref) noexcept;

    // Conversion
    public:
    explicit constexpr operator bool() const noexcept;

    // Bit manipulation
    public:
    void set(bool b) noexcept;
    void set() noexcept;
    void reset() noexcept;
    void flip() noexcept;

    // Implementation details: data members
    private:
    bool _value;

    // Comparison operators
    public:
    friend constexpr bool operator==(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator!=(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator<(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator<=(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator>(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;
    friend constexpr bool operator>=(
        bit_value lhs, 
        bit_value rhs
    ) noexcept;

    // Stream functions
    public:
    template <class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(
        std::basic_ostream<CharT, Traits>& os,
        bit_value x
    );
    template <class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>& operator>>(
        std::basic_istream<CharT, Traits>& is,
        bit_value& x
    );
};
/* ************************************************************************** */



// -------------------------- BIT VALUE: LIFECYCLE -------------------------- //
// Implicitly constructs a bit value from a bit reference
template <class T> 
constexpr bit_value::bit_value(
    bit_reference<T> ref
) noexcept
: _value(static_cast<bool>(ref))
{
}
    
// Explicitly constructs an aligned bit value
template <class UIntType>
constexpr bit_value::bit_value(
    UIntType val
) noexcept
: _value(val & 1)
{
    static_assert(binary_digits<UIntType>::value, "");
}

// Explicitly constructs an unaligned bit value
template <class UIntType> 
constexpr bit_value::bit_value(
    UIntType val, 
    size_type pos
)
: _value((assert(pos < binary_digits<UIntType>::value), val >> pos & 1))
{
    static_assert(binary_digits<UIntType>::value, "");
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT VALUE: ASSIGNMENT -------------------------- //
// Assigns a bit reference to the bit value
template <class T> 
bit_value& bit_value::operator=(
    bit_reference<T> ref
) noexcept
{
    _value = static_cast<bool>(ref);
    return *this;
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT VALUE: CONVERSION -------------------------- //
// Explicitly converts the bit value to a boolean value
constexpr bit_value::operator bool(
) const noexcept
{
    return _value;
}
// -------------------------------------------------------------------------- //



// ---------------------- BIT VALUE: BIT MANIPULATION ----------------------- //
// Sets the value of the bit to the provided boolean value
void bit_value::set(
    bool b
) noexcept
{
    _value = b;
}

// Sets the value of the bit to 1
void bit_value::set(
) noexcept
{
    _value = true;
}

// Resets the value of the bit to 0
void bit_value::reset(
) noexcept
{
    _value = false;
}

// Flips the value of the bit
void bit_value::flip(
) noexcept
{
    _value = !_value;
}
// -------------------------------------------------------------------------- //



// -------------------- BIT VALUE: COMPARISON OPERATORS --------------------- //
// Checks if the left hand side is equal to the right hand side
constexpr bool operator==(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value == rhs._value;
}

// Checks if the left hand side is non equal to the right hand side
template <class T, class U>
constexpr bool operator!=(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value != rhs._value;
}

// Checks if the left hand side is less than the right hand side
template <class T, class U>
constexpr bool operator<(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value < rhs._value;
}

// Checks if the left hand side is less than or equal to the right hand side
template <class T, class U>
constexpr bool operator<=(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value <= rhs._value;
}

// Checks if the left hand side is greater than the right hand side
template <class T, class U>
constexpr bool operator>(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value > rhs._value;
}

// Checks if the left hand side is greater than or equal to the right hand side
template <class T, class U>
constexpr bool operator>=(
    bit_value lhs, 
    bit_value rhs
) noexcept
{
    return lhs._value >= rhs._value;
}
// -------------------------------------------------------------------------- //



// ---------------------- BIT VALUE: STREAM FUNCTIONS ----------------------- //
// Inserts a bit value in an output stream
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    bit_value x
)
{
    return os << static_cast<char>('0' + x._value);
}

// Extracts a bit value from an input stream
template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_value& x
)
{
    return is >> x._value;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_VALUE_HPP_INCLUDED
// ========================================================================== //
