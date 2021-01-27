#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP

#include "monoid.hpp"
#include "utilities.hpp"

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

template<
    class T,
    class Monoid = monoid::sum<T>,
    class Allocator = std::allocator<T>
>
class segment_tree : private Monoid
{
public:
    using value_type = T;
    using monoid_type = Monoid;
    using allocator_type = Allocator;
    using container_type = std::vector<value_type,allocator_type>;
    using size_type = typename container_type::size_type;

    template<class Iterator>
    segment_tree(
            Iterator first, Iterator last,
            allocator_type const &allocator = allocator_type{})
        : size_(std::distance(first, last))
        , tree_(allocator)
    {
        static_assert(
            std::is_convertible<
                typename std::iterator_traits<Iterator>::value_type,
                value_type
            >::value,
            "incompatible value type");

        if (size_)
        {
            tree_.assign(
                utilities::round_up_to_power_of_two(size_) * 2,
                monoid_type::neutral_element());

            build_tree(0, size_, first, last);
        }
    }

    segment_tree(
            std::initializer_list<T> const &list,
            allocator_type const &allocator = allocator_type{})
        : segment_tree(list.begin(), list.end(), allocator)
    {
    }

    size_type size() const noexcept { return size_; }

    value_type search(size_type first, size_type last) const
    {
        if ((first >= last) || (last > size_))
            throw std::invalid_argument("invalid range");

        return search_tree(0, 0, size_, first, last);
    }

    void update(size_type index, const value_type &value)
    {
        if (index >= size_)
            throw std::out_of_range("invalid index");

        update_tree(0, 0, size_, index, value);
    }

private:
    template<class Iterator>
    void build_tree(
            size_type const pos,
            size_type const size,
            Iterator first, Iterator last)
    {
        if (size == 1)
        {
            tree_[pos] = *first;
        }
        else
        {
            auto const half = size / 2;
            auto const middle = std::next(first, half);

            build_tree(2 * pos + 1, half, first, middle);
            build_tree(2 * pos + 2, size - half, middle, last);

            tree_[pos] = monoid_type::operator()(
                    tree_[2 * pos + 1],
                    tree_[2 * pos + 2]
                );
        }
    }

    value_type search_tree(
            size_type const pos,
            size_type const left, size_type const right,
            size_type const first, size_type const last) const
    {
        if (first >= last)
            return monoid_type::neutral_element();

        if (left == first && right == last)
            return tree_[pos];

        auto const middle = left + (right - left) / 2;

        return monoid_type::operator()(
                search_tree(2 * pos + 1, left, middle, first, std::min(middle, last)),
                search_tree(2 * pos + 2, middle, right, std::max(middle, first), last)
            );
    }

    void update_tree(
            size_type const pos,
            size_type const left, size_type const right,
            size_type const index, const value_type &value)
    {
        if (1 == right - left)
        {
            tree_[pos] = value;
        }
        else
        {
            auto const middle = left + (right - left) / 2;

            if (index < middle)
                update_tree(2 * pos + 1, left, middle, index, value);
            else
                update_tree(2 * pos + 2, middle, right, index, value);

            tree_[pos] = monoid_type::operator()(
                    tree_[2 * pos + 1],
                    tree_[2 * pos + 2]
                );
        }
    }

    size_type const size_;
    container_type tree_;
};

template<class T, class Allocator = std::allocator<T>>
using segment_min_tree = segment_tree<T, monoid::min<T>, Allocator>;

template<class T, class Allocator = std::allocator<T>>
using segment_max_tree = segment_tree<T, monoid::max<T>, Allocator>;

#endif