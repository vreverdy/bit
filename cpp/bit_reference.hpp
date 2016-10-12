// ============================= BIT REFERENCE ============================== //
// Project:         The C++ Bit Library
// Name:            bit_reference.hpp
// Description:     A class representing a reference to a bit
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_REFERENCE_HPP_INCLUDED
#define _BIT_REFERENCE_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit_details.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ***************************** BIT REFERENCE ****************************** */
// Bit reference class definition
template <class UIntType>
class bit_reference
{
    // Assertions
    static_assert(binary_digits<UIntType>::value, "");

    // Friendship
    template <class> friend class bit_reference;
    friend class bit_pointer<UIntType>;
    
    // Types
    public:
    using underlying_type = UIntType;
    using size_type = std::size_t;

    // Lifecycle
    public:
    template <class T> 
    constexpr bit_reference(const bit_reference<T>& other) noexcept;
    explicit constexpr bit_reference(underlying_type& ref) noexcept;
    constexpr bit_reference(underlying_type& ref, size_type pos);

    // Assignment
    public:
    bit_reference& operator=(const bit_reference& other) noexcept;
    template <class T> 
    bit_reference& operator=(const bit_reference<T>& other) noexcept;
    bit_reference& operator=(bit_value val) noexcept;
    void assign(underlying_type val) noexcept;
    void assign(underlying_type val, size_type pos);
    
    // Conversion
    public:
    explicit constexpr operator bool() const noexcept;

    // Access
    public:
    constexpr bit_pointer<UIntType> operator&() const noexcept;
    
    // Swap members
    public:
    template <class T> 
    void swap(bit_reference<T> other);
    void swap(bit_value& other);

    // Bit manipulation
    public:
    void set(bool b) noexcept;
    void set() noexcept;
    void reset() noexcept;
    void flip() noexcept;

    // Underlying details
    public:
    constexpr underlying_type* address() const noexcept;
    constexpr size_type position() const noexcept;
    constexpr underlying_type mask() const noexcept;

    // Implementation details: function members
    private:
    bit_reference() noexcept = default;
    explicit constexpr bit_reference(std::nullptr_t) noexcept;
    explicit constexpr bit_reference(underlying_type* ptr) noexcept;
    constexpr bit_reference(underlying_type* ptr, size_type pos);

    // Implementation details: data members
    private:
    underlying_type* _ptr;
    typename std::remove_cv<underlying_type>::type _mask;

    // Stream functions
    public:
    template <class CharT, class Traits, class T>
    friend std::basic_ostream<CharT, Traits>& operator<<(
        std::basic_ostream<CharT, Traits>& os,
        bit_reference<T> x
    );
    template <class CharT, class Traits, class T>
    friend std::basic_istream<CharT, Traits>& operator>>(
        std::basic_istream<CharT, Traits>& is,
        bit_reference<T>& x
    );
};

// Swap and exchange
template <class T, class U>
void swap(
    bit_reference<T> lhs,
    bit_reference<U> rhs
) noexcept;
template <class T>
void swap(
    bit_reference<T> lhs,
    bit_value& rhs
) noexcept;
template <class U>
void swap(
    bit_value& lhs,
    bit_reference<U> rhs
) noexcept;
template <class T, class U = bit_value>
bit_value exchange(
    bit_reference<T> x,
    U&& val
);
/* ************************************************************************** */



// ------------------------ BIT REFERENCE: LIFECYCLE ------------------------ //
// Implicitly constructs a bit reference from another bit reference
template <class UIntType>
template <class T> 
constexpr bit_reference<UIntType>::bit_reference(
    const bit_reference<T>& other
) noexcept
: _ptr(other._ptr)
, _mask(other._mask)
{
}

// Explicitly constructs an aligned bit reference
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    underlying_type& ref
) noexcept
: _ptr(&ref)
, _mask(1)
{
}

// Explicitly constructs an unaligned bit reference
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    underlying_type& ref, 
    size_type pos
)
: _ptr((assert(pos < binary_digits<underlying_type>::value), &ref))
, _mask(static_cast<underlying_type>(1) << pos)
{
}
// -------------------------------------------------------------------------- //



// ----------------------- BIT REFERENCE: ASSIGNMENT ------------------------ //
// Copies a bit reference to the bit reference
template <class UIntType>
bit_reference<UIntType>& bit_reference<UIntType>::operator=(
    const bit_reference& other
) noexcept
{
    other ? set() : reset();
    return *this;
}

// Assigns a bit reference to the bit reference
template <class UIntType>
template <class T> 
bit_reference<UIntType>& bit_reference<UIntType>::operator=(
    const bit_reference<T>& other
) noexcept
{
    other ? set() : reset();
    return *this;
}

// Assigns a bit value to the bit reference
template <class UIntType>
bit_reference<UIntType>& bit_reference<UIntType>::operator=(
    bit_value val
) noexcept
{
    val ? set() : reset();
    return *this;
}

// Assigns the aligned bit of a value to the bit reference
template <class UIntType>
void bit_reference<UIntType>::assign(
    underlying_type val
) noexcept
{
    val & 1 ? set() : reset();
}

// Assigns an unaligned bit of a value to the bit reference
template <class UIntType>
void bit_reference<UIntType>::assign(
    underlying_type val, 
    size_type pos
)
{
    assert(pos < binary_digits<underlying_type>::value);
    val >> pos & 1 ? set() : reset();
}
// -------------------------------------------------------------------------- //



