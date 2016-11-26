// ============================ INTEGRAL BITSET ============================= //
// Project:         The C++ Bit Library
// Name:            integral_bitset.hpp
// Description:     A class exhibiting both behaviors of a bitset and an integer
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2016-2017]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _INTEGRAL_BITSET_HPP_INCLUDED
#define _INTEGRAL_BITSET_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit_details.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* **************************** INTEGRAL BITSET ***************************** */
// Integral bitset class definition
template <std::size_t Min, std::size_t Max, class Container>
class integral_bitset
{   
    // Assertions
    static_assert(binary_digits<typename Container::value_type>::value, "");
    static_assert(Min <= Max, "");
    
    // Types
    public:
    using container_type = Container;
    using underlying_type = typename Container::value_type;
    using value_type = bit_value;
    using reference = bit_reference<underlying_type>;
    using const_reference = bit_reference<const underlying_type>;
    using pointer = bit_pointer<underlying_type>;
    using const_pointer = bit_pointer<const underlying_type>;
    using iterator = bit_iterator<typename Container::iterator>;
    using const_iterator = bit_iterator<typename Container::const_iterator>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = typename iterator::size_type;
    using difference_type = typename iterator::difference_type;
    
    // Constants
    public:
    static constexpr size_type dynamic = -1;
    static constexpr size_type minimum_size = Min;
    static constexpr size_type maximum_size = Max;
    static constexpr bool has_minimum_size = Min != dynamic;
    static constexpr bool has_maximum_size = Max != dynamic;
    static constexpr bool has_fixed_size = Min == Max && Max != dynamic;
    static constexpr bool has_bounded_size = Min < Max && Max != dynamic;
    static constexpr bool has_half_bounded_size = Min < Max && Max == dynamic;
    static constexpr bool has_dynamic_size = Min == dynamic && Max == dynamic;

    // Lifecyle
    public:
    constexpr integral_bitset(const integral_bitset& other) const;
    constexpr integral_bitset(integral_bitset&& other) const;
    constexpr integral_bitset(std::initializer_list<value_type> init);
    

    // Element access
    public:
    constexpr reference at(size_type pos);
    constexpr const_reference at(size_type pos) const;
    constexpr reference operator[](size_type pos);
    constexpr const_reference operator[](size_type pos) const;
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;
    
    // Iterators
    public:
    constexpr iterator begin();
    constexpr const_iterator begin() const;
    constexpr const_iterator cbegin() const;
    constexpr iterator end();
    constexpr const_iterator end() const;
    constexpr const_iterator cend() const;
    constexpr reverse_iterator rbegin();
    constexpr const_reverse_iterator rbegin() const;
    constexpr const_reverse_iterator crbegin() const;
    constexpr reverse_iterator rend();
    constexpr const_reverse_iterator rend() const;
    constexpr const_reverse_iterator crend() const;
    
    // Capacity
    public:
    constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    void resize(size_type count);
    void resize(size_type count, const value_type& value);
    constexpr size_type capacity() const noexcept;
    void reserve(size_type new_cap);
    void shrink_to_fit();
    
    // Implementation details: data members
    private:
    container_type _container;
    typename std::conditional<
        has_fixed_size, 
        std::integral_constant<size_type, Min>, 
        size_type
    >::type _size;
    
    // Implementation details: capacity
    private:
    template <class X = container_type, class = decltype(X().max_size())>
    constexpr size_type _max_size() const noexcept;
    template <class... X, class = typename std::enable_if<!sizeof...(X)>::type>
    constexpr size_type _max_size(X...) const;
    template <class X = container_type, class = decltype(X().resize(0))>
    void _resize(size_type count);
    template <class X = container_type, class = decltype(X().resize(0, 0))>
    void _resize(size_type count, value_type value);
    template <class... X, class = typename std::enable_if<!sizeof...(X)>::type>
    void _resize(size_type count, X...);
    template <class... X, class = typename std::enable_if<!sizeof...(X)>::type>
    void _resize(size_type count, value_type value, X...);
    template <class X = container_type, class = decltype(X().resize(0))>
    void _reserve(size_type new_cap);
    template <class X = container_type, class = decltype(X().resize(0))>
    void _shrink_to_fit();
    template <class... X, class = typename std::enable_if<!sizeof...(X)>::type>
    void _shrink_to_fit(X...);
    template <class X = container_type, class = decltype(X().shrink_to_fit())>
    void _shrink_to_fit_container();
    template <class... X, class = typename std::enable_if<!sizeof...(X)>::type>
    void _shrink_to_fit_container(X...);
};
/* ************************************************************************** */



// -------------------- INTEGRAL BITSET: ELEMENT ACCESS --------------------- //
// Accesses the mutable bit at specified location with bounds checking
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::reference 
integral_bitset<Min, Max, Container>::at(
    size_type pos
)
{
    constexpr auto message = "integral_bitset::at";
    return pos < size() ? begin()[pos] : throw std::out_of_range(message);
}

// Accesses the immutable bit at specified location with bounds checking
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reference 
integral_bitset<Min, Max, Container>::at(
    size_type pos
) const
{
    constexpr auto message = "integral_bitset::at";
    return pos < size() ? begin()[pos] : throw std::out_of_range(message);
}

// Accesses the mutable bit at specified location
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::reference 
integral_bitset<Min, Max, Container>::operator[](
    size_type pos
)
{
    assert(pos < size());
    return begin()[pos];
}

// Accesses the immutable bit at specified location
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reference 
integral_bitset<Min, Max, Container>::operator[](
    size_type pos
) const
{
    assert(pos < size());
    return begin()[pos];
}

// Returns a mutable reference to the first bit
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::reference 
integral_bitset<Min, Max, Container>::front(
)
{
    assert(size() > 0);
    return *begin();
}

// Returns an immutable reference to the first bit
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reference 
integral_bitset<Min, Max, Container>::front(
) const
{
    assert(size() > 0);
    return *begin();
}

// Returns a mutable reference to the last bit
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::reference 
integral_bitset<Min, Max, Container>::back(
)
{
    assert(size() > 0);
    return std::advance(begin(), size() - 1);
}

// Returns an immutable reference to the last bit
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reference 
integral_bitset<Min, Max, Container>::back(
) const
{
    assert(size() > 0);
    return std::advance(begin(), size() - 1);
}
// -------------------------------------------------------------------------- //



// ----------------------- INTEGRAL BITSET: ITERATORS ----------------------- //
// Returns a mutable iterator to the bitset beginning
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::iterator 
integral_bitset<Min, Max, Container>::begin(
)
{
    return iterator(std::begin(_container));
}

// Returns an immutable iterator to bitset beginning
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_iterator 
integral_bitset<Min, Max, Container>::begin(
) const
{
    return const_iterator(std::cbegin(_container));
}

// Explicitly returns an immutable iterator to the bitset beginning
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_iterator 
integral_bitset<Min, Max, Container>::cbegin(
) const
{
    return begin();
}

// Returns a mutable iterator to the bitset end
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::iterator 
integral_bitset<Min, Max, Container>::end(
)
{
    return std::advance(begin(), size());
}

// Returns an immutable iterator to the bitset end
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_iterator 
integral_bitset<Min, Max, Container>::end(
) const
{
    return std::advance(begin(), size());
}

// Explicitly returns an immutable iterator to the bitset end
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_iterator 
integral_bitset<Min, Max, Container>::cend(
) const
{
    return end();
}

// Returns a mutable iterator to the reversed bitset beginning
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::reverse_iterator 
integral_bitset<Min, Max, Container>::rbegin(
)
{
    return reverse_iterator(end());
}

// Returns an immutable iterator to the reversed bitset beginning
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reverse_iterator 
integral_bitset<Min, Max, Container>::rbegin(
) const
{
    return reverse_iterator(end());
}

// Explicitly returns an immutable iterator to the reversed bitset beginning
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reverse_iterator 
integral_bitset<Min, Max, Container>::crbegin(
) const
{
    return reverse_iterator(cend());
}

// Returns a mutable iterator to the reversed bitset end
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::reverse_iterator 
integral_bitset<Min, Max, Container>::rend(
)
{
    return reverse_iterator(begin());
}

// Returns an immutable iterator to the reversed bitset end
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reverse_iterator 
integral_bitset<Min, Max, Container>::rend(
) const
{
    return reverse_iterator(begin());
}

// Explicitly returns an immutable iterator to the reversed bitset end
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::const_reverse_iterator 
integral_bitset<Min, Max, Container>::crend(
) const
{
    return reverse_iterator(cbegin());
}
// -------------------------------------------------------------------------- //



// ----------------------- INTEGRAL BITSET: CAPACITY ------------------------ //
// Checks if the container has no elements
template <std::size_t Min, std::size_t Max, class Container>
constexpr bool integral_bitset<Min, Max, Container>::empty(
) const noexcept
{
    return size() == 0;
}

// Returns the current number of bits in the bitset
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::size_type 
integral_bitset<Min, Max, Container>::size(
) const noexcept
{
    return _size;
}

// Returns the maximum number of bits the bitset is able to hold
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::size_type 
integral_bitset<Min, Max, Container>::max_size(
) const noexcept
{
    return _max_size();
}

// Resizes to contain count bits with new ones being default inserted
template <std::size_t Min, std::size_t Max, class Container>
void integral_bitset<Min, Max, Container>::resize(
    size_type count
)
{
    _resize(count);
}

// Resizes to contain count bits with new ones initialized to value 
template <std::size_t Min, std::size_t Max, class Container>
void integral_bitset<Min, Max, Container>::resize(
    size_type count, 
    const value_type& value
)
{
    _resize(count, value);
}

// Returns the number of bits that can be hold in the container size
template <std::size_t Min, std::size_t Max, class Container>
constexpr typename integral_bitset<Min, Max, Container>::size_type 
integral_bitset<Min, Max, Container>::capacity(
) const noexcept
{
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits;
    return _container.size() <= ratio ? _container.size() * digits : max;
}

// Increases the capacity to a value that's greater or equal to new capacity
template <std::size_t Min, std::size_t Max, class Container>
void integral_bitset<Min, Max, Container>::reserve(
    size_type new_cap
)
{
    _reserve(new_cap);
}

// Requests the removal of unused capacity
template <std::size_t Min, std::size_t Max, class Container>
void integral_bitset<Min, Max, Container>::shrink_to_fit(
)
{
    _shrink_to_fit();
}
// -------------------------------------------------------------------------- //



// ----------- INTEGRAL BITSET: IMPLEMENTATION DETAILS: CAPACITY ------------ //
// Returns the maximum number of bits that the container can hold
template <std::size_t Min, std::size_t Max, class Container>
template <class X, class>
constexpr size_type integral_bitset<Min, Max, Container>::_max_size(
) const noexcept
{
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits;
    size_type underlying_max_size = _container.max_size();
    return underlying_max_size <= ratio ? underlying_max_size * digits : max;
}

// Returns the maximum number of bits that an equivalent container can hold
template <std::size_t Min, std::size_t Max, class Container>
template <class... X, class>
constexpr size_type integral_bitset<Min, Max, Container>::_max_size(
    X...
) const
{
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits;
    constexpr size_type quotient = has_fixed_size * maximum_size / digits;
    constexpr size_type remainder = has_fixed_size * maximum_size % digits;
    using type = typename std::conditional<
        has_fixed_size, 
        std::array<underlying_type, quotient + (remainder != 0)>, 
        std::vector<underlying_type>
    >::type;
    size_type underlying_max_size = type().max_size();
    return underlying_max_size <= ratio ? underlying_max_size * digits : max;
}

// Resizes to contain count bits with dynamic container and default insertion
template <std::size_t Min, std::size_t Max, class Container>
template <class X, class>
void integral_bitset<Min, Max, Container>::_resize(
    size_type count
)
{
    constexpr auto message = "integral_bitset::resize";
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits; 
    const size_type new_container_size = count / digits + (count % digits != 0);
    const bool minimum_condition = !has_minimum_size || count >= minimum_size;
    const bool maximum_condition = !has_maximum_size || count <= maximum_size;
    if (count <= size() ? minimum_condition : maximum_condition) {
        _container.resize(new_container_size);
        _size = count;
    } else {
        throw std::length_error(message);
    }
}

