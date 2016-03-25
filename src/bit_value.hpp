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
    constexpr operator bool() const noexcept;
    
    // Operations
    public:
    void set(bool val) noexcept;
    void set() noexcept;
    void reset() noexcept;
    void flip() noexcept;

    // Compound assignment operators
    public:
    template <class T> bit_value& operator+=(const T& val) noexcept;
    template <class T> bit_value& operator-=(const T& val) noexcept;
    template <class T> bit_value& operator*=(const T& val) noexcept;
    template <class T> bit_value& operator/=(const T& val) noexcept;
    template <class T> bit_value& operator%=(const T& val) noexcept;
    template <class T> bit_value& operator&=(const T& val) noexcept;
    template <class T> bit_value& operator|=(const T& val) noexcept;
    template <class T> bit_value& operator^=(const T& val) noexcept;
    template <class T> bit_value& operator<<=(const T& val) noexcept;
    template <class T> bit_value& operator>>=(const T& val) noexcept;
    
    // Increment and decrement operators
    public:
    bit_value& operator++() noexcept;
    bit_value& operator--() noexcept;
    bit_value operator++(int) noexcept;
    bit_value operator--(int) noexcept;
    
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



// ---------------- BIT VALUE: COMPOUND ASSIGNMENT OPERATORS ---------------- //
// Adds and assigns
template <class T> 
bit_value& bit_value::operator+=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() + std::declval<T>());
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (static_cast<byte_type>(val) & 1) {
            flip();
        }
    } else {
        *this = *this + val;
    }
    return *this;
}

// Substracts and assigns
template <class T> 
bit_value& bit_value::operator-=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() - std::declval<T>());
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (static_cast<byte_type>(val) & 1) {
            flip();
        }
    } else {
        *this = *this - val;
    }
    return *this;
}

// Multiplies and assigns
template <class T> 
bit_value& bit_value::operator*=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() * std::declval<T>());
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (*this && !(static_cast<byte_type>(val) & 1)) {
            reset();
        }
    } else {
        *this = *this * val;
    }
    return *this;
}

// Divides and assigns
template <class T> 
bit_value& bit_value::operator/=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() / std::declval<T>());
    static constexpr type one = 1;
    static constexpr type signed_one = std::is_unsigned<T>::value ? 1 : -1;
    if (std::is_arithmetic<T>::value && std::is_arithmetic<type>::value) {
        if (*this && val) {
            if (std::is_integral<type>::value) {
                if (val != one) {
                    if (std::is_unsigned<T>::value || val != signed_one) {
                        reset();
                    }
                }
            } else {
                if (val < signed_one || val > one) {
                    reset();
                } else {
                    *this = *this / val;
                }
            }
        } else if (!val) {
            *this = *this / val;
        }
    } else {
        *this = *this / val;
    }
    return *this;
}

// Computes a modulus and assigns
template <class T> 
bit_value& bit_value::operator%=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() % std::declval<T>());
    static constexpr type one = 1;
    static constexpr type signed_one = std::is_unsigned<T>::value ? 1 : -1;
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (*this && val) {
            if (val != one && val != signed_one) {
                reset();
            }
        } else if (!val) {
            *this = *this % val;
        }
    } else {
        *this = *this % val;
    }
    return *this;
}

// Computes a bitwise and and assigns
template <class T> 
bit_value& bit_value::operator&=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() & std::declval<T>());
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (*this && val & 1) {
            set();
        }
    } else {
        *this = *this & val;
    }
    return *this;
}

// Computes a bitwise or and assigns
template <class T> 
bit_value& bit_value::operator|=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() | std::declval<T>());
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (!*this && val & 1) {
            set();
        }
    } else {
        *this = *this & val;
    }
    return *this;
}

// Computes a bitwise xor and assigns
template <class T> 
bit_value& bit_value::operator^=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() ^ std::declval<T>());
    if (std::is_arithmetic<T>::value && std::is_integral<type>::value) {
        if (val & 1) {
            flip();
        }
    } else {
        *this = *this & val;
    }
    return *this;
}

// Shifts to the left and assigns
template <class T> 
bit_value& bit_value::operator<<=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() << std::declval<T>());
    static constexpr type one = 1;
    static constexpr type digits = std::numeric_limits<int>::digits;
    if (std::is_arithmetic<T>::value && std::is_same<type, int>::value) {
        if (val + one >= one && val + one <= digits) {
            if (*this && val >= one) {
                reset();
            }
        } else {
            *this = *this << val;
        }
    } else {
        *this = *this << val;
    }
    return *this;
}

// Shifts to the right and assigns
template <class T> 
bit_value& bit_value::operator>>=(
    const T& val
) noexcept
{
    using type = decltype(std::declval<bit_value>() >> std::declval<T>());
    static constexpr type one = 1;
    static constexpr type digits = std::numeric_limits<int>::digits;
    if (std::is_arithmetic<T>::value && std::is_same<type, int>::value) {
        if (val + one >= one && val + one <= digits) {
            if (*this && val >= one) {
                reset();
            }
        } else {
            *this = *this >> val;
        }
    } else {
        *this = *this >> val;
    }
    return *this;
}
// -------------------------------------------------------------------------- //



// -------------- BIT VALUE: INCREMENT AND DECREMENT OPERATORS -------------- //
// Increments the bit value and returns it
bit_value& bit_value::operator++(
) noexcept
{
    flip();
    return *this;
}

// Decrements the bit value and returns it
bit_value& bit_value::operator--(
) noexcept
{
    flip();
    return *this;
}

// Increments the bit value and returns the old one
bit_value bit_value::operator++(
    int
) noexcept
{
    bit_value old = *this;
    flip();
    return old;
}

// Decrements the bit value and returns the old one
bit_value bit_value::operator--(
    int
) noexcept
{
    bit_value old = *this;
    flip();
    return old;
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
