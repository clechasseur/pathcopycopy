/**
 * @file
 * @brief Utilities used by <tt>coveo::enumerable</tt>.
 *
 * @copyright 2016-2019, Coveo Solutions Inc.
 *            Distributed under the Apache License, Version 2.0 (see <a href="https://github.com/coveo/linq/blob/master/LICENSE">LICENSE</a>).
 */

#ifndef COVEO_SEQUENCE_UTIL_H
#define COVEO_SEQUENCE_UTIL_H

#include <coveo/seq/detail/enumerable_detail.h>

#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace coveo {

/// @cond

// Forward declaration of enumerable, for type traits
template<typename> class enumerable;

/// @endcond

/**
 * @brief Traits class for elements in a sequence.
 * @headerfile sequence_util.h <coveo/seq/sequence_util.h>
 *
 * Traits class containing definitions pertaining to the elements of a sequence.
 * For sequences of references or <tt>std::reference_wrapper</tt>s, provides
 * information about the referred type instead.
 *
 * @tparam T Type of elements in the sequence.
 */
template<typename T>
struct seq_element_traits
{
    /**
     * @brief Type of element in the sequence.
     *
     * Type of the sequence's elements. Corresponds to @c T.
     */
    using value_type = T;

    /**
     * @brief Type of element in the sequence, but @c const.
     *
     * Same as <tt>coveo::seq_element_traits::value_type</tt>, but @c const.
     */
    using const_value_type = const value_type;

    /**
     * @brief Raw type of element in the sequence.
     *
     * Same as <tt>coveo::seq_element_traits::value_type</tt>, but "raw", e.g. without @c const or @c volatile.
     */
    using raw_value_type = typename std::remove_cv<value_type>::type;

    /**
     * @brief Pointer to a sequence element.
     *
     * Pointer to an element in the sequence.
     * Corresponds to <tt>coveo::seq_element_traits::value_type*</tt>.
     */
    using pointer = value_type*;
    
    /**
     * @brief Reference to a sequence element.
     *
     * Reference to an element in the sequence.
     * Corresponds to <tt>coveo::seq_element_traits::value_type&</tt>.
     */
    using reference = value_type&;

    /**
     * @brief Pointer to a @c const sequence element.
     *
     * Pointer to a @c const element in the sequence.
     * Corresponds to <tt>coveo::seq_element_traits::const_value_type*</tt>.
     */
    using const_pointer = const_value_type*;

    /**
     * @brief Reference to a @c const sequence element.
     *
     * Reference to a @c const element in the sequence.
     * Corresponds to <tt>coveo::seq_element_traits::const_value_type&</tt>.
     */
    using const_reference = const_value_type&;
};
/// @cond
template<typename T> struct seq_element_traits<T&> : seq_element_traits<T> { };
template<typename T> struct seq_element_traits<T&&> : seq_element_traits<T> { };
template<typename T> struct seq_element_traits<std::reference_wrapper<T>> : seq_element_traits<T> { };
/// @endcond

/**
 * @brief Traits class for a sequence.
 * @headerfile sequence_util.h <coveo/seq/sequence_util.h>
 *
 * Traits class containing definitions pertaining to a sequence. A shorthand
 * for <tt>coveo::seq_element_traits</tt> that infers the sequence's @c value_type
 * from the return value of its iterators. Also provides the type of iterator
 * used by the sequence.
 *
 * @tparam Seq Type of sequence.
 */
template<typename Seq>
struct seq_traits : public seq_element_traits<decltype(*std::begin(std::declval<Seq&>()))>
{
    /**
     * @brief Type of iterator used by the sequence.
     *
     * Type of iterator used by the sequence, which is defined as the type
     * of iterator returned when calling <tt>std::begin()</tt>.
     */
    using iterator_type = typename std::decay<decltype(std::begin(std::declval<Seq&>()))>::type;
};
/// @cond
template<typename Seq> struct seq_traits<Seq&> : seq_traits<Seq> { };
template<typename Seq> struct seq_traits<Seq&&> : seq_traits<Seq> { };
template<typename Seq> struct seq_traits<std::reference_wrapper<Seq>> : seq_traits<Seq> { };
/// @endcond

#ifdef DOXYGEN_INVOKED
/**
 * @brief Traits class to identify enumerable objects.
 * @headerfile sequence_util.h <coveo/seq/sequence_util.h>
 *
 * Traits class that can be used to identify enumerable objects.
 *
 * @tparam T Type to identify.
 */
template<typename T> struct is_enumerable;
#else
template<typename> struct is_enumerable : std::false_type { };
template<typename T> struct is_enumerable<enumerable<T>> : std::true_type { };
#endif

/**
 * @brief Helper function to quickly reserve space in a container if possible.
 * @headerfile sequence_util.h <coveo/seq/sequence_util.h>
 *
 * Attempts to @c reserve space in container @c cnt to hold as many
 * elements as found in sequence @c seq. This is performed only if
 * it's possible to do so "quickly".
 *
 * - If @c seq is a <tt>coveo::enumerable</tt>, space is reserved
 *   by using its <tt>coveo::enumerable::size()</tt> method if
 *   <tt>coveo::enumerable::has_fast_size()</tt> returns @c true.
 * - If @c seq is a container with a <tt>size()</tt> method, space
 *   is reserved by using that method.
 * - If @c seq is a sequence that uses random-access iterators,
 *   space is reserved by using <tt>std::distance()</tt>.
 * - Otherwise, space is not reserved.
 *
 * @param cnt Container in which to reserve space.
 * @param seq Sequence to use to try to reserve space for.
 * @return @c true if space has actually been reserved in @c cnt.
 */
template<typename C, typename Seq>
auto try_reserve(C& cnt, const Seq& seq) -> typename std::enable_if<is_enumerable<Seq>::value, bool>::type
{
    const bool can_reserve = seq.has_fast_size();
    if (can_reserve) {
        cnt.reserve(seq.size());
    }
    return can_reserve;
}
template<typename C, typename Seq>
auto try_reserve(C& cnt, const Seq& seq) -> typename std::enable_if<!is_enumerable<Seq>::value &&
                                                                    coveo::detail::has_size_const_method<Seq>::value, bool>::type
{
    cnt.reserve(seq.size());
    return true;
}
template<typename C, typename Seq>
auto try_reserve(C& cnt, const Seq& seq) -> typename std::enable_if<!coveo::detail::has_size_const_method<Seq>::value &&
                                                                    std::is_base_of<std::random_access_iterator_tag,
                                                                                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category>::value,
                                                                    bool>::type
{
    cnt.reserve(std::distance(std::begin(seq), std::end(seq)));
    return true;
}
template<typename C, typename Seq>
auto try_reserve(C&, const Seq&) -> typename std::enable_if<!coveo::detail::has_size_const_method<typename std::decay<Seq>::type>::value &&
                                                            !std::is_base_of<std::random_access_iterator_tag,
                                                                             typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category>::value,
                                                            bool>::type
{
    // Can't reserve, no fast way of doing so
    return false;
}

/**
 * @brief Helper function to get a fast size delegate if possible.
 * @headerfile sequence_util.h <coveo/seq/sequence_util.h>
 *
 * Attempts to create a <tt>coveo::enumerable::size_delegate</tt>
 * that can quickly calculate the number of elements found in
 * sequence @c seq. A size delegate is returned only if it's
 * possible to calculate the number of elements "quickly".
 *
 * - If @c seq is a <tt>coveo::enumerable</tt>, its
 *   <tt>coveo::enumerable::size()</tt> method is used to produce
 *   the size delegate if <tt>coveo::enumerable::has_fast_size()</tt>
 *   returns @c true.
 * - If @c seq is a container with a <tt>size()</tt> method, a size
 *   delegate is produced using that method.
 * - If @c seq is a sequence that uses random-access iterators,
 *   a size delegate is produced by using <tt>std::distance()</tt>.
 * - Otherwise, no size delegate is produced.
 *
 * @param seq Sequence to calculate the number of elements of
 *            in order to produce the size delegate.
 * @return <tt>coveo::enumerable::size_delegate</tt> instance,
 *         or @c nullptr if it's not possible to quickly calculate
 *         the number of elements in @c seq.
 * @see coveo::enumerable::size_delegate
 */
template<typename Seq>
auto try_get_size_delegate(const Seq& seq) -> typename std::enable_if<is_enumerable<Seq>::value,
                                                                      std::function<std::size_t()>>::type
{
    std::function<std::size_t()> siz;
    if (seq.has_fast_size()) {
        const std::size_t size = seq.size();
        siz = [size]() -> std::size_t { return size; };
    }
    return siz;
}
template<typename Seq>
auto try_get_size_delegate(const Seq& seq) -> typename std::enable_if<!is_enumerable<Seq>::value &&
                                                                      coveo::detail::has_size_const_method<Seq>::value,
                                                                      std::function<std::size_t()>>::type
{
    const std::size_t size = seq.size();
    return [size]() -> std::size_t { return size; };
}
template<typename Seq>
auto try_get_size_delegate(const Seq& seq) -> typename std::enable_if<!coveo::detail::has_size_const_method<Seq>::value &&
                                                                      std::is_base_of<std::random_access_iterator_tag,
                                                                                      typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category>::value,
                                                                      std::function<std::size_t()>>::type
{
    const std::size_t size = static_cast<std::size_t>(std::distance(std::begin(seq), std::end(seq)));
    return [size]() -> std::size_t { return size; };
}
template<typename Seq>
auto try_get_size_delegate(const Seq&) -> typename std::enable_if<!coveo::detail::has_size_const_method<Seq>::value &&
                                                                  !std::is_base_of<std::random_access_iterator_tag,
                                                                                   typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category>::value,
                                                                  std::function<std::size_t()>>::type
{
    // No way to quickly determine size, don't try
    return nullptr;
}

/**
 * @brief Helper function to get a fast size delegate from iterators if possible.
 * @headerfile sequence_util.h <coveo/seq/sequence_util.h>
 *
 * Attempts to create a <tt>coveo::enumerable::size_delegate</tt> that can
 * quickly calculate the number of elements in range <tt>[beg, end[</tt>.
 * A size delegate is returned only if it's possible to calculate the number
 * of elements "quickly".
 *
 * - If @c beg and @c end are random-access iterators,
 *   a size delegate is produced by using <tt>std::distance()</tt>.
 * - Otherwise, no size delegate is produced.
 *
 * @param beg Iterator pointing at beginning of range.
 * @param end Iterator pointing at end of range.
 * @return <tt>coveo::enumerable::size_delegate</tt> instance,
 *         or @c nullptr if it's not possible to quickly calculate
 *         the number of elements in <tt>[beg, end[</tt>
 * @see coveo::enumerable::size_delegate
 */
template<typename It>
auto try_get_size_delegate_for_iterators(const It& beg, const It& end) -> typename std::enable_if<std::is_base_of<std::random_access_iterator_tag,
                                                                                                                  typename std::iterator_traits<It>::iterator_category>::value,
                                                                                                  std::function<std::size_t()>>::type
{
    return [beg, end]() -> std::size_t { return std::distance(beg, end); };
}
template<typename It>
auto try_get_size_delegate_for_iterators(const It&, const It&) -> typename std::enable_if<!std::is_base_of<std::random_access_iterator_tag,
                                                                                                           typename std::iterator_traits<It>::iterator_category>::value,
                                                                                          std::function<std::size_t()>>::type
{
    // No way to quickly determine size, don't try
    return nullptr;
}

} // coveo

#endif // COVEO_SEQUENCE_UTIL_H