// Resizes to contain count bits with dynamic container and value insertion
template <std::size_t Min, std::size_t Max, class Container>
template <class X, class>
void integral_bitset<Min, Max, Container>::_resize(
    size_type count, 
    value_type value
)
{
    constexpr auto message = "integral_bitset::resize";
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits; 
    const size_type new_container_size = count / digits + (count % digits != 0);
    const bool minimum_condition = !has_minimum_size || count >= minimum_size;
    const bool maximum_condition = !has_maximum_size || count <= maximum_size;
    if (count <= size() ? minimum_condition : maximum_condition) {
        _container.resize(new_container_size);
        count > size() ? fill(end(), std::next(begin(), count), value) : void();
        _size = count;
    } else {
        throw std::length_error(message);
    }
}

// Resizes to contain count bits with fixed size container and default insertion
template <std::size_t Min, std::size_t Max, class Container>
template <class... X, class>
void integral_bitset<Min, Max, Container>::_resize(
    size_type count, 
    X...
)
{
    constexpr auto message = "integral_bitset::resize";
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits;
    const size_type new_container_size = count / digits + (count % digits != 0);
    const bool container_condition = new_container_size <= _container.size();
    const bool minimum_condition = !has_minimum_size || count >= minimum_size;
    const bool maximum_condition = !has_maximum_size || count <= maximum_size;
    if (count < size()) {
        _size = minimum_condition ? count : throw std::length_error(message);
    } else if (count > size()) {
        _size = condition ? count : throw std::length_error(message);
    }
}

// Resizes to contain count bits with fixed size container and value insertion
template <std::size_t Min, std::size_t Max, class Container>
template <class... X, class>
void integral_bitset<Min, Max, Container>::_resize(
    size_type count, 
    value_type value, 
    X...
)
{
    constexpr auto message = "integral_bitset::resize";
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits;
    const size_type new_container_size = count / digits + (count % digits != 0);
    const bool container_condition = new_container_size <= _container.size();
    const bool minimum_condition = !has_minimum_size || count >= minimum_size;
    const bool maximum_condition = !has_maximum_size || count <= maximum_size;
    if (count < size()) {
        _size = minimum_condition ? count : throw std::length_error(message);
    } else if (count > size()) {
        if (container_condition && maximum_condition) {
            fill(end(), std::next(begin(), count), value);
            _size = count;
        } else {
            throw std::length_error(message);
        }
    }
}

// Reserves new bit storage by resizing the underlying dynamic container
template <std::size_t Min, std::size_t Max, class Container>
template <class X, class>
void integral_bitset<Min, Max, Container>::_reserve(
    size_type new_cap
)
{
    constexpr size_type digits = binary_digits<underlying_type>::value;
    constexpr size_type max = std::numeric_limits<size_type>::max();
    constexpr size_type ratio = max / digits;
    size_type new_container_cap = new_cap / digits + (new_cap % digits != 0);
    if (new_container_cap > _container.size()) {
        _container.resize(new_container_cap);
    }
}

// Removes unused capacity by resizing the underlying dynamic container
template <std::size_t Min, std::size_t Max, class Container>
template <class X, class>
void integral_bitset<Min, Max, Container>::_shrink_to_fit(
)
{
    constexpr size_type digits = binary_digits<underlying_type>::value;
    _container.resize(_size / digits + (_size % digits != 0));
    _shrink_to_fit_on_container();
}

// Cannot removes unused capacity for fixed size underlying container
template <std::size_t Min, std::size_t Max, class Container>
template <class... X, class>
void integral_bitset<Min, Max, Container>::_shrink_to_fit(
    X...
)
{
}

// Removes unused capacity in the underlying container
template <std::size_t Min, std::size_t Max, class Container>
template <class X, class>
void integral_bitset<Min, Max, Container>::_shrink_to_fit_container(
)
{
    _container.shrink_to_fit();
}

// Cannot calls shrink to fit on the underlying container
template <std::size_t Min, std::size_t Max, class Container>
template <class... X, class>
void integral_bitset<Min, Max, Container>::_shrink_to_fit_container(
    X...
)
{
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _INTEGRAL_BITSET_HPP_INCLUDED
// ========================================================================== //
