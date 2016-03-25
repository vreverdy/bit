// =============================== BIT VALUE ================================ //
// Project:         C++ bit library
// Name:            bit_value.hpp
// Description:     A class representing an independent, non-referenced bit
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2016]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_VALUE_HPP_INCLUDED
#define _BIT_VALUE_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <iostream>
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
class bit_value;
template <class UIntType> class bit_reference;
template <class UIntType> class bit_pointer;
template <class Iterator> class bit_iterator;
// ========================================================================== //



/* ******************************* BIT VALUE ******************************** */
// Bit value class definition
class bit_value
{
    // Types
    public:
    using byte_type = unsigned char;
     
    // Lifecycle
    public:
    bit_value() noexcept = default;
    constexpr bit_value(byte_type val) noexcept;
    
    // Conversion
    public:
    explicit constexpr operator bool() const noexcept;
    
    // Operations
    public:
    void set(bool val) noexcept;
    void set() noexcept;
    void reset() noexcept;
    void flip() noexcept;
    
    // Implementation details: data members
    private:
    bool _value;
};

// Stream functions
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    const bit_value& x
);
template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_value& x
);
/* ************************************************************************** */



// -------------------------- BIT VALUE: LIFECYCLE -------------------------- //
// Implicitly constructs the bit value from the least significant bit of a byte
constexpr bit_value::bit_value(
    byte_type val
) noexcept
: _value(val & static_cast<byte_type>(1))
{
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT VALUE: CONVERSION -------------------------- //
// Implicitly converts the bit value to a boolean
constexpr bit_value::operator bool(
) const noexcept
{
    return _value;
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT VALUE: OPERATIONS -------------------------- //
// Sets the value of the bit to the provided boolean value
void bit_value::set(
    bool b
) noexcept
{
    _value = b;
}

// Sets the value of the bit to one
void bit_value::set(
) noexcept
{
    _value = true;
}

// Resets the value of the bit to zero
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



// ---------------------------- STREAM FUNCTIONS ---------------------------- //
// Inserts a bit value in an output stream
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    const bit_value& x
)
{
    return os << (x ? '1' : '0');
}

// Extracts a bit value from an input stream
template <class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_value& x
)
{
    unsigned int tmp;
    is >> tmp;
    x = tmp;
    return is;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_VALUE_HPP_INCLUDED
// ========================================================================== //
