// ============================= BIT ALGORITHM ============================== //
// Project:         The C++ Bit Library
// Name:            bit_algorithm.hpp
// Description:     Optimized versions of algorithms for bit manipulation
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2016]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_ALGORITHM_HPP_INCLUDED
#define _BIT_ALGORITHM_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit_details.hpp"
#include "bit_value.hpp"
#include "bit_reference.hpp"
#include "bit_pointer.hpp"
#include "bit_iterator.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ***************************** BIT CONSTANTS ****************************** */
// Bit constants
static constexpr bit_value zero_bit(0U);
static constexpr bit_value one_bit(1U);
/* ************************************************************************** */



/* ************************** STANDARD ALGORITHMS *************************** */
// Non-modifying sequence operations
template <class InputIt, class T> 
typename bit_iterator<InputIt>::difference_type
count(
    bit_iterator<InputIt> first, 
    bit_iterator<InputIt> last, 
    const T& value
);
/* ************************************************************************** */



// ------------------- NON-MODIFYING SEQUENCE OPERATIONS -------------------- //
// Counts the number of bits equal to the provided bit value
template <class InputIt, class T> 
typename bit_iterator<InputIt>::difference_type
count(
    bit_iterator<InputIt> first, 
    bit_iterator<InputIt> last, 
    const T& value
)
{
    // Initialization
    using underlying_type = typename bit_iterator<InputIt>::underlying_type;
    using difference_type = typename bit_iterator<InputIt>::difference_type;
    constexpr difference_type digits = binary_digits<underlying_type>::value;
    const bit_value input = value;
    difference_type result = 0;
    auto it = first.base();
    
    // Computation when bits belong to several underlying values
    if (first.base() != last.base()) {
        if (first.position() != 0) {
            result = _popcnt(*first.base() >> first.position());
            ++it;
        }
        for (; it != last.base(); ++it) {
            result += _popcnt(*it);
        }
        if (last.position() != 0) {
            result += _popcnt(*last.base() << (digits - last.position()));
        }
    // Computation when bits belong to the same underlying value
    } else {
        result = _popcnt(_bextr(
            *first.base(), 
            static_cast<underlying_type>(first.position()), 
            static_cast<underlying_type>(last.position() - first.position())
        ));
    }
    
    // Negates when the number of zero bits is requested
    if (!static_cast<bool>(input)) {
        result = (last - first) - result;
    }
    
    // Finalization
    return result;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_ALGORITHM_HPP_INCLUDED
// ========================================================================== //