// ----------------------- BIT REFERENCE: CONVERSION ------------------------ //
// Explicitly converts the bit reference to a boolean value
template <class UIntType>
constexpr bit_reference<UIntType>::operator bool(
) const noexcept
{
    return *_ptr & _mask;
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT REFERENCE: ACCESS -------------------------- //
// Gets a bit pointer from the bit reference
template <class UIntType>
constexpr bit_pointer<UIntType> bit_reference<UIntType>::operator&(
) const noexcept
{
    return bit_pointer<UIntType>(_ptr, position());
}
// -------------------------------------------------------------------------- //



// ---------------------- BIT REFERENCE: SWAP MEMBERS ----------------------- //
// Swaps the value of the referenced bit with another bit reference
template <class UIntType>
template <class T>
void bit_reference<UIntType>::swap(
    bit_reference<T> other
)
{
    if (other != *this) {
        flip();
        other.flip();
    }
}

// Swaps the value of the referenced bit with a bit value
template <class UIntType>
void bit_reference<UIntType>::swap(
    bit_value& other
)
{
    if (other != *this) {
        flip();
        other.flip();
    }
}
// -------------------------------------------------------------------------- //



// -------------------- BIT REFERENCE: BIT MANIPULATION --------------------- //
// Sets the value of the referenced bit to the provided boolean value
template <class UIntType>
void bit_reference<UIntType>::set(
    bool b
) noexcept
{
    b ? set() : reset();
}

// Sets the value of the referenced bit to 1
template <class UIntType>
void bit_reference<UIntType>::set(
) noexcept
{
    *_ptr |= _mask;
}

// Resets the value of the referenced bit to 0
template <class UIntType>
void bit_reference<UIntType>::reset(
) noexcept
{
    *_ptr &= ~_mask;
}

// Flips the value of the referenced bit
template <class UIntType>
void bit_reference<UIntType>::flip(
) noexcept
{
    *_ptr ^= _mask;
}
// -------------------------------------------------------------------------- //



// ------------------- BIT REFERENCE: UNDERLYING DETAILS -------------------- //
// Returns a pointer to the underlying object
template <class UIntType>
constexpr typename bit_reference<UIntType>::underlying_type* 
bit_reference<UIntType>::address(
) const noexcept
{
    return _ptr;
}

// Returns the position of the referenced bit within the underlying object
template <class UIntType>
constexpr typename bit_reference<UIntType>::size_type
bit_reference<UIntType>::position(
) const noexcept
{
    return _tzcnt(_mask);
}

// Returns a mask corresponding to the referenced bit
template <class UIntType>
constexpr typename bit_reference<UIntType>::underlying_type 
bit_reference<UIntType>::mask(
) const noexcept
{
    return _mask;
}
// -------------------------------------------------------------------------- //



// -------------------- BIT REFERENCE: STREAM FUNCTIONS --------------------- //
// Inserts a bit reference in an output stream
template <class CharT, class Traits, class T>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    bit_reference<T> x
)
{
    return os << static_cast<char>('0' + static_cast<bool>(x));
}

// Extracts a bit reference from an input stream
template <class CharT, class Traits, class T>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_reference<T>& x
)
{
    bit_value tmp;
    is >> tmp;
    x = tmp;
    return is;
}
// -------------------------------------------------------------------------- //



// -------------------- BIT REFERENCE: SWAP AND EXCHANGE -------------------- //
// Swaps two bit references
template <class T, class U>
void swap(
    bit_reference<T> lhs,
    bit_reference<U> rhs
) noexcept
{
    if (lhs != rhs) {
        lhs.flip();
        rhs.flip();
    }
}

// Swaps a bit reference and a bit value
template <class T>
void swap(
    bit_reference<T> lhs,
    bit_value& rhs
) noexcept
{
    if (lhs != rhs) {
        lhs.flip();
        rhs.flip();
    }
}

// Swaps a bit value and a bit reference
template <class U>
void swap(
    bit_value& lhs,
    bit_reference<U> rhs
) noexcept
{
    if (lhs != rhs) {
        lhs.flip();
        rhs.flip();
    }
}

// Exchanges a bit reference and a value
template <class T, class U>
bit_value exchange(
    bit_reference<T> x,
    U&& val
)
{
    bit_value old = x;
    x = val;
    return old;
}
// -------------------------------------------------------------------------- //



// -------- BIT REFERENCE: IMPLEMENTATION DETAILS: FUNCTION MEMBERS --------- //
// Privately explicitly constructs a bit reference from a nullptr
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    std::nullptr_t
) noexcept
: _ptr(nullptr)
, _mask()
{
}
    
// Privately explicitly constructs an aligned bit reference from a pointer
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    underlying_type* ptr
) noexcept
: _ptr(ptr)
, _mask(1)
{
}

// Privately explicitly constructs an unaligned bit reference from a pointer
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    underlying_type* ptr, 
    size_type pos
)
: _ptr((assert(pos < binary_digits<underlying_type>::value), ptr))
, _mask(static_cast<underlying_type>(1) << pos)
{
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_REFERENCE_HPP_INCLUDED
// ========================================================================== //
