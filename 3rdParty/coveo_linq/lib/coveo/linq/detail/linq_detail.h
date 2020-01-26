/**
 * @file
 * @brief Implementation details of LINQ operators.
 *
 * This file contains implementation details for built-in LINQ operators.
 * It should not be necessary to use this file directly when using the library.
 * Code in the <tt>coveo::linq::detail</tt> namespace is subject to change
 * in-between versions.
 *
 * @copyright 2016-2019, Coveo Solutions Inc.
 *            Distributed under the Apache License, Version 2.0 (see <a href="https://github.com/coveo/linq/blob/master/LICENSE">LICENSE</a>).
 */

#ifndef COVEO_LINQ_DETAIL_H
#define COVEO_LINQ_DETAIL_H

#include <coveo/seq/detail/enumerable_detail.h>
#include <coveo/seq/sequence_util.h>

#include <algorithm>
#include <cstddef>
#include <forward_list>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace coveo {
namespace linq {
namespace detail {

/**
 * @internal
 * @brief Proxy comparator.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Comparator that keeps a pointer to another comparator and uses it
 * to implement <tt>operator()()</tt>. Allows instances of lambdas to be
 * used as predicates for <tt>set</tt>s, for instance.
 *
 * @tparam Pred Predicate to proxy.
 */
template<typename Pred>
class proxy_cmp
{
private:
    const typename std::decay<Pred>::type* ppred_;

public:
    explicit proxy_cmp(const Pred& pred)
        : ppred_(std::addressof(pred)) { }
    
    template<typename T, typename U>
    auto operator()(T&& left, U&& right) const -> decltype((*ppred_)(std::forward<T>(left), std::forward<U>(right))) {
        return (*ppred_)(std::forward<T>(left), std::forward<U>(right));
    }
};

/**
 * @internal
 * @brief Dereferencing comparator.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Comparator that receives pointers to elements to compare,
 * dereferences them and calls another predicate.
 *
 * @tparam Pred Predicate to call with dereferenced pointers.
 */
template<typename Pred>
class deref_cmp
{
private:
    Pred pred_;

public:
    explicit deref_cmp(Pred&& pred)
        : pred_(std::forward<Pred>(pred)) { }

    template<typename T, typename U>
    auto operator()(T* const pleft, U* const pright) const -> decltype(pred_(*pleft, *pright)) {
        return pred_(*pleft, *pright);
    }
};

/**
 * @internal
 * @brief Indexless proxy selector.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Selector that can be used with operators that pass an element
 * and its index, when the index is not needed. Will simply drop
 * the index and call another selector with the element.
 *
 * @tparam Selector Selector to call with element only.
 */
template<typename Selector>
class indexless_selector_proxy
{
private:
    Selector sel_;  // Selector that doesn't care about index

public:
    explicit indexless_selector_proxy(Selector&& sel)
        : sel_(std::forward<Selector>(sel)) { }

    template<typename T>
    auto operator()(T&& element, std::size_t) -> decltype(sel_(std::forward<T>(element))) {
        return sel_(std::forward<T>(element));
    }
};

/**
 * @internal
 * @brief Dereferencing next delegate implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of a <tt>coveo::enumerable::next_delegate</tt>
 * that takes a sequence of pointers and returns references by
 * dereferencing said pointers. Meant to be used to construct
 * <tt>coveo::enumerable</tt>s.
 *
 * @tparam Seq Sequence containing pointers to wrap.
 * @see coveo::linq::detail::make_deref_next_impl()
 */
template<typename Seq>
class deref_next_impl
{
public:
    using iterator_type = typename seq_traits<Seq>::iterator_type;

private:
    // Struct storing sequence and ending. Shared among delegates.
    struct deref_info {
        Seq seq_;               // Sequence of pointers.
        iterator_type iend_;    // Iterator pointing at end of seq_.

        explicit deref_info(Seq&& seq)
            : seq_(std::forward<Seq>(seq)),
              iend_(std::end(seq_)) { }

        // Cannot copy/move, stored in a shared_ptr
        deref_info(const deref_info&) = delete;
        deref_info& operator=(const deref_info&) = delete;
    };
    using deref_info_sp = std::shared_ptr<deref_info>;

    deref_info_sp spinfo_;      // Shared sequence info.
    iterator_type icur_;        // Iterator pointing at current element.

public:
    explicit deref_next_impl(Seq&& seq)
        : spinfo_(std::make_shared<deref_info>(std::forward<Seq>(seq))),
          icur_(std::begin(spinfo_->seq_)) { }

    auto operator()() -> typename seq_traits<Seq>::value_type {
        typename seq_traits<Seq>::value_type pobj = nullptr;
        if (icur_ != spinfo_->iend_) {
            pobj = *icur_;
            ++icur_;
        }
        return pobj;
    }
};

/// @cond

/**
 * @internal
 * @brief Helper to create dereferencing next delegates.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Helper function to create instances of <tt>coveo::linq::detail::deref_next_impl</tt>.
 * Allows easier creation by deducing the sequence type.
 *
 * @tparam Seq Sequence containing pointers to wrap.
 * @see coveo::linq::detail::deref_next_impl
 */
template<typename Seq>
auto make_deref_next_impl(Seq&& seq) -> deref_next_impl<Seq> {
    return deref_next_impl<Seq>(std::forward<Seq>(seq));
}

/// @endcond

/**
 * @internal
 * @brief Identity unary predicate.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Transparent unary predictate that returns values unmodified.
 */
template<typename = void>
struct identity {
    template<typename T>
    auto operator()(T&& obj) const -> decltype(std::forward<T>(obj)) {
        return std::forward<T>(obj);
    }
};

/**
 * @internal
 * @brief Pair-creating binary predicate.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Transparent binary predictate that returns pairs of its two arguments.
 */
template<typename = void>
struct pair_of {
    template<typename T, typename U>
    auto operator()(T&& obj1, U&& obj2) const -> std::pair<T, U> {
        return std::pair<T, U>(std::forward<T>(obj1), std::forward<U>(obj2));
    }
};

/**
 * @internal
 * @brief Less-than predicate.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Transparent less-than binary predicate, like
 * <tt>std::less</tt> from C++14.
 */
template<typename = void>
struct less {
    template<typename T, typename U>
    auto operator()(T&& left, U&& right) const -> decltype(std::forward<T>(left) < std::forward<U>(right)) {
        return std::forward<T>(left) < std::forward<U>(right);
    }
};

/**
 * @internal
 * @brief Greater-than predicate.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Transparent greater-than binary predicate, like
 * <tt>std::greater</tt> from C++14.
 */
template<typename = void>
struct greater {
    template<typename T, typename U>
    auto operator()(T&& left, U&& right) const -> decltype(std::forward<T>(left) > std::forward<U>(right)) {
        return std::forward<T>(left) > std::forward<U>(right);
    }
};

/// @cond

/**
 * @internal
 * @brief Helper to create <tt>std::unique_ptr</tt>s.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Creates an object and stores it in a <tt>std::unique_ptr</tt>.
 * Like <tt>std::make_unique()</tt> from C++14.
 *
 * @tparam T Object to store in the @c unique_ptr.
 * @tparam Args Arguments to forward to the constructor of @c T.
 */
template<typename T, typename... Args>
auto make_unique(Args&&... args) -> std::unique_ptr<T> {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/// @endcond

/**
 * @internal
 * @brief <tt>coveo::linq::aggregate()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::aggregate()</tt> LINQ operator.
 * Version with a single argument (aggregate function).
 *
 * @tparam F Function to aggregate the values.
 * @see coveo::linq::aggregate()
 */
template<typename F>
class aggregate_impl_1
{
private:
    const F& agg_f_;

public:
    explicit aggregate_impl_1(const F& agg_f)
        : agg_f_(agg_f) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename std::decay<decltype(*std::begin(seq))>::type {
        auto it = std::begin(seq);
        auto end = std::end(seq);
        if (it == end) {
            throw_linq_empty_sequence();
        }
        auto aggregate(*it);
        for (++it; it != end; ++it) {
            aggregate = agg_f_(aggregate, *it);
        }
        return aggregate;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::aggregate()</tt> implementation (2).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::aggregate()</tt> LINQ operator.
 * Version with two arguments (initial value and aggregate function).
 *
 * @tparam Acc Type of aggregate value.
 * @tparam F Function to aggregate the values.
 * @see coveo::linq::aggregate()
 */
template<typename Acc, typename F>
class aggregate_impl_2
{
private:
    const Acc& seed_;
    const F& agg_f_;

public:
    aggregate_impl_2(const Acc& seed, const F& agg_f)
        : seed_(seed), agg_f_(agg_f) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> Acc {
        Acc aggregate(seed_);
        for (auto&& element : seq) {
            aggregate = agg_f_(aggregate, element);
        }
        return aggregate;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::aggregate()</tt> implementation (3).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::aggregate()</tt> LINQ operator.
 * Version with three arguments (initial value, aggregate function and
 * result function).
 *
 * @tparam Acc Type of aggregate value.
 * @tparam F Function to aggregate the values.
 * @tparam RF Function called to produce final result from aggregate.
 * @see coveo::linq::aggregate()
 */
template<typename Acc, typename F, typename RF>
class aggregate_impl_3 : public aggregate_impl_2<Acc, F>
{
private:
    const RF& result_f_;

public:
    aggregate_impl_3(const Acc& seed, const F& agg_f, const RF& result_f)
        : aggregate_impl_2<Acc, F>(seed, agg_f), result_f_(result_f) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(result_f_(std::declval<Acc>())) {
        return result_f_(aggregate_impl_2<Acc, F>::operator()(seq));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::all()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::all()</tt> LINQ operator.
 *
 * @tparam Pred Predicate used on sequence elements.
 * @see coveo::linq::all()
 */
template<typename Pred>
class all_impl
{
private:
    const Pred& pred_;

public:
    explicit all_impl(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> bool {
        return std::all_of(std::begin(seq), std::end(seq), pred_);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::any()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::any()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::any()
 */
template<typename = void>
class any_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> bool {
        return std::begin(seq) != std::end(seq);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::any()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::any()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate used on sequence elements.
 * @see coveo::linq::any()
 */
template<typename Pred>
class any_impl_1
{
private:
    const Pred& pred_;

public:
    explicit any_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> bool {
        return std::any_of(std::begin(seq), std::end(seq), pred_);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::average()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::average()</tt> LINQ operator.
 *
 * @tparam F Function to get numeric value for each element.
 * @see coveo::linq::average()
 */
template<typename F>
class average_impl
{
private:
    const F& num_f_;

public:
    explicit average_impl(const F& num_f)
        : num_f_(num_f) { }

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> typename std::decay<decltype(num_f_(*std::begin(seq)))>::type
    {
        auto it = std::begin(seq);
        auto end = std::end(seq);
        if (it == end) {
            throw_linq_empty_sequence();
        }
        auto total = num_f_(*it);
        decltype(total) count = 1;
        for (++it; it != end; ++it) {
            total += num_f_(*it);
            ++count;
        }
        return total / count;
    }
};

/**
 * @internal
 * @brief Selector for <tt>coveo::linq::cast()</tt>.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Selector used to implement the <tt>coveo::linq::cast()</tt>
 * LINQ operator through <tt>coveo::linq::select()</tt>.
 *
 * @tparam U Type to cast the elements to.
 * @see coveo::linq::cast()
 */
template<typename U>
class cast_selector
{
public:
    template<typename T>
    auto operator()(T&& obj) const -> U {
        return static_cast<U>(obj);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::concat()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::concat()</tt> LINQ operator.
 *
 * @tparam Seq2 Second sequence to concatenate (the first one was passed
 *              via the call to <tt>coveo::linq::from()</tt>).
 * @see coveo::linq::concat()
 */
template<typename Seq2>
class concat_impl
{
private:
    // Implementation of next delegate that concatenates two sequences
    template<typename Seq1>
    class next_impl
    {
    public:
        // Type of element returned by this next delegate. The elements will be const
        // if at least one sequence is const.
        using enum_type         = typename std::conditional<std::is_const<typename seq_traits<Seq1>::value_type>::value ||
                                                            std::is_const<typename seq_traits<Seq2>::value_type>::value,
                                                            typename seq_traits<Seq1>::const_value_type,
                                                            typename seq_traits<Seq1>::value_type>::type;
        using enum_pointer      = typename seq_element_traits<enum_type>::pointer;
        using enum_reference    = typename seq_element_traits<enum_type>::reference;

    private:
        // Type of iterators for both sequences
        using first_iterator_type   = typename seq_traits<Seq1>::iterator_type;
        using second_iterator_type  = typename seq_traits<Seq2>::iterator_type;

        // Information used to concatenate sequences. Shared among delegates.
        class concat_info
        {
        private:
            Seq1 seq1_;                     // First sequence to concatenate
            first_iterator_type iend1_;     // End of first sequence
            Seq2 seq2_;                     // Second sequence to concatenate
            second_iterator_type iend2_;    // End of second sequence

        public:
            concat_info(Seq1&& seq1, Seq2&& seq2)
                : seq1_(std::forward<Seq1>(seq1)),
                  iend1_(std::end(seq1_)),
                  seq2_(std::forward<Seq2>(seq2)),
                  iend2_(std::end(seq2_)) { }

            // Cannot move/copy, stored in a shared_ptr
            concat_info(const concat_info&) = delete;
            concat_info& operator=(const concat_info&) = delete;

            first_iterator_type first_begin() {
                return std::begin(seq1_);
            }
            second_iterator_type second_begin() {
                return std::begin(seq2_);
            }

            // Returns next element from one of the sequences or nullptr when done
            auto get_next(first_iterator_type& icur1, second_iterator_type& icur2) -> enum_pointer {
                // First return all elements from first sequence, then from second sequence.
                enum_pointer pobj = nullptr;
                if (icur1 != iend1_) {
                    enum_reference robj = *icur1;
                    pobj = std::addressof(robj);
                    ++icur1;
                } else if (icur2 != iend2_) {
                    enum_reference robj = *icur2;
                    pobj = std::addressof(robj);
                    ++icur2;
                }
                return pobj;
            }
        };
        using concat_info_sp = std::shared_ptr<concat_info>;

        concat_info_sp spinfo_;         // Shared concat info
        first_iterator_type icur1_;     // Current position in first sequence
        second_iterator_type icur2_;    // Current position in second sequence

    public:
        next_impl(Seq1&& seq1, Seq2&& seq2)
            : spinfo_(std::make_shared<concat_info>(std::forward<Seq1>(seq1), std::forward<Seq2>(seq2))),
              icur1_(spinfo_->first_begin()), icur2_(spinfo_->second_begin()) { }

        auto operator()() -> decltype(spinfo_->get_next(icur1_, icur2_)) {
            return spinfo_->get_next(icur1_, icur2_);
        }
    };

private:
    Seq2 seq2_;     // Second sequence (possibly a ref)

public:
    explicit concat_impl(Seq2&& seq2)
        : seq2_(std::forward<Seq2>(seq2)) { }

    // Movable but not copyable
    concat_impl(const concat_impl&) = delete;
    concat_impl(concat_impl&&) = default;
    concat_impl& operator=(const concat_impl&) = delete;
    concat_impl& operator=(concat_impl&&) = default;

    template<typename Seq1>
    auto operator()(Seq1&& seq1)
        -> enumerable<typename next_impl<Seq1>::enum_type>
    {
        auto siz1 = try_get_size_delegate(seq1);
        auto siz2 = try_get_size_delegate(seq2_);
        typename enumerable<typename next_impl<Seq1>::enum_type>::size_delegate siz;
        if (siz1 != nullptr && siz2 != nullptr) {
            std::size_t size = siz1() + siz2();
            siz = [size]() -> std::size_t { return size; };
        }
        return { next_impl<Seq1>(std::forward<Seq1>(seq1), std::forward<Seq2>(seq2_)),
                 siz };
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::contains()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::contains()</tt> LINQ operator.
 * Version with one argument (object to look for).
 *
 * @tparam T Type of object to look for.
 * @see coveo::linq::contains()
 */
template<typename T>
class contains_impl_1
{
private:
    const T& obj_;  // Object to look for.

public:
    explicit contains_impl_1(const T& obj)
        : obj_(obj) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> bool {
        bool found = false;
        for (auto&& element : seq) {
            if (element == obj_) {
                found = true;
                break;
            }
        }
        return found;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::contains()</tt> implementation (2).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::contains()</tt> LINQ operator.
 * Version with two arguments (object to look for and predicate).
 *
 * @tparam T Type of object to look for.
 * @tparam Pred Predicate used to compare elements.
 * @see coveo::linq::contains()
 */
template<typename T, typename Pred>
class contains_impl_2
{
private:
    const T& obj_;      // Object to look for.
    const Pred& pred_;  // Predicate used to compare objects.

public:
    contains_impl_2(const T& obj, const Pred& pred)
        : obj_(obj), pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> bool {
        bool found = false;
        for (auto&& element : seq) {
            if (pred_(element, obj_)) {
                found = true;
                break;
            }
        }
        return found;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::count()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::count()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::count()
 */
template<typename = void>
class count_impl_0
{
private:
    // Used if sequence has size() method
    template<typename Seq,
             typename = typename std::enable_if<coveo::detail::has_size_const_method<typename std::decay<Seq>::type>::value, void>::type>
    auto impl(Seq&& seq) -> std::size_t {
        return seq.size();
    }

    // Used otherwise (no choice but to use distance)
    template<typename Seq,
             typename _V = typename std::enable_if<!coveo::detail::has_size_const_method<typename std::decay<Seq>::type>::value, void*>::type>
    auto impl(Seq&& seq, _V = nullptr) -> std::size_t {
        return static_cast<std::size_t>(std::distance(std::begin(seq), std::end(seq)));
    }

public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> std::size_t {
        return impl(std::forward<Seq>(seq));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::count()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::count()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate used on elements.
 * @see coveo::linq::count()
 */
template<typename Pred>
class count_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy.

public:
    explicit count_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> std::size_t {
        return static_cast<std::size_t>(std::count_if(std::begin(seq), std::end(seq), pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::default_if_empty()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::default_if_empty()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::default_if_empty()
 */
template<typename = void>
class default_if_empty_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::const_value_type>
    {
        enumerable<typename seq_traits<Seq>::const_value_type> e;
        if (any_impl_0<>()(std::forward<Seq>(seq))) {
            e = enumerate_container(std::forward<Seq>(seq));
        } else {
            e = enumerate_one(typename seq_traits<Seq>::raw_value_type());
        }
        return e;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::default_if_empty()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::default_if_empty()</tt> LINQ operator.
 * Version with one argument (default value).
 *
 * @tparam T Type of default value.
 * @see coveo::linq::default_if_empty()
 */
template<typename T>
class default_if_empty_impl_1
{
private:
    const T& obj_;  // Object to use to create default value if empty.

public:
    explicit default_if_empty_impl_1(const T& obj)
        : obj_(obj) { }

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::const_value_type>
    {
        enumerable<typename seq_traits<Seq>::const_value_type> e;
        if (any_impl_0<>()(std::forward<Seq>(seq))) {
            e = enumerate_container(std::forward<Seq>(seq));
        } else {
            e = enumerate_one(typename seq_traits<Seq>::raw_value_type(obj_));
        }
        return e;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::distinct()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::distinct()</tt> LINQ operator.
 *
 * @tparam Pred Predicate used to compare elements.
 * @see coveo::linq::distinct()
 */
template<typename Pred>
class distinct_impl
{
private:
    // Implementation of next delegate that filters duplicate elements
    template<typename Seq>
    class next_impl
    {
    private:
        // Type of iterator for the sequence
        using iterator_type     = typename seq_traits<Seq>::iterator_type;

        // Set storing pointers to seen elements
        using seen_elements_set = std::set<typename seq_traits<Seq>::const_pointer, deref_cmp<proxy_cmp<Pred>>>;

        // Info used to produce distinct elements. Shared among delegates.
        class distinct_info
        {
        private:
            Seq seq_;               // Sequence being iterated
            iterator_type iend_;    // Iterator pointing at end of sequence
            Pred pred_;             // Predicate ordering the elements

        public:
            distinct_info(Seq&& seq, Pred&& pred)
                : seq_(std::forward<Seq>(seq)),
                  iend_(std::end(seq_)),
                  pred_(std::forward<Pred>(pred)) { }

            // Cannot copy/move, stored in a shared_ptr
            distinct_info(const distinct_info&) = delete;
            distinct_info& operator=(const distinct_info&) = delete;

            iterator_type seq_begin() {
                return std::begin(seq_);
            }
            seen_elements_set init_seen_elements() {
                return seen_elements_set(deref_cmp<proxy_cmp<Pred>>(proxy_cmp<Pred>(pred_)));
            }

            // Returns next distinct element or nullptr when done
            auto get_next(iterator_type& icur, seen_elements_set& seen)
                -> typename seq_traits<Seq>::pointer
            {
                typename seq_traits<Seq>::pointer pobj = nullptr;
                for (; pobj == nullptr && icur != iend_; ++icur) {
                    typename seq_traits<Seq>::reference robjtmp = *icur;
                    auto pobjtmp = std::addressof(robjtmp);
                    if (seen.emplace(pobjtmp).second) {
                        // Not seen yet, return this element.
                        pobj = pobjtmp;
                    }
                }
                return pobj;
            }
        };
        using distinct_info_sp = std::shared_ptr<distinct_info>;

        distinct_info_sp spinfo_;   // Shared info
        iterator_type icur_;        // Iterator pointing at current element
        seen_elements_set seen_;    // Set of seen elements

    public:
        next_impl(Seq&& seq, Pred&& pred)
            : spinfo_(std::make_shared<distinct_info>(std::forward<Seq>(seq), std::forward<Pred>(pred))),
              icur_(spinfo_->seq_begin()), seen_(spinfo_->init_seen_elements()) { }

        auto operator()() -> decltype(spinfo_->get_next(icur_, seen_)) {
            return spinfo_->get_next(icur_, seen_);
        }
    };

private:
    Pred pred_;     // Predicate used to compare elements

public:
    explicit distinct_impl(Pred&& pred)
        : pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    distinct_impl(const distinct_impl&) = delete;
    distinct_impl(distinct_impl&&) = default;
    distinct_impl& operator=(const distinct_impl&) = delete;
    distinct_impl& operator=(distinct_impl&&) = default;

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        return next_impl<Seq>(std::forward<Seq>(seq), std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::element_at()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::element_at()</tt> LINQ operator.
 *
 * @see coveo::linq::element_at()
 */
template<typename = void>
class element_at_impl
{
private:
    std::size_t n_;     // Index of element to fetch.

private:
    // If we have random-access iterators, we can perform fast computations
    template<typename Seq>
    auto impl(Seq&& seq, std::random_access_iterator_tag) -> decltype(*std::begin(seq)) {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (static_cast<std::size_t>(iend - icur) <= n_) {
            throw_linq_out_of_range();
        }
        icur += n_;
        return *icur;
    }

    // Otherwise, we can only move by hand
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag) -> decltype(*std::begin(seq)) {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        for (std::size_t i = 0; i < n_ && icur != iend; ++i, ++icur) {
        }
        if (icur == iend) {
            throw_linq_out_of_range();
        }
        return *icur;
    }

public:
    explicit element_at_impl(std::size_t n)
        : n_(n) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::element_at_or_default()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::element_at_or_default()</tt> LINQ operator.
 *
 * @see coveo::linq::element_at_or_default()
 */
template<typename = void>
class element_at_or_default_impl
{
private:
    std::size_t n_;     // Index of element to fetch.

private:
    // If we have random-access iterators, we can perform fast computations
    template<typename Seq>
    auto impl(Seq&& seq, std::random_access_iterator_tag) -> typename seq_traits<Seq>::raw_value_type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        return static_cast<std::size_t>(iend - icur) > n_ ? *(icur + n_)
                                                          : typename seq_traits<Seq>::raw_value_type();
    }

    // Otherwise, we can only move by hand
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag) -> typename seq_traits<Seq>::raw_value_type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        for (std::size_t i = 0; i < n_ && icur != iend; ++i, ++icur) {
        }
        return icur != iend ? *icur
                            : typename seq_traits<Seq>::raw_value_type();
    }

public:
    element_at_or_default_impl(std::size_t n)
        : n_(n) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::except()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::except()</tt> LINQ operator.
 *
 * @tparam Seq2 Sequence to except (e.g. substract) from the first.
 * @tparam Pred Predicate used to compare elements.
 * @see coveo::linq::except()
 */
template<typename Seq2, typename Pred>
class except_impl
{
private:
    // Implementation of next delegate that filters out elements in the second sequence
    template<typename Seq1>
    class next_impl
    {
    private:
        // Vector of pointers to elements in the second sequence. Used to filter them out.
        using elements_to_filter_v  = std::vector<typename seq_traits<Seq2>::const_pointer>;

        // Type of iterator for the first sequence.
        using first_iterator_type   = typename seq_traits<Seq1>::iterator_type;

        // Bean storing info about elements to filter out. Shared among delegate instances.
        class filter_info
        {
        private:
            Seq1 seq1_;                         // Sequence of elements to scan and return.
            first_iterator_type iend1_;         // End of seq1_.
            Seq2 seq2_;                         // Sequence of elements to filter out.
            deref_cmp<Pred> pred_;              // Predicate used to sort and search through v_to_filter_.
            elements_to_filter_v v_to_filter_;  // Elements to filter out. Late-initialized.
            bool init_called_ = false;          // Init flag for v_to_filter_.

            void init() {
                try_reserve(v_to_filter_, seq2_);
                auto icur2 = std::begin(seq2_);
                auto iend2 = std::end(seq2_);
                for (; icur2 != iend2; ++icur2) {
                    typename seq_traits<Seq2>::const_reference robjtmp = *icur2;
                    v_to_filter_.emplace_back(std::addressof(robjtmp));
                }
                std::sort(v_to_filter_.begin(), v_to_filter_.end(), pred_);
                init_called_ = true;
            }

        public:
            filter_info(Seq1&& seq1, Seq2&& seq2, Pred&& pred)
                : seq1_(std::forward<Seq1>(seq1)), iend1_(std::end(seq1_)),
                  seq2_(std::forward<Seq2>(seq2)), pred_(std::forward<Pred>(pred)) { }

            // No move or copy, it's stored in a shared_ptr
            filter_info(const filter_info&) = delete;
            filter_info& operator=(const filter_info&) = delete;

            first_iterator_type first_begin() {
                return std::begin(seq1_);
            }

            bool filtered(const typename seq_traits<Seq1>::pointer pobj) {
                if (!init_called_) {
                    // Init elements to filter on first call
                    init();
                }
                return std::binary_search(v_to_filter_.cbegin(), v_to_filter_.cend(), pobj, pred_);
            }

            // Returns next non-filtered element or nullptr when done
            auto get_next(first_iterator_type& icur1) -> typename seq_traits<Seq1>::pointer {
                typename seq_traits<Seq1>::pointer pobj = nullptr;
                for (; pobj == nullptr && icur1 != iend1_; ++icur1) {
                    typename seq_traits<Seq1>::reference robjtmp = *icur1;
                    auto pobjtmp = std::addressof(robjtmp);
                    if (!filtered(pobjtmp)) {
                        pobj = pobjtmp;
                    }
                }
                return pobj;
            }
        };
        using filter_info_sp = std::shared_ptr<filter_info>;

        filter_info_sp spfilter_;   // Bean containing filter info
        first_iterator_type icur_;  // Current position in first sequence

    public:
        next_impl(Seq1&& seq1, Seq2&& seq2, Pred&& pred)
            : spfilter_(std::make_shared<filter_info>(std::forward<Seq1>(seq1),
                                                      std::forward<Seq2>(seq2),
                                                      std::forward<Pred>(pred))),
              icur_(spfilter_->first_begin()) { }

        auto operator()() -> decltype(spfilter_->get_next(icur_)) {
            return spfilter_->get_next(icur_);
        }
    };

private:
    Seq2 seq2_;     // Sequence of elements to filter out
    Pred pred_;     // Predicate used to compare elements

public:
    except_impl(Seq2&& seq2, Pred&& pred)
        : seq2_(std::forward<Seq2>(seq2)), pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    except_impl(const except_impl&) = delete;
    except_impl(except_impl&&) = default;
    except_impl& operator=(const except_impl&) = delete;
    except_impl& operator=(except_impl&&) = default;

    template<typename Seq1>
    auto operator()(Seq1&& seq1)
        -> enumerable<typename seq_traits<Seq1>::value_type>
    {
        return next_impl<Seq1>(std::forward<Seq1>(seq1),
                               std::forward<Seq2>(seq2_),
                               std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::first()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::first()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::first()
 */
template<typename = void>
class first_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        auto icur = std::begin(seq);
        if (icur == std::end(seq)) {
            throw_linq_empty_sequence();
        }
        return *icur;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::first()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::first()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate used to locate element.
 * @see coveo::linq::contains()
 */
template<typename Pred>
class first_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy.

public:
    explicit first_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (icur == iend) {
            throw_linq_empty_sequence();
        }
        auto ifound = std::find_if(icur, iend, pred_);
        if (ifound == iend) {
            throw_linq_out_of_range();
        }
        return *ifound;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::first_or_default()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::first_or_default()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::first_or_default()
 */
template<typename = void>
class first_or_default_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        auto icur = std::begin(seq);
        return icur != std::end(seq) ? *icur
                                     : typename seq_traits<Seq>::raw_value_type();
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::first_or_default()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::first_or_default()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate used to locate element.
 * @see coveo::linq::first_or_default()
 */
template<typename Pred>
class first_or_default_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy.

public:
    explicit first_or_default_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        auto iend = std::end(seq);
        auto ifound = std::find_if(std::begin(seq), iend, pred_);
        return ifound != iend ? *ifound
                              : typename seq_traits<Seq>::raw_value_type();
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::group_by()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::group_by()</tt>,
 * <tt>coveo::linq::group_values_by()</tt>, <tt>coveo::linq::group_by_and_fold()</tt>
 * and <tt>coveo::linq::group_values_by_and_fold</tt> LINQ operators.
 *
 * @tparam KeySelector Selector used to obtain keys for each element.
 * @tparam ValueSelector Selector used to obtain values for each element.
 * @tparam ResultSelector Selector used to produce the final results from
 *                        elements' keys and corresponding values.
 * @tparam Pred Predicate used to compare keys.
 * @see coveo::linq::group_by()
 * @see coveo::linq::group_values_by()
 * @see coveo::linq::group_by_and_fold()
 * @see coveo::linq::group_values_by_and_fold()
 */
template<typename KeySelector,
         typename ValueSelector,
         typename ResultSelector,
         typename Pred>
class group_by_impl
{
private:
    // Implementation of next delegate that returns group information
    template<typename Seq>
    class next_impl
    {
    public:
        // Key and value types returned by selectors.
        using key               = decltype(std::declval<KeySelector>()(std::declval<typename seq_traits<Seq>::reference>()));
        using value             = decltype(std::declval<ValueSelector>()(std::declval<typename seq_traits<Seq>::reference>()));

        // Vector of values sharing a common key.
        using value_v           = std::vector<typename std::decay<value>::type>;
        using values            = decltype(enumerate_container(std::declval<value_v&&>()));

        // Map that stores keys and their corresponding values.
        using values_by_key_m   = std::map<typename std::decay<key>::type, value_v, proxy_cmp<Pred>>;

        // Result returned by result selector.
        using result            = decltype(std::declval<ResultSelector>()(std::declval<key>(), std::declval<values>()));

        // Vector of results returned by this next delegate.
        using result_v          = std::vector<typename std::decay<result>::type>;

    private:
        // Bean storing group information. Shared among delegates in a shared_ptr.
        class groups_info
        {
        private:
            Seq seq_;                       // Sequence containing the elements
            KeySelector key_sel_;           // Returns keys for elements
            ValueSelector value_sel_;       // Returns values for elements
            ResultSelector result_sel_;     // Converts groups into end results
            Pred pred_;                     // Compares keys
            result_v results_;              // Vector of end results
            bool init_called_ = false;      // Whether results_ has been initialized

            void init() {
                // First build map of groups
                values_by_key_m groups{proxy_cmp<Pred>(pred_)};
                for (auto&& obj : seq_) {
                    groups[key_sel_(obj)].emplace_back(value_sel_(obj));
                }

                // Now build vector of results
                // Note that since we no longer need the map afterwards, we can actually move
                // the vectors stored as map values into the results vector.
                results_.reserve(groups.size());
                for (auto&& group_pair : groups) {
                    results_.emplace_back(result_sel_(group_pair.first,
                                                      enumerate_container(std::move(group_pair.second))));
                }

                init_called_ = true;
            }

        public:
            groups_info(Seq&& seq, KeySelector&& key_sel, ValueSelector&& value_sel,
                        ResultSelector&& result_sel, Pred&& pred)
                : seq_(std::forward<Seq>(seq)),
                  key_sel_(std::forward<KeySelector>(key_sel)),
                  value_sel_(std::forward<ValueSelector>(value_sel)),
                  result_sel_(std::forward<ResultSelector>(result_sel)),
                  pred_(std::forward<Pred>(pred)) { }

            // Not copyable/movable, stored in a shared_ptr
            groups_info(const groups_info&) = delete;
            groups_info& operator=(const groups_info&) = delete;

            const result_v& get_results() {
                if (!init_called_) {
                    init();
                }
                return results_;
            }
        };
        using groups_info_sp = std::shared_ptr<groups_info>;

        groups_info_sp spgroups_;                   // Information about groups
        typename result_v::const_iterator icurr_{}; // Iterator pointing at current result.
        typename result_v::const_iterator iendr_{}; // Iterator pointing at end of result vector.
        bool init_called_ = false;                  // Whether icurr_ and iendr_ have been initialized

        void init() {
            const auto& results = spgroups_->get_results();
            icurr_ = std::begin(results);
            iendr_ = std::end(results);
            init_called_ = true;
        }

    public:
        next_impl(Seq&& seq, KeySelector&& key_sel, ValueSelector&& value_sel,
                  ResultSelector&& result_sel, Pred&& pred)
            : spgroups_(std::make_shared<groups_info>(std::forward<Seq>(seq),
                                                      std::forward<KeySelector>(key_sel),
                                                      std::forward<ValueSelector>(value_sel),
                                                      std::forward<ResultSelector>(result_sel),
                                                      std::forward<Pred>(pred))) { }

        auto operator()() -> typename seq_traits<result_v>::const_pointer {
            // Init iterators on first call
            if (!init_called_) {
                init();
            }
            typename seq_traits<result_v>::const_pointer pobj = nullptr;
            if (icurr_ != iendr_) {
                typename seq_traits<result_v>::const_reference robj = *icurr_;
                pobj = std::addressof(robj);
                ++icurr_;
            }
            return pobj;
        }
    };

private:
    KeySelector key_sel_;       // Selector that provides keys for each element
    ValueSelector value_sel_;   // Selector that provides values for each element
    ResultSelector result_sel_; // Selector that converts each group into a final result
    Pred pred_;                 // Predicate used to compare keys

public:
    group_by_impl(KeySelector&& key_sel, ValueSelector&& value_sel,
                  ResultSelector&& result_sel, Pred&& pred)
        : key_sel_(std::forward<KeySelector>(key_sel)),
          value_sel_(std::forward<ValueSelector>(value_sel)),
          result_sel_(std::forward<ResultSelector>(result_sel)),
          pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    group_by_impl(const group_by_impl&) = delete;
    group_by_impl(group_by_impl&&) = default;
    group_by_impl& operator=(const group_by_impl&) = delete;
    group_by_impl& operator=(group_by_impl&&) = default;

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<typename next_impl<Seq>::result_v>::const_value_type>
    {
        return next_impl<Seq>(std::forward<Seq>(seq),
                              std::forward<KeySelector>(key_sel_),
                              std::forward<ValueSelector>(value_sel_),
                              std::forward<ResultSelector>(result_sel_),
                              std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::group_join()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::group_join()</tt> LINQ operator.
 *
 * @tparam InnerSeq Inner sequence to join (the outer sequence will have
 *                  been provided via the call to <tt>coveo::linq::from()</tt>).
 * @tparam OuterKeySelector Selector used to fetch keys from elements
 *                          in the outer sequence.
 * @tparam InnerKeySelector Selector used to fetch keys from elements
 *                          in the inner sequence.
 * @tparam ResultSelector Selector used to produce final results from
 *                        elements in outer sequence and corresponding
 *                        group of elements in inner sequence.
 * @tparam Pred Predicate used to compare keys.
 * @see coveo::linq::group_join()
 */
template<typename InnerSeq,
         typename OuterKeySelector,
         typename InnerKeySelector,
         typename ResultSelector,
         typename Pred>
class group_join_impl
{
private:
    // Implementation of next delegate that returns final results
    template<typename OuterSeq>
    class next_impl
    {
    public:
        // Key returned by key selectors.
        using key               = decltype(std::declval<OuterKeySelector>()(std::declval<typename seq_traits<OuterSeq>::reference>()));

        // Group of pointers to elements from the inner sequence that share a common key.
        using inner_element_v   = std::vector<typename seq_traits<InnerSeq>::pointer>;
        using inner_elements    = enumerable<typename seq_traits<InnerSeq>::const_value_type>;
    
        // Result returned by result selector.
        using result            = decltype(std::declval<ResultSelector>()(std::declval<typename seq_traits<OuterSeq>::reference>(),
                                                                          std::declval<inner_elements>()));

        // Vector of results returned by this next delegate.
        using result_v          = std::vector<typename std::decay<result>::type>;

    private:
        // Bean storing groups information. Shared among delegates in a shared_ptr.
        class groups_info
        {
        private:
            OuterSeq outer_seq_;                // Outer sequence.
            InnerSeq inner_seq_;                // Inner sequence from which to create groups.
            OuterKeySelector outer_key_sel_;    // Key selector for outer sequence.
            InnerKeySelector inner_key_sel_;    // Key selector for inner sequence.
            ResultSelector result_sel_;         // Selector converting groups into final results.
            Pred pred_;                         // Predicate used to compare keys.
            result_v results_;                  // Vector of final results.
            bool init_called_ = false;          // Whether results_ has been initialized.

            void init() {
                // Build map of groups of elements from inner sequence.
                using groups_m = std::map<typename std::decay<key>::type, inner_element_v, proxy_cmp<Pred>>;
                groups_m keyed_inner_elems{proxy_cmp<Pred>(pred_)};
                for (auto&& inner_elem : inner_seq_) {
                    typename seq_traits<InnerSeq>::reference robj = inner_elem;
                    keyed_inner_elems[inner_key_sel_(inner_elem)].emplace_back(std::addressof(robj));
                }

                // Iterate outer sequence and build final results by matching the elements with
                // the groups we built earlier.
                try_reserve(results_, outer_seq_);
                const auto iendki = keyed_inner_elems.end();
                for (auto&& outer_elem : outer_seq_) {
                    const key outer_key = outer_key_sel_(outer_elem);
                    const auto icurki = keyed_inner_elems.find(outer_key);
                    inner_elements inner_elems;
                    if (icurki != iendki) {
                        const std::size_t inner_size = icurki->second.size();
                        inner_elems = inner_elements(make_deref_next_impl(icurki->second),
                                                     [inner_size]() -> std::size_t { return inner_size; });
                    }
                    results_.emplace_back(result_sel_(outer_elem, inner_elems));
                }

                init_called_ = true;
            }

        public:
            groups_info(OuterSeq&& outer_seq, InnerSeq&& inner_seq,
                        OuterKeySelector&& outer_key_sel, InnerKeySelector&& inner_key_sel,
                        ResultSelector&& result_sel, Pred&& pred)
                : outer_seq_(std::forward<OuterSeq>(outer_seq)),
                  inner_seq_(std::forward<InnerSeq>(inner_seq)),
                  outer_key_sel_(std::forward<OuterKeySelector>(outer_key_sel)),
                  inner_key_sel_(std::forward<InnerKeySelector>(inner_key_sel)),
                  result_sel_(std::forward<ResultSelector>(result_sel)),
                  pred_(std::forward<Pred>(pred)) { }

            // Not copyable/movable, stored in a shared_ptr
            groups_info(const groups_info&) = delete;
            groups_info& operator=(const groups_info&) = delete;

            const result_v& get_results() {
                if (!init_called_) {
                    init();
                }
                return results_;
            }
        };
        using groups_info_sp = std::shared_ptr<groups_info>;

        groups_info_sp spgroups_;                   // Information about groups and results
        typename result_v::const_iterator icurr_{}; // Iterator pointing at current result.
        typename result_v::const_iterator iendr_{}; // Iterator pointing at end of result vector.
        bool init_called_ = false;                  // Whether icurr_/iendr_ have been initialized.

        void init() {
            const auto& results = spgroups_->get_results();
            icurr_ = std::begin(results);
            iendr_ = std::end(results);
            init_called_ = true;
        }

    public:
        next_impl(OuterSeq&& outer_seq, InnerSeq&& inner_seq,
                  OuterKeySelector&& outer_key_sel, InnerKeySelector&& inner_key_sel,
                  ResultSelector&& result_sel, Pred&& pred)
            : spgroups_(std::make_shared<groups_info>(std::forward<OuterSeq>(outer_seq),
                                                      std::forward<InnerSeq>(inner_seq),
                                                      std::forward<OuterKeySelector>(outer_key_sel),
                                                      std::forward<InnerKeySelector>(inner_key_sel),
                                                      std::forward<ResultSelector>(result_sel),
                                                      std::forward<Pred>(pred))) { }

        auto operator()() -> typename seq_traits<result_v>::const_pointer {
            // Init iterators on first call
            if (!init_called_) {
                init();
            }
            typename seq_traits<result_v>::const_pointer pobj = nullptr;
            if (icurr_ != iendr_) {
                typename seq_traits<result_v>::const_reference robj = *icurr_;
                pobj = std::addressof(robj);
                ++icurr_;
            }
            return pobj;
        }
    };

private:
    InnerSeq inner_seq_;                // Inner sequence whose elements to group with outer elements with matching keys
    OuterKeySelector outer_key_sel_;    // Selector to get keys from elements in the outer sequence
    InnerKeySelector inner_key_sel_;    // Selector to get keys from elements in the inner sequence
    ResultSelector result_sel_;         // Selector to convert groups into final results
    Pred pred_;                         // Predicate used to compare keys

public:
    group_join_impl(InnerSeq&& inner_seq,
                    OuterKeySelector&& outer_key_sel,
                    InnerKeySelector&& inner_key_sel,
                    ResultSelector&& result_sel,
                    Pred&& pred)
        : inner_seq_(std::forward<InnerSeq>(inner_seq)),
          outer_key_sel_(std::forward<OuterKeySelector>(outer_key_sel)),
          inner_key_sel_(std::forward<InnerKeySelector>(inner_key_sel)),
          result_sel_(std::forward<ResultSelector>(result_sel)),
          pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    group_join_impl(const group_join_impl&) = delete;
    group_join_impl(group_join_impl&&) = default;
    group_join_impl& operator=(const group_join_impl&) = delete;
    group_join_impl& operator=(group_join_impl&&) = default;

    template<typename OuterSeq>
    auto operator()(OuterSeq&& outer_seq)
        -> enumerable<typename seq_traits<typename next_impl<OuterSeq>::result_v>::const_value_type>
    {
        return next_impl<OuterSeq>(std::forward<OuterSeq>(outer_seq),
                                   std::forward<InnerSeq>(inner_seq_),
                                   std::forward<OuterKeySelector>(outer_key_sel_),
                                   std::forward<InnerKeySelector>(inner_key_sel_),
                                   std::forward<ResultSelector>(result_sel_),
                                   std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::intersect()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::intersect()</tt> LINQ operator.
 *
 * @tparam Seq2 Second sequence to intersect (the first one will have been
 *              provided via the call to <tt>coveo::linq::from()</tt>).
 * @tparam Pred Predicate used to compare elements.
 * @see coveo::linq::intersect()
 */
template<typename Seq2, typename Pred>
class intersect_impl
{
public:
    // Implementation of next delegate that performs intersection
    template<typename Seq1>
    class next_impl
    {
    private:
        // Vector storing pointers to the elements of second sequence.
        using seq2_element_v        = std::vector<typename seq_traits<Seq2>::pointer>;

        // Type of iterators for the sequences.
        using first_iterator_type   = typename seq_traits<Seq1>::iterator_type;

        // Information about intersection. Shared among delegates through smart_ptr.
        class intersect_info
        {
        private:
            Seq1 seq1_;                     // First sequence to intersect.
            first_iterator_type iend1_;     // Iterator pointing at end of first sequence.
            Seq2 seq2_;                     // Second sequence to intersect.
            deref_cmp<Pred> pred_;          // Predicate used to sort and search through v_in_seq2_.
            seq2_element_v v_in_seq2_;      // Vector of elements from second sequence.
            bool init_called_ = false;      // Whether v_in_seq2_ has been initialized.

            void init() {
                // Add all elements from second sequence to a vector and sort it.
                try_reserve(v_in_seq2_, seq2_);
                auto icur2 = std::begin(seq2_);
                auto iend2 = std::end(seq2_);
                for (; icur2 != iend2; ++icur2) {
                    typename seq_traits<Seq2>::reference robjtmp = *icur2;
                    v_in_seq2_.emplace_back(std::addressof(robjtmp));
                }
                std::sort(v_in_seq2_.begin(), v_in_seq2_.end(), pred_);
                init_called_ = true;
            }

        public:
            intersect_info(Seq1&& seq1, Seq2&& seq2, Pred&& pred)
                : seq1_(std::forward<Seq1>(seq1)),
                  iend1_(std::end(seq1_)),
                  seq2_(std::forward<Seq2>(seq2)),
                  pred_(std::forward<Pred>(pred)) { }

            // Not copyable/movable, stored in a shared_ptr
            intersect_info(const intersect_info&) = delete;
            intersect_info& operator=(const intersect_info&) = delete;

            first_iterator_type first_begin() {
                return std::begin(seq1_);
            }

            bool is_in_seq2(const typename seq_traits<Seq1>::pointer pobj) {
                if (!init_called_) {
                    init();
                }
                return std::binary_search(v_in_seq2_.cbegin(), v_in_seq2_.cend(), pobj, pred_);
            }

            // Returns next element that is both in first and second sequence or nullptr when done
            auto get_next(first_iterator_type& icur1) -> typename seq_traits<Seq1>::pointer {
                typename seq_traits<Seq1>::pointer pobj = nullptr;
                for (; pobj == nullptr && icur1 != iend1_; ++icur1) {
                    typename seq_traits<Seq1>::reference robjtmp = *icur1;
                    auto pobjtmp = std::addressof(robjtmp);
                    if (is_in_seq2(pobjtmp)) {
                        pobj = pobjtmp;
                    }
                }
                return pobj;
            }
        };
        using intersect_info_sp = std::shared_ptr<intersect_info>;

        intersect_info_sp spint_info_;      // Intersection information.
        first_iterator_type icur_;          // Current position in first sequence.

    public:
        next_impl(Seq1&& seq1, Seq2&& seq2, Pred&& pred)
            : spint_info_(std::make_shared<intersect_info>(std::forward<Seq1>(seq1),
                                                           std::forward<Seq2>(seq2),
                                                           std::forward<Pred>(pred))),
              icur_(spint_info_->first_begin()) { }

        auto operator()() -> decltype(spint_info_->get_next(icur_)) {
            return spint_info_->get_next(icur_);
        }
    };

private:
    Seq2 seq2_;     // Second sequence to intersect.
    Pred pred_;     // Predicate used to compare elements.

public:
    intersect_impl(Seq2&& seq2, Pred&& pred)
        : seq2_(std::forward<Seq2>(seq2)),
          pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    intersect_impl(const intersect_impl&) = delete;
    intersect_impl(intersect_impl&&) = default;
    intersect_impl& operator=(const intersect_impl&) = delete;
    intersect_impl& operator=(intersect_impl&&) = default;

    template<typename Seq1>
    auto operator()(Seq1&& seq1)
        -> enumerable<typename seq_traits<Seq1>::value_type>
    {
        return next_impl<Seq1>(std::forward<Seq1>(seq1),
                               std::forward<Seq2>(seq2_),
                               std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::join()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::join()</tt> LINQ operator.
 *
 * @tparam InnerSeq Inner sequence to join (the outer sequence will have been
 *                  provided by the call to <tt>coveo::linq::from()</tt>).
 * @tparam OuterKeySelector Selector used to fetch keys from elements
 *                          in the outer sequence.
 * @tparam InnerKeySelector Selector used to fetch keys from elements
 *                          in the inner sequence.
 * @tparam ResultSelector Selector used to produce final results from
 *                        elements from outer and inner sequence with
 *                        matching keys.
 * @tparam Pred Predicate used to compare keys.
 * @see coveo::linq::join()
 */
template<typename InnerSeq,
         typename OuterKeySelector,
         typename InnerKeySelector,
         typename ResultSelector,
         typename Pred>
class join_impl
{
private:
    // Implementation of next delegate that performs join
    template<typename OuterSeq>
    class next_impl
    {
    public:
        // Key returned by key selectors.
        using key               = decltype(std::declval<OuterKeySelector>()(std::declval<typename seq_traits<OuterSeq>::reference>()));

        // Group of pointers to elements from the inner sequence that share a common key.
        using inner_element_v   = std::vector<typename seq_traits<InnerSeq>::pointer>;
    
        // Result returned by result selector.
        using result            = decltype(std::declval<ResultSelector>()(std::declval<typename seq_traits<OuterSeq>::reference>(),
                                                                          std::declval<typename seq_traits<InnerSeq>::reference>()));

        // Vector of results returned by this next delegate.
        using result_v          = std::vector<typename std::decay<result>::type>;

    private:
        // Bean storing join information. Shared among delegates in a shared_ptr.
        class join_info
        {
        private:
            OuterSeq outer_seq_;                // Outer sequence.
            InnerSeq inner_seq_;                // Inner sequence to join with outer sequence.
            OuterKeySelector outer_key_sel_;    // Key selector for outer sequence.
            InnerKeySelector inner_key_sel_;    // Key selector for inner sequence.
            ResultSelector result_sel_;         // Selector converting joined elements into final results.
            Pred pred_;                         // Predicate used to compare keys.
            result_v results_;                  // Vector of final results.
            bool init_called_ = false;          // Whether results_ has been initialized.

            void init() {
                // Build map of groups of elements from inner sequence.
                using groups_m = std::map<typename std::decay<key>::type, inner_element_v, proxy_cmp<Pred>>;
                groups_m keyed_inner_elems{proxy_cmp<Pred>(pred_)};
                for (auto&& inner_elem : inner_seq_) {
                    typename seq_traits<InnerSeq>::reference robj = inner_elem;
                    keyed_inner_elems[inner_key_sel_(inner_elem)].emplace_back(std::addressof(robj));
                }

                // Iterate outer sequence and build final results by joining the elements with
                // those in the groups we built earlier.
                try_reserve(results_, inner_seq_);
                const auto iendki = keyed_inner_elems.end();
                for (auto&& outer_elem : outer_seq_) {
                    const key outer_key = outer_key_sel_(outer_elem);
                    const auto icurki = keyed_inner_elems.find(outer_key);
                    if (icurki != iendki) {
                        for (auto* pinner_elem : icurki->second) {
                            results_.emplace_back(result_sel_(outer_elem, *pinner_elem));
                        }
                    }
                }

                init_called_ = true;
            }

        public:
            join_info(OuterSeq&& outer_seq, InnerSeq&& inner_seq,
                      OuterKeySelector&& outer_key_sel, InnerKeySelector&& inner_key_sel,
                      ResultSelector&& result_sel, Pred&& pred)
                : outer_seq_(std::forward<OuterSeq>(outer_seq)),
                  inner_seq_(std::forward<InnerSeq>(inner_seq)),
                  outer_key_sel_(std::forward<OuterKeySelector>(outer_key_sel)),
                  inner_key_sel_(std::forward<InnerKeySelector>(inner_key_sel)),
                  result_sel_(std::forward<ResultSelector>(result_sel)),
                  pred_(std::forward<Pred>(pred)) { }

            // Not copyable/movable, stored in a shared_ptr
            join_info(const join_info&) = delete;
            join_info& operator=(const join_info&) = delete;

            const result_v& get_results() {
                if (!init_called_) {
                    init();
                }
                return results_;
            }
        };
        using join_info_sp = std::shared_ptr<join_info>;

        join_info_sp spjoin_info_;                  // Information about joined elements and results
        typename result_v::const_iterator icurr_{}; // Iterator pointing at current result.
        typename result_v::const_iterator iendr_{}; // Iterator pointing at end of result vector.
        bool init_called_ = false;                  // Whether icurr_/iendr_ have been initialized.

        void init() {
            const auto& results = spjoin_info_->get_results();
            icurr_ = std::begin(results);
            iendr_ = std::end(results);
            init_called_ = true;
        }

    public:
        next_impl(OuterSeq&& outer_seq, InnerSeq&& inner_seq,
                  OuterKeySelector&& outer_key_sel, InnerKeySelector&& inner_key_sel,
                  ResultSelector&& result_sel, Pred&& pred)
            : spjoin_info_(std::make_shared<join_info>(std::forward<OuterSeq>(outer_seq),
                                                       std::forward<InnerSeq>(inner_seq),
                                                       std::forward<OuterKeySelector>(outer_key_sel),
                                                       std::forward<InnerKeySelector>(inner_key_sel),
                                                       std::forward<ResultSelector>(result_sel),
                                                       std::forward<Pred>(pred))) { }

        auto operator()() -> typename seq_traits<result_v>::const_pointer {
            // Init iterators on first call
            if (!init_called_) {
                init();
            }
            typename seq_traits<result_v>::const_pointer pobj = nullptr;
            if (icurr_ != iendr_) {
                typename seq_traits<result_v>::const_reference robj = *icurr_;
                pobj = std::addressof(robj);
                ++icurr_;
            }
            return pobj;
        }
    };

private:
    InnerSeq inner_seq_;                // Inner sequence to join.
    OuterKeySelector outer_key_sel_;    // Fetches keys for elements of outer sequence.
    InnerKeySelector inner_key_sel_;    // Fetches keys for elements of inner sequence.
    ResultSelector result_sel_;         // Creates results from joined elements.
    Pred pred_;                         // Predicate to compare keys.

public:
    join_impl(InnerSeq&& inner_seq,
              OuterKeySelector&& outer_key_sel,
              InnerKeySelector&& inner_key_sel,
              ResultSelector&& result_sel,
              Pred&& pred)
        : inner_seq_(std::forward<InnerSeq>(inner_seq)),
          outer_key_sel_(std::forward<OuterKeySelector>(outer_key_sel)),
          inner_key_sel_(std::forward<InnerKeySelector>(inner_key_sel)),
          result_sel_(std::forward<ResultSelector>(result_sel)),
          pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    join_impl(const join_impl&) = delete;
    join_impl(join_impl&&) = default;
    join_impl& operator=(const join_impl&) = delete;
    join_impl& operator=(join_impl&&) = default;

    template<typename OuterSeq>
    auto operator()(OuterSeq&& outer_seq)
        -> enumerable<typename seq_traits<typename next_impl<OuterSeq>::result_v>::const_value_type>
    {
        return next_impl<OuterSeq>(std::forward<OuterSeq>(outer_seq),
                                   std::forward<InnerSeq>(inner_seq_),
                                   std::forward<OuterKeySelector>(outer_key_sel_),
                                   std::forward<InnerKeySelector>(inner_key_sel_),
                                   std::forward<ResultSelector>(result_sel_),
                                   std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::last()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::last()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::last()
 */
template<typename = void>
class last_impl_0
{
private:
    // If we have bidi iterators, we can simply use rbegin
    template<typename Seq>
    auto impl(Seq&& seq, std::bidirectional_iterator_tag) -> decltype(*std::begin(seq)) {
        auto ricur = seq.rbegin();
        if (ricur == seq.rend()) {
            throw_linq_empty_sequence();
        }
        return *ricur;
    }

    // Otherwise we'll have to be creative
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag) -> decltype(*std::begin(seq)) {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (icur == iend) {
            throw_linq_empty_sequence();
        }
        decltype(icur) iprev;
        while (icur != iend) {
            iprev = icur;
            ++icur;
        }
        return *iprev;
    }

public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::last()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::last()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate to satisfy.
 * @see coveo::linq::last()
 */
template<typename Pred>
class last_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy

private:
    // If we have bidi iterators, we can simply use rbegin
    template<typename Seq>
    auto impl(Seq&& seq, std::bidirectional_iterator_tag) -> decltype(*std::begin(seq)) {
        auto ricur = seq.rbegin();
        auto riend = seq.rend();
        if (ricur == riend) {
            throw_linq_empty_sequence();
        }
        auto rifound = std::find_if(ricur, riend, pred_);
        if (rifound == riend) {
            throw_linq_out_of_range();
        }
        return *rifound;
    }

    // Otherwise we'll have to be creative
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag) -> decltype(*std::begin(seq)) {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (icur == iend) {
            throw_linq_empty_sequence();
        }
        auto ifound = iend;
        while (icur != iend) {
            if (pred_(*icur)) {
                ifound = icur;
            }
            ++icur;
        }
        if (ifound == iend) {
            throw_linq_out_of_range();
        }
        return *ifound;
    }

public:
    explicit last_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::last_or_default()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::last_or_default()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::last_or_default()
 */
template<typename = void>
class last_or_default_impl_0
{
private:
    // If we have bidi iterators, we can simply use rbegin
    template<typename Seq>
    auto impl(Seq&& seq, std::bidirectional_iterator_tag) -> typename seq_traits<Seq>::raw_value_type {
        auto ricur = seq.rbegin();
        return ricur != seq.rend() ? *ricur
                                   : typename seq_traits<Seq>::raw_value_type();
    }

    // Otherwise we'll have to be creative
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag) -> typename seq_traits<Seq>::raw_value_type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        auto iprev = iend;
        while (icur != iend) {
            iprev = icur;
            ++icur;
        }
        return iprev != iend ? *iprev
                             : typename seq_traits<Seq>::raw_value_type();
    }

public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::last_or_default()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::last_or_default()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate to satisfy.
 * @see coveo::linq::last_or_default()
 */
template<typename Pred>
class last_or_default_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy

private:
    // If we have bidi iterators, we can simply use rbegin
    template<typename Seq>
    auto impl(Seq&& seq, std::bidirectional_iterator_tag) -> typename seq_traits<Seq>::raw_value_type {
        auto riend = seq.rend();
        auto rifound = std::find_if(seq.rbegin(), riend, pred_);
        return rifound != riend ? *rifound
                                : typename seq_traits<Seq>::raw_value_type();
    }

    // Otherwise we'll have to be creative
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag) -> typename seq_traits<Seq>::raw_value_type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        auto ifound = iend;
        while (icur != iend) {
            if (pred_(*icur)) {
                ifound = icur;
            }
            ++icur;
        }
        return ifound != iend ? *ifound
                              : typename seq_traits<Seq>::raw_value_type();
    }

public:
    explicit last_or_default_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::max()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::max()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::max()
 */
template<typename = void>
class max_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        auto iend = std::end(seq);
        auto imax = std::max_element(std::begin(seq), iend);
        if (imax == iend) {
            throw_linq_empty_sequence();
        }
        return *imax;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::max()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::max()</tt> LINQ operator.
 * Version with value selector.
 *
 * @tparam Selector Selector used to fetch values from sequence elements.
 * @see coveo::linq::max()
 */
template<typename Selector>
class max_impl_1
{
private:
    const Selector& sel_;

public:
    explicit max_impl_1(const Selector& sel)
        : sel_(sel) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename std::decay<decltype(sel_(*std::begin(seq)))>::type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (icur == iend) {
            throw_linq_empty_sequence();
        }
        auto max_val = sel_(*icur);
        while (++icur != iend) {
            max_val = std::max(max_val, sel_(*icur));
        }
        return max_val;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::min()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::min()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::min()
 */
template<typename = void>
class min_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        auto iend = std::end(seq);
        auto imin = std::min_element(std::begin(seq), iend);
        if (imin == iend) {
            throw_linq_empty_sequence();
        }
        return *imin;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::min()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::min()</tt> LINQ operator.
 * Version with value selector.
 *
 * @tparam Selector Selector used to fetch values from sequence elements.
 * @see coveo::linq::min()
 */
template<typename Selector>
class min_impl_1
{
private:
    const Selector& sel_;   // Selector used to fetch values from sequence elements.

public:
    explicit min_impl_1(const Selector& sel)
        : sel_(sel) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename std::decay<decltype(sel_(*std::begin(seq)))>::type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (icur == iend) {
            throw_linq_empty_sequence();
        }
        auto min_val = sel_(*icur);
        while (++icur != iend) {
            min_val = std::min(min_val, sel_(*icur));
        }
        return min_val;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::none()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::none()</tt> LINQ operator.
 *
 * @see coveo::linq::none()
 */
template<typename Pred>
class none_impl
{
private:
    const Pred& pred_;  // Predicate to satisfy.

public:
    explicit none_impl(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> bool {
        return std::none_of(std::begin(seq), std::end(seq), pred_);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::order_by()</tt> internal comparator.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Comparator of sequence elements used by <tt>coveo::linq::order_by()</tt> et al.
 * Fetches keys from sequence elements then compares them with a predicate.
 * Its <tt>operator()()</tt> returns an @c int that represents the relationship
 * between the elements (negative values mean left is before right, etc.)
 *
 * @tparam KeySelector Selector used to fetch keys from sequence elements.
 * @tparam Pred Predicate used to compare the keys.
 * @tparam Descending Whether we should order ascending or descending.
 * @tparam _LessValue Value to use to compute ordering. Set automatically
 *                    from the value of @c Descending.
 * @see coveo::linq::order_by()
 * @see coveo::linq::order_by_descending()
 * @see coveo::linq::then_by()
 * @see coveo::linq::then_by_descending()
 * @see coveo::linq::detail::dual_order_by_comparator
 * @see coveo::linq::detail::order_by_impl
 * @see coveo::linq::detail::order_by_impl_with_seq
 */
template<typename KeySelector,
         typename Pred,
         bool Descending,
         int _LessValue = Descending ? 1 : -1>
class order_by_comparator
{
private:
    KeySelector key_sel_;   // Key selector used to fetch keys from elements.
    Pred pred_;             // Predicate used to compare keys.

public:
    order_by_comparator(KeySelector&& key_sel, Pred&& pred)
        : key_sel_(std::forward<KeySelector>(key_sel)), pred_(std::forward<Pred>(pred)) { }

    // Cannot copy/move, stored in a unique_ptr
    order_by_comparator(const order_by_comparator&) = delete;
    order_by_comparator& operator=(const order_by_comparator&) = delete;

    // Compares two values, returning relative position of the two in an ordered sequence.
    template<typename T1, typename T2>
    int operator()(T1&& left, T2&& right) const {
        decltype(key_sel_(left)) leftk = key_sel_(left);
        decltype(key_sel_(right)) rightk = key_sel_(right);
        int cmp;
        if (pred_(leftk, rightk)) {
            cmp = _LessValue;
        } else if (pred_(rightk, leftk)) {
            cmp = -_LessValue;
        } else {
            // Keys are equal.
            cmp = 0;
        }
        return cmp;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::order_by()</tt> internal dual comparator.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Dual comparator used by <tt>coveo::linq::order_by()</tt> et al. Chains
 * two comparators together: when <tt>operator()()</tt> is called, the
 * first comparator is used; if it returns 0, the second one is used.
 *
 * @tparam Cmp1 First comparator to chain.
 * @tparam Cmp2 Second comparator to chain.
 * @see coveo::linq::order_by()
 * @see coveo::linq::order_by_descending()
 * @see coveo::linq::then_by()
 * @see coveo::linq::then_by_descending()
 * @see coveo::linq::detail::order_by_comparator
 * @see coveo::linq::detail::order_by_impl
 * @see coveo::linq::detail::order_by_impl_with_seq
 */
template<typename Cmp1, typename Cmp2>
class dual_order_by_comparator
{
private:
    std::unique_ptr<Cmp1> upcmp1_;
    std::unique_ptr<Cmp2> upcmp2_;

public:
    dual_order_by_comparator(std::unique_ptr<Cmp1>&& upcmp1, std::unique_ptr<Cmp2>&& upcmp2)
        : upcmp1_(std::move(upcmp1)), upcmp2_(std::move(upcmp2)) { }

    // Cannot copy/move, stored in a unique_ptr
    dual_order_by_comparator(const dual_order_by_comparator&) = delete;
    dual_order_by_comparator& operator=(const dual_order_by_comparator&) = delete;

    // Compares two values by calling first then second comparator.
    template<typename T1, typename T2>
    int operator()(T1&& left, T2&& right) const {
        int cmp = (*upcmp1_)(left, right);
        if (cmp == 0) {
            cmp = (*upcmp2_)(left, right);
        }
        return cmp;
    }
};

/// @cond

// Forward declaration to declare friendship
template<typename Cmp> class order_by_impl;

/// @endcond

/**
 * @internal
 * @brief <tt>coveo::linq::order_by()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * "Second" implementation of the <tt>coveo::linq::order_by()</tt>,
 * <tt>coveo::linq::order_by_descending()</tt>, <tt>coveo::linq::then_by()</tt>
 * and <tt>coveo::linq::then_by_descending()</tt> LINQ operators.
 * This is the implementation that has both a comparator and a sequence.
 * It is the final form when the operator is fully applied.
 *
 * Contrarily to all other LINQ operation implementations, this
 * implementation cannot return a <tt>coveo::enumerable</tt>, because
 * we need to keep our identity to allow chaining. Thus, we implement
 * an @c enumerable "interface" manually by fronting the same methods.
 *
 * @tparam Seq Sequence to order.
 * @tparam Cmp Comparator used to order sequence elements.
 * @see coveo::linq::order_by()
 * @see coveo::linq::order_by_descending()
 * @see coveo::linq::then_by()
 * @see coveo::linq::then_by_descending()
 * @see coveo::linq::detail::order_by_impl
 */
template<typename Seq, typename Cmp>
class order_by_impl_with_seq
{
    // We need this friendship so that it can "steal" our internals.
    template<typename> friend class order_by_impl;

private:
    Seq seq_;                                                   // Sequence we're ordering.
    std::unique_ptr<Cmp> upcmp_;                                // Comparator used to order a sequence.
    enumerable<typename seq_traits<Seq>::value_type> enum_;     // Enumerator of ordered elements.
    bool init_called_ = false;                                  // Whether enum_ has been initialized.

    // Called to initialize enum_ before using it.
    void init() {
        std::vector<typename seq_traits<Seq>::pointer> ordered;
        try_reserve(ordered, seq_);
        for (auto&& elem : seq_) {
            typename seq_traits<Seq>::reference robj = elem;
            ordered.push_back(std::addressof(robj));
        }
        std::stable_sort(ordered.begin(),
                         ordered.end(),
                         [this](typename seq_traits<Seq>::pointer pleft,
                                typename seq_traits<Seq>::pointer pright) {
            return (*upcmp_)(*pleft, *pright) < 0;
        });
        const std::size_t num_ordered = ordered.size();
        enum_ = { make_deref_next_impl(std::move(ordered)),
                  [num_ordered]() -> std::size_t { return num_ordered; } };
        init_called_ = true;
    }

public:
    // Type of iterators used for the ordered sequence.
    using iterator          = typename enumerable<typename seq_traits<Seq>::value_type>::iterator;
    using const_iterator    = typename enumerable<typename seq_traits<Seq>::value_type>::const_iterator;

    // Constructor called by the impl without sequence.
    order_by_impl_with_seq(Seq&& seq, std::unique_ptr<Cmp>&& upcmp)
        : seq_(std::forward<Seq>(seq)), upcmp_(std::move(upcmp)) { }

    // Movable but not copyable
    order_by_impl_with_seq(const order_by_impl_with_seq&) = delete;
    order_by_impl_with_seq(order_by_impl_with_seq&&) = default;
    order_by_impl_with_seq& operator=(const order_by_impl_with_seq&) = delete;
    order_by_impl_with_seq& operator=(order_by_impl_with_seq&&) = default;

    // Support for ordered sequence.
    iterator begin() const {
        if (!init_called_) {
            const_cast<order_by_impl_with_seq<Seq, Cmp>*>(this)->init();
        }
        return enum_.begin();
    }
    iterator end() const {
        if (!init_called_) {
            const_cast<order_by_impl_with_seq<Seq, Cmp>*>(this)->init();
        }
        return enum_.end();
    }
    iterator cbegin() const {
        return begin();
    }
    iterator cend() const {
        return end();
    }

    // Support for sequence size (a bit like the enumerable API)
    bool has_fast_size() const {
        if (!init_called_) {
            const_cast<order_by_impl_with_seq<Seq, Cmp>*>(this)->init();
        }
        return enum_.has_fast_size();
    }
    std::size_t size() const {
        if (!init_called_) {
            const_cast<order_by_impl_with_seq<Seq, Cmp>*>(this)->init();
        }
        return enum_.size();
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::order_by()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * "First" implementation of the <tt>coveo::linq::order_by()</tt>,
 * <tt>coveo::linq::order_by_descending()</tt>, <tt>coveo::linq::then_by()</tt>
 * and <tt>coveo::linq::then_by_descending()</tt> LINQ operators.
 * This is the implementation with a comparator only; it is returned by
 * the operator helper methods. It supports two operations: either being
 * applied to a sequence, in which case it produces the "second" implementation
 * (see <tt>coveo::linq::detail::order_by_impl_with_seq</tt>), or being
 * applied to an @c order_by_impl_with_seq, in which case we chain the
 * comparators.
 *
 * @tparam Cmp Comparator used to order sequence elements.
 * @see coveo::linq::order_by()
 * @see coveo::linq::order_by_descending()
 * @see coveo::linq::then_by()
 * @see coveo::linq::then_by_descending()
 * @see coveo::linq::detail::order_by_impl_with_seq
 * @see coveo::linq::detail::order_by_comparator
 * @see coveo::linq::detail::dual_order_by_comparator
 */
template<typename Cmp>
class order_by_impl
{
private:
    std::unique_ptr<Cmp> upcmp_;    // Comparator used to order a sequence.

public:
    explicit order_by_impl(std::unique_ptr<Cmp>&& upcmp)
        : upcmp_(std::move(upcmp)) { }

    // Movable by not copyable
    order_by_impl(const order_by_impl&) = delete;
    order_by_impl(order_by_impl&&) = default;
    order_by_impl& operator=(const order_by_impl&) = delete;
    order_by_impl& operator=(order_by_impl&&) = default;

    // When applied to a sequence, produces a different object.
    template<typename Seq>
    auto operator()(Seq&& seq) -> order_by_impl_with_seq<Seq, Cmp> {
        return order_by_impl_with_seq<Seq, Cmp>(std::forward<Seq>(seq), std::move(upcmp_));
    }

    // When applied to an impl with sequence, merges the two and chains the comparators.
    template<typename ImplSeq, typename ImplCmp>
    auto operator()(order_by_impl_with_seq<ImplSeq, ImplCmp>&& impl)
        -> order_by_impl_with_seq<ImplSeq, dual_order_by_comparator<ImplCmp, Cmp>>
    {
        using dual_comparator = dual_order_by_comparator<ImplCmp, Cmp>;
        auto new_upcmp = detail::make_unique<dual_comparator>(std::move(impl.upcmp_), std::move(upcmp_));
        return order_by_impl_with_seq<ImplSeq, dual_comparator>(std::forward<ImplSeq>(impl.seq_), std::move(new_upcmp));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::reverse()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::reverse()</tt> LINQ operator.
 *
 * @see coveo::linq::reverse()
 */
template<typename = void>
class reverse_impl
{
private:
    // next impl when we can use reverse iterators
    template<typename Seq>
    class next_impl_fast
    {
    private:
        // Iterator used to go backward in sequence.
        using reverse_iterator = typename std::decay<decltype(std::declval<Seq>().rbegin())>::type;

        // Bean containing info shared among delegates
        struct reverse_info {
            Seq seq_;                   // Sequence we're iterating.
            reverse_iterator irend_;    // End of reversed seq_.

            explicit reverse_info(Seq&& seq)
                : seq_(std::forward<Seq>(seq)),
                  irend_(seq_.rend()) { }

            // Cannot copy/move, stored in a shared_ptr.
            reverse_info(const reverse_info&) = delete;
            reverse_info& operator=(const reverse_info&) = delete;
        };
        using reverse_info_sp = std::shared_ptr<reverse_info>;

        reverse_info_sp spinfo_;    // Shared info with sequence.
        reverse_iterator ircur_;    // Current point in reverse sequence.

    public:
        explicit next_impl_fast(Seq&& seq)
            : spinfo_(std::make_shared<reverse_info>(std::forward<Seq>(seq))),
              ircur_(spinfo_->seq_.rbegin()) { }

        auto operator()() -> typename seq_traits<Seq>::pointer {
            typename seq_traits<Seq>::pointer pobj = nullptr;
            if (ircur_ != spinfo_->irend_) {
                typename seq_traits<Seq>::reference robj = *ircur_;
                pobj = std::addressof(robj);
                ++ircur_;
            }
            return pobj;
        }
    };

    // next impl when we don't have reverse iterators natively
    template<typename Seq>
    class next_impl_slow
    {
    private:
        // Vector storing pointers of elements from sequence.
        using pelems_v                  = std::vector<typename seq_traits<Seq>::pointer>;
        using pelems_v_reverse_iterator = typename pelems_v::reverse_iterator;

        // Bean containing info shared among delegates
        struct reverse_info {
            Seq seq_;                           // Sequence containing the elements.
            pelems_v vpelems_;                  // Vector of pointers to the elements.
            pelems_v_reverse_iterator virend_;  // Iterator pointing at end of vpelems_.

            explicit reverse_info(Seq&& seq)
                : seq_(std::forward<Seq>(seq))
            {
                // Build vector of pointers to sequence elements.
                try_reserve(vpelems_, seq_);
                for (auto&& elem : seq_) {
                    typename seq_traits<Seq>::reference robj = elem;
                    vpelems_.push_back(std::addressof(robj));
                }

                // Init end iterator.
                virend_ = vpelems_.rend();
            }

            // Cannot copy/move, stored in a shared_ptr.
            reverse_info(const reverse_info&) = delete;
            reverse_info& operator=(const reverse_info&) = delete;
        };
        using reverse_info_sp = std::shared_ptr<reverse_info>;

        reverse_info_sp spinfo_;            // Bean storing sequence and vector of pointers.
        pelems_v_reverse_iterator vircur_;  // Iterator pointing at current pointer to element.

    public:
        explicit next_impl_slow(Seq&& seq)
            : spinfo_(std::make_shared<reverse_info>(std::forward<Seq>(seq))),
              vircur_(spinfo_->vpelems_.rbegin()) { }

        auto operator()() -> typename seq_traits<Seq>::pointer {
            typename seq_traits<Seq>::pointer pobj = nullptr;
            if (vircur_ != spinfo_->virend_) {
                pobj = *vircur_;
                ++vircur_;
            }
            return pobj;
        }

        // To be able to build a proper size delegate
        std::size_t size() const {
            return spinfo_->vpelems_.size();
        }
    };

    // If we have bidi iterators, we can simply use rbegin
    template<typename Seq>
    auto impl(Seq&& seq, std::bidirectional_iterator_tag)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        auto siz = try_get_size_delegate(seq);
        return enumerable<typename seq_traits<Seq>::value_type>(next_impl_fast<Seq>(std::forward<Seq>(seq)),
                                                                siz);
    }

    // Otherwise we'll have to be creative
    template<typename Seq>
    auto impl(Seq&& seq, std::input_iterator_tag)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        next_impl_slow<Seq> next_impl(std::forward<Seq>(seq));
        const std::size_t size = next_impl.size();
        auto siz = [size]() -> std::size_t { return size; };
        return enumerable<typename seq_traits<Seq>::value_type>(std::move(next_impl),
                                                                siz);
    }

public:
    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        return impl(std::forward<Seq>(seq),
                    typename std::iterator_traits<typename seq_traits<Seq>::iterator_type>::iterator_category());
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::select()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::select()</tt> and
 * <tt>coveo::linq::select_with_index()</tt> LINQ operators.
 *
 * @tparam Selector Selector to transform sequence elements.
 * @see coveo::linq::select()
 * @see coveo::linq::select_with_index()
 */
template<typename Selector>
class select_impl
{
public:
    // Next delegate implementation for select operator
    template<typename Seq, typename CU, typename RU>
    class next_impl
    {
    private:
        // Iterator used by the sequence.
        using iterator_type             = typename seq_traits<Seq>::iterator_type;

        // Containers storing transformed elements.
        using transformed_v             = std::vector<RU>;
        using transformed_l             = std::forward_list<RU>;
        using transformed_l_iterator    = typename transformed_l::iterator;

        // Bean storing info about elements. Shared among delegates.
        struct select_info {
            Seq seq_;                       // Sequence being transformed.
            iterator_type icur_;            // Iterator pointing at current element in seq_.
            iterator_type iend_;            // Iterator pointing at end of seq_.
            Selector sel_;                  // Selector transforming the elements.
            transformed_v vtransformed_;    // Vector of transformed elements.
            transformed_l ltransformed_;    // List of transformed elements.
            transformed_l_iterator llast_;  // Iterator pointing to last element in ltransformed_ (before end()).
            std::size_t lsize_;             // Number of elements in ltransformed_.
            bool use_vector_;               // Whether we use ltransformed_ (false) or vtransformed_ (true).

            select_info(Seq&& seq, Selector&& sel)
                : seq_(std::forward<Seq>(seq)),
                  icur_(std::begin(seq_)),
                  iend_(std::end(seq_)),
                  sel_(std::forward<Selector>(sel)),
                  vtransformed_(),
                  ltransformed_(),
                  llast_(ltransformed_.before_begin()),
                  lsize_(0),
                  use_vector_(try_reserve(vtransformed_, seq_)) { }

            // Cannot copy/move, stored in a shared_ptr
            select_info(const select_info&) = delete;
            select_info& operator=(const select_info&) = delete;

            auto get_next_in_vector(std::size_t& vncur) -> CU* {
                while (vtransformed_.size() <= vncur && icur_ != iend_) {
                    vtransformed_.emplace_back(sel_(*icur_, vtransformed_.size()));
                    ++icur_;
                }
                return vtransformed_.size() > vncur ? std::addressof(vtransformed_[vncur++])
                                                    : nullptr;
            }

            auto get_next_in_list(transformed_l_iterator& licur) -> CU* {
                while (licur == llast_ && icur_ != iend_) {
                    llast_ = ltransformed_.emplace_after(llast_, sel_(*icur_, lsize_++));
                    ++icur_;
                }
                return licur != llast_ ? std::addressof(*++licur)
                                       : nullptr;
            }

            auto get_next(std::size_t& vncur, transformed_l_iterator& licur) -> CU* {
                return use_vector_ ? get_next_in_vector(vncur)
                                   : get_next_in_list(licur);
            }
        };
        using select_info_sp = std::shared_ptr<select_info>;

        select_info_sp spinfo_;         // Shared information about elements.
        std::size_t vncur_;             // Index of current element (if in vector).
        transformed_l_iterator licur_;  // Iterator pointing at current element (if in list).

    public:
        next_impl(Seq&& seq, Selector&& sel)
            : spinfo_(std::make_shared<select_info>(std::forward<Seq>(seq),
                                                    std::forward<Selector>(sel))),
              vncur_(0),
              licur_(spinfo_->ltransformed_.before_begin()) { }

        auto operator()() -> CU* {
            return spinfo_->get_next(vncur_, licur_);
        }
    };

private:
    Selector sel_;  // Selector used to transform elements.

public:
    explicit select_impl(Selector&& sel)
        : sel_(std::forward<Selector>(sel)) { }

    template<typename Seq,
             typename _SelectorRes = decltype(std::declval<Selector>()(std::declval<typename seq_traits<Seq>::reference>(), std::declval<std::size_t>())),
             typename _CU = typename seq_element_traits<_SelectorRes>::const_value_type,
             typename _RU = typename seq_element_traits<_SelectorRes>::raw_value_type>
    auto operator()(Seq&& seq) -> enumerable<_CU> {
        auto siz = try_get_size_delegate(seq);
        return enumerable<_CU>(next_impl<Seq, _CU, _RU>(std::forward<Seq>(seq), std::forward<Selector>(sel_)),
                               siz);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::select_many()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::select_many()</tt> and
 * <tt>coveo::linq::select_many_with_index</tt> LINQ operators.
 *
 * @tparam Selector Selector used to transform sequence elements.
 * @see coveo::linq::select_many()
 * @see coveo::linq::select_many_with_index()
 */
template<typename Selector>
class select_many_impl
{
public:
    // Next delegate implementation for select_many operator
    template<typename Seq, typename CU, typename RU>
    class next_impl
    {
    private:
        // Iterator used by the sequence.
        using iterator_type             = typename seq_traits<Seq>::iterator_type;

        // List storing transformed elements.
        using transformed_l             = std::forward_list<RU>;
        using transformed_l_iterator    = typename transformed_l::iterator;

        // Bean storing info about elements. Shared among delegates.
        struct select_info {
            Seq seq_;                       // Sequence being transformed.
            iterator_type icur_;            // Iterator pointing at current element in seq_.
            std::size_t idx_;               // Index of current element in seq_.
            iterator_type iend_;            // Iterator pointing at end of seq_.
            Selector sel_;                  // Selector transforming the elements.
            transformed_l ltransformed_;    // List of transformed elements.
            transformed_l_iterator llast_;  // Iterator pointing to last element in ltransformed_ (before end()).

            select_info(Seq&& seq, Selector&& sel)
                : seq_(std::forward<Seq>(seq)),
                  icur_(std::begin(seq_)),
                  idx_(0),
                  iend_(std::end(seq_)),
                  sel_(std::forward<Selector>(sel)),
                  ltransformed_(),
                  llast_(ltransformed_.before_begin()) { }

            // Cannot copy/move, stored in a shared_ptr
            select_info(const select_info&) = delete;
            select_info& operator=(const select_info&) = delete;

            auto get_next(transformed_l_iterator& licur) -> CU* {
                while (licur == llast_ && icur_ != iend_) {
                    auto new_elements = sel_(*icur_, idx_++);
                    llast_ = ltransformed_.insert_after(llast_, std::begin(new_elements), std::end(new_elements));
                    ++icur_;
                }
                return licur != llast_ ? std::addressof(*++licur)
                                       : nullptr;
            }
        };
        using select_info_sp = std::shared_ptr<select_info>;

        select_info_sp spinfo_;         // Shared information about elements.
        transformed_l_iterator licur_;  // Iterator pointing at current element.

    public:
        next_impl(Seq&& seq, Selector&& sel)
            : spinfo_(std::make_shared<select_info>(std::forward<Seq>(seq),
                                                    std::forward<Selector>(sel))),
              licur_(spinfo_->ltransformed_.before_begin()) { }

        auto operator()() -> CU* {
            return spinfo_->get_next(licur_);
        }
    };

private:
    Selector sel_;  // Selector used to transform elements.

public:
    explicit select_many_impl(Selector&& sel)
        : sel_(std::forward<Selector>(sel)) { }

    template<typename Seq,
             typename _SelectorSeqRes = decltype(std::declval<Selector>()(std::declval<typename seq_traits<Seq>::const_reference>(), std::declval<std::size_t>())),
             typename _CU = typename seq_traits<_SelectorSeqRes>::const_value_type,
             typename _RU = typename seq_traits<_SelectorSeqRes>::raw_value_type>
    auto operator()(Seq&& seq) -> enumerable<_CU> {
        return next_impl<Seq, _CU, _RU>(std::forward<Seq>(seq), std::forward<Selector>(sel_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::sequence_equal()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::sequence_equal()</tt> LINQ operator.
 * Version with one argument (second sequence).
 *
 * @tparam Seq2 Second sequence to compare. The first one will have been
 *              provided in the call to <tt>coveo::linq::from()</tt>.
 * @see coveo::linq::sequence_equal()
 */
template<typename Seq2>
class sequence_equal_impl_1
{
private:
    const Seq2& seq2_;  // Second sequence to compare.

public:
    explicit sequence_equal_impl_1(const Seq2& seq2)
        : seq2_(seq2) { }

    template<typename Seq1>
    auto operator()(Seq1&& seq1) -> bool {
        auto icur1 = std::begin(seq1);
        auto iend1 = std::end(seq1);
        auto icur2 = std::begin(seq2_);
        auto iend2 = std::end(seq2_);
        bool is_equal = true;
        for (; is_equal && icur1 != iend1 && icur2 != iend2; ++icur1, ++icur2) {
            is_equal = *icur1 == *icur2;
        }
        return is_equal && icur1 == iend1 && icur2 == iend2;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::sequence_equal()</tt> implementation (2).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::sequence_equal()</tt> LINQ operator.
 * Version with two arguments (second sequence and predicate).
 *
 * @tparam Seq2 Second sequence to compare. The first one will have been
 *              provided in the call to <tt>coveo::linq::from()</tt>.
 * @tparam Pred Predicate used to compare sequence elements.
 * @see coveo::linq::sequence_equal()
 */
template<typename Seq2, typename Pred>
class sequence_equal_impl_2
{
private:
    const Seq2& seq2_;  // Second sequence to compare.
    const Pred& pred_;  // Equality predicate.

public:
    sequence_equal_impl_2(const Seq2& seq2, const Pred& pred)
        : seq2_(seq2), pred_(pred) { }

    template<typename Seq1>
    auto operator()(Seq1&& seq1) -> bool {
        auto icur1 = std::begin(seq1);
        auto iend1 = std::end(seq1);
        auto icur2 = std::begin(seq2_);
        auto iend2 = std::end(seq2_);
        bool is_equal = true;
        for (; is_equal && icur1 != iend1 && icur2 != iend2; ++icur1, ++icur2) {
            is_equal = pred_(*icur1, *icur2);
        }
        return is_equal && icur1 == iend1 && icur2 == iend2;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::single()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::single()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::single()
 */
template<typename = void>
class single_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        auto ifirst = std::begin(seq);
        auto iend = std::end(seq);
        if (ifirst == iend) {
            throw_linq_empty_sequence();
        }
        auto inext = ifirst;
        ++inext;
        if (inext != iend) {
            throw_linq_out_of_range();
        }
        return *ifirst;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::single()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::single()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Predicate to satisfy.
 * @see coveo::linq::single()
 */
template<typename Pred>
class single_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy.

public:
    explicit single_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> decltype(*std::begin(seq)) {
        auto ibeg = std::begin(seq);
        auto iend = std::end(seq);
        if (ibeg == iend) {
            throw_linq_empty_sequence();
        }
        auto ifound = std::find_if(ibeg, iend, pred_);
        if (ifound == iend) {
            throw_linq_out_of_range();
        }
        auto inext = ifound;
        ++inext;
        auto ifoundagain = std::find_if(inext, iend, pred_);
        if (ifoundagain != iend) {
            throw_linq_out_of_range();
        }
        return *ifound;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::single_or_default()</tt> implementation (0).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::single_or_default()</tt> LINQ operator.
 * Version without argument.
 *
 * @see coveo::linq::single_or_default()
 */
template<typename = void>
class single_or_default_impl_0
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        auto icur = std::begin(seq);
        auto iend = std::end(seq);
        if (icur != iend) {
            auto inext = icur;
            ++inext;
            if (inext != iend) {
                icur = iend;
            }
        }
        return icur != iend ? *icur
                            : typename seq_traits<Seq>::raw_value_type();
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::single_or_default()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::single_or_default()</tt> LINQ operator.
 * Version with predicate.
 *
 * @tparam Pred Predicate to satisfy.
 * @see coveo::linq::single_or_default()
 */
template<typename Pred>
class single_or_default_impl_1
{
private:
    const Pred& pred_;  // Predicate to satisfy.

public:
    explicit single_or_default_impl_1(const Pred& pred)
        : pred_(pred) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename seq_traits<Seq>::raw_value_type {
        auto iend = std::end(seq);
        auto ifound = std::find_if(std::begin(seq), iend, pred_);
        if (ifound != iend) {
            auto inext = ifound;
            ++inext;
            auto ifoundagain = std::find_if(inext, iend, pred_);
            if (ifoundagain != iend) {
                ifound = iend;
            }
        }
        return ifound != iend ? *ifound
                              : typename seq_traits<Seq>::raw_value_type();
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::skip()</tt> internal predicate.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Predicate used by the implementation of the <tt>coveo::linq::skip()</tt>
 * LINQ operator. Simply skips N elements.
 *
 * @see coveo::linq::skip()
 * @see coveo::linq::detail::skip_impl
 */
template<typename = void>
class skip_n_pred
{
private:
    std::size_t n_; // Number of elements to skip.

public:
    explicit skip_n_pred(std::size_t n)
        : n_(n) { }

    template<typename T>
    auto operator()(T&&, std::size_t idx) -> bool {
        return idx < n_;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::skip()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::skip()</tt>,
 * <tt>coveo::linq::skip_while()</tt> and
 * <tt>coveo::linq::skip_while_with_index()</tt> LINQ operators.
 *
 * @tparam Pred Predicate used to skip elements.
 * @see coveo::linq::skip()
 * @see coveo::linq::skip_while()
 * @see coveo::linq::skip_while_with_index()
 */
template<typename Pred>
class skip_impl
{
public:
    // Next delegate implementation for skip operators
    template<typename Seq>
    class next_impl
    {
    private:
        // Iterator for the sequence type.
        using iterator_type = typename seq_traits<Seq>::iterator_type;

        // Bean containing info to share among delegates
        struct skip_info {
            Seq seq_;               // Sequence to skip elements from
            iterator_type iend_;    // Iterator pointing at end of sequence
            Pred pred_;             // Predicate to satisfy to skip elements

            skip_info(Seq&& seq, Pred&& pred)
                : seq_(std::forward<Seq>(seq)),
                  iend_(std::end(seq_)),
                  pred_(std::forward<Pred>(pred)) { }

            // Cannot copy/move, stored in a shared_ptr
            skip_info(const skip_info&) = delete;
            skip_info& operator=(const skip_info&) = delete;
        };
        using skip_info_sp = std::shared_ptr<skip_info>;

        skip_info_sp spinfo_;       // Pointer to shared info
        iterator_type icur_;        // Iterator pointing at current element
        bool init_called_ = false;  // Whether icur_ has been initialized

        void init() {
            icur_ = std::begin(spinfo_->seq_);
            std::size_t n = 0;
            while (icur_ != spinfo_->iend_ && spinfo_->pred_(*icur_, n++)) {
                ++icur_;
            }
            init_called_ = true;
        }

    public:
        next_impl(Seq&& seq, Pred&& pred)
            : spinfo_(std::make_shared<skip_info>(std::forward<Seq>(seq),
                                                  std::forward<Pred>(pred))) { }

        auto operator()() -> typename seq_traits<Seq>::pointer {
            // Init starting point on first call
            if (!init_called_) {
                init();
            }
            typename seq_traits<Seq>::pointer pobj = nullptr;
            if (icur_ != spinfo_->iend_) {
                typename seq_traits<Seq>::reference robj = *icur_;
                pobj = std::addressof(robj);
                ++icur_;
            }
            return pobj;
        }
    };

private:
    Pred pred_;     // Predicate to satisfy to skip.
    std::size_t n_; // How many items to skip, if known (otherwise -1).

public:
    skip_impl(Pred&& pred, std::size_t n)
        : pred_(std::forward<Pred>(pred)),
          n_(n) { }

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        typename enumerable<typename seq_traits<Seq>::value_type>::size_delegate siz;
        if (n_ != static_cast<std::size_t>(-1)) {
            auto seq_siz = try_get_size_delegate(seq);
            if (seq_siz != nullptr) {
                const std::size_t seq_size = seq_siz();
                const std::size_t size = seq_size > n_ ? seq_size - n_ : 0;
                siz = [size]() -> std::size_t { return size; };
            }
        }
        return enumerable<typename seq_traits<Seq>::value_type>(next_impl<Seq>(std::forward<Seq>(seq),
                                                                               std::forward<Pred>(pred_)),
                                                                siz);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::sum()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::sum()</tt> LINQ operator.
 *
 * @tparam F Function to fetch values from sequence elements.
 * @see coveo::linq::sum()
 */
template<typename F>
class sum_impl
{
private:
    const F& num_f_;

public:
    explicit sum_impl(const F& num_f)
        : num_f_(num_f) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> typename std::decay<decltype(num_f_(*std::begin(seq)))>::type {
        auto it = std::begin(seq);
        auto end = std::end(seq);
        if (it == end) {
            throw_linq_empty_sequence();
        }
        auto total = num_f_(*it);
        for (++it; it != end; ++it) {
            total += num_f_(*it);
        }
        return total;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::take()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::take()</tt>,
 * <tt>coveo::linq::take_while()</tt> and
 * <tt>coveo::linq::take_while_with_index()</tt> LINQ operators.
 *
 * @tparam Pred Predicate used to take elements.
 * @see coveo::linq::take()
 * @see coveo::linq::take_while()
 * @see coveo::linq::take_while_with_index()
 */
template<typename Pred>
class take_impl
{
public:
    // Next delegate implementation for take operators
    template<typename Seq>
    class next_impl
    {
    private:
        // Iterator for the sequence type.
        using iterator_type = typename seq_traits<Seq>::iterator_type;

        // Bean containing info to share among delegates
        struct take_info {
            Seq seq_;               // Sequence to take elements from
            iterator_type iend_;    // Iterator pointing at end of sequence
            Pred pred_;             // Predicate to satisfy to take elements

            take_info(Seq&& seq, Pred&& pred)
                : seq_(std::forward<Seq>(seq)),
                  iend_(std::end(seq_)),
                  pred_(std::forward<Pred>(pred)) { }

            // Cannot copy/move, stored in a shared_ptr
            take_info(const take_info&) = delete;
            take_info& operator=(const take_info&) = delete;
        };
        using take_info_sp = std::shared_ptr<take_info>;

        take_info_sp spinfo_;       // Pointer to shared info
        iterator_type icur_;        // Iterator pointing at current element
        std::size_t n_ = 0;         // Index of current element
        bool done_ = false;         // Whether we're done taking elements

    public:
        next_impl(Seq&& seq, Pred&& pred)
            : spinfo_(std::make_shared<take_info>(std::forward<Seq>(seq),
                                                  std::forward<Pred>(pred))),
              icur_(std::begin(spinfo_->seq_)) { }

        auto operator()() -> typename seq_traits<Seq>::pointer {
            typename seq_traits<Seq>::pointer pobj = nullptr;
            if (!done_) {
                if (icur_ != spinfo_->iend_ && spinfo_->pred_(*icur_, n_++)) {
                    typename seq_traits<Seq>::reference robj = *icur_;
                    pobj = std::addressof(robj);
                    ++icur_;
                } else {
                    done_ = true;
                }
            }
            return pobj;
        }
    };

private:
    Pred pred_;     // Predicate to satisfy to skip.
    std::size_t n_; // How many items to take, if known (otherwise -1).

public:
    take_impl(Pred&& pred, std::size_t n)
        : pred_(std::forward<Pred>(pred)),
          n_(n) { }

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        typename enumerable<typename seq_traits<Seq>::value_type>::size_delegate siz;
        if (n_ != static_cast<std::size_t>(-1)) {
            auto seq_siz = try_get_size_delegate(seq);
            if (seq_siz != nullptr) {
                const std::size_t size = std::min(n_, seq_siz());
                siz = [size]() -> std::size_t { return size; };
            }
        }
        return enumerable<typename seq_traits<Seq>::value_type>(next_impl<Seq>(std::forward<Seq>(seq),
                                                                               std::forward<Pred>(pred_)),
                                                                siz);
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::to()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::to()</tt> LINQ operator.
 *
 * @tparam Container Type of container to return.
 * @see coveo::linq::to()
 */
template<typename Container>
class to_impl
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> Container {
        return Container(std::begin(std::forward<Seq>(seq)), std::end(std::forward<Seq>(seq)));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::to_vector()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::to_vector()</tt> LINQ operator.
 *
 * @see coveo::linq::to_vector()
 */
template<typename = void>
class to_vector_impl
{
public:
    template<typename Seq>
    auto operator()(Seq&& seq) -> std::vector<typename seq_traits<Seq>::raw_value_type> {
        std::vector<typename seq_traits<Seq>::raw_value_type> v;
        try_reserve(v, seq);
        v.insert(v.end(), std::begin(std::forward<Seq>(seq)), std::end(std::forward<Seq>(seq)));
        return v;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::to_associative()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::to_associative()</tt> LINQ operator.
 * Version with one argument (key selector).
 *
 * @tparam Container Type of container to return.
 * @tparam KeySelector Selector used to fetch keys from sequence elements.
 * @see coveo::linq::to_associative()
 */
template<typename Container, typename KeySelector>
class to_associative_impl_1
{
private:
    const KeySelector& key_sel_;    // Selector to fetch keys for sequence elements.

public:
    explicit to_associative_impl_1(const KeySelector& key_sel)
        : key_sel_(key_sel) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> Container {
        Container c;
        for (auto&& elem : seq) {
            auto key = key_sel_(elem);
            auto emplace_res = c.emplace(key, elem);
            if (!emplace_res.second) {
                emplace_res.first->second = elem;
            }
        }
        return c;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::to_associative()</tt> implementation (2).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::to_associative()</tt> LINQ operator.
 * Version with two arguments (key and element selectors).
 *
 * @tparam Container Type of container to return.
 * @tparam KeySelector Selector used to fetch keys from sequence elements.
 * @tparam ElementSelector Selector used to fetch values from sequence elements.
 * @see coveo::linq::to_associative()
 */
template<typename Container, typename KeySelector, typename ElementSelector>
class to_associative_impl_2
{
private:
    const KeySelector& key_sel_;        // Selector to fetch keys for sequence elements.
    const ElementSelector& elem_sel_;   // Selector to fetch values for sequence elements.

public:
    to_associative_impl_2(const KeySelector& key_sel, const ElementSelector& elem_sel)
        : key_sel_(key_sel), elem_sel_(elem_sel) { }

    template<typename Seq>
    auto operator()(Seq&& seq) -> Container {
        Container c;
        for (auto&& elem : seq) {
            auto key = key_sel_(elem);
            auto mapped = elem_sel_(elem);
            auto emplace_res = c.emplace(key, mapped);
            if (!emplace_res.second) {
                emplace_res.first->second = mapped;
            }
        }
        return c;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::to_map()</tt> implementation (1).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::to_map()</tt> LINQ operator.
 * Version with one argument (key selector).
 *
 * @tparam KeySelector Selector used to fetch keys from sequence elements.
 * @see coveo::linq::to_map()
 */
template<typename KeySelector>
class to_map_impl_1
{
private:
    const KeySelector& key_sel_;    // Selector to fetch keys for sequence elements.

public:
    explicit to_map_impl_1(const KeySelector& key_sel)
        : key_sel_(key_sel) { }

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> std::map<typename std::decay<decltype(key_sel_(*std::begin(seq)))>::type,
                    typename seq_traits<Seq>::raw_value_type>
    {
        std::map<typename std::decay<decltype(key_sel_(*std::begin(seq)))>::type,
                 typename seq_traits<Seq>::raw_value_type> m;
        for (auto&& elem : seq) {
            auto key = key_sel_(elem);
            auto emplace_res = m.emplace(key, elem);
            if (!emplace_res.second) {
                emplace_res.first->second = elem;
            }
        }
        return m;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::to_map()</tt> implementation (2).
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::to_map()</tt> LINQ operator.
 * Version with two arguments (key and element selectors).
 *
 * @tparam KeySelector Selector used to fetch keys from sequence elements.
 * @tparam ElementSelector Selector used to fetch values from sequence elements.
 * @see coveo::linq::to_map()
 */
template<typename KeySelector, typename ElementSelector>
class to_map_impl_2
{
private:
    const KeySelector& key_sel_;        // Selector to fetch keys for sequence elements.
    const ElementSelector& elem_sel_;   // Selector to fetch values for sequence elements.

public:
    to_map_impl_2(const KeySelector& key_sel, const ElementSelector& elem_sel)
        : key_sel_(key_sel), elem_sel_(elem_sel) { }

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> std::map<typename std::decay<decltype(key_sel_(*std::begin(seq)))>::type,
                    typename std::decay<decltype(elem_sel_(*std::begin(seq)))>::type>
    {
        std::map<typename std::decay<decltype(key_sel_(*std::begin(seq)))>::type,
                 typename std::decay<decltype(elem_sel_(*std::begin(seq)))>::type> m;
        for (auto&& elem : seq) {
            auto key = key_sel_(elem);
            auto mapped = elem_sel_(elem);
            auto emplace_res = m.emplace(key, mapped);
            if (!emplace_res.second) {
                emplace_res.first->second = mapped;
            }
        }
        return m;
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::union_with()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::union_with()</tt> LINQ operator.
 *
 * @tparam Seq2 Second sequence to union. The first sequence will have been
 *              provided by the call to <tt>coveo::linq::from()</tt>.
 * @see coveo::linq::union_with()
 */
template<typename Seq2, typename Pred>
class union_impl
{
public:
    // Implementation of next delegate that filters duplicate elements
    template<typename Seq1>
    class next_impl
    {
    public:
        // Type of element returned by this next delegate. The elements will be const
        // if at least one sequence is const.
        using enum_type     = typename std::conditional<std::is_const<typename seq_traits<Seq1>::value_type>::value ||
                                                        std::is_const<typename seq_traits<Seq2>::value_type>::value,
                                                        typename seq_traits<Seq1>::const_value_type,
                                                        typename seq_traits<Seq1>::value_type>::type;
        using enum_ptr_type = typename seq_element_traits<enum_type>::pointer;
        using enum_ref_type = typename seq_element_traits<enum_type>::reference;

    private:
        // Type of iterator for the sequences
        using first_iterator_type   = typename seq_traits<Seq1>::iterator_type;
        using second_iterator_type  = typename seq_traits<Seq2>::iterator_type;

        // Set storing pointers to seen elements
        using seen_elements_set     = std::set<enum_ptr_type, deref_cmp<proxy_cmp<Pred>>>;

        // Info used to produce distinct elements. Shared among delegates.
        class union_info
        {
        private:
            Seq1 seq1_;                     // First sequence being iterated
            first_iterator_type iend1_;     // Iterator pointing at end of first sequence
            Seq2 seq2_;                     // Second sequence being iterated
            second_iterator_type iend2_;    // Iterator pointing at end of second sequence
            Pred pred_;                     // Predicate ordering the elements

            template<typename It>
            auto get_next_in_seq(It& icur, const It& iend, seen_elements_set& seen) -> enum_ptr_type {
                enum_ptr_type pobj = nullptr;
                for (; pobj == nullptr && icur != iend; ++icur) {
                    enum_ref_type robjtmp = *icur;
                    auto pobjtmp = std::addressof(robjtmp);
                    if (seen.emplace(pobjtmp).second) {
                        // Not seen yet, return this element.
                        pobj = pobjtmp;
                    }
                }
                return pobj;
            }

        public:
            union_info(Seq1&& seq1, Seq2&& seq2, Pred&& pred)
                : seq1_(std::forward<Seq1>(seq1)),
                  iend1_(std::end(seq1_)),
                  seq2_(std::forward<Seq2>(seq2)),
                  iend2_(std::end(seq2_)),
                  pred_(std::forward<Pred>(pred)) { }

            // Cannot copy/move, stored in a shared_ptr
            union_info(const union_info&) = delete;
            union_info& operator=(const union_info&) = delete;

            first_iterator_type first_begin() {
                return std::begin(seq1_);
            }
            second_iterator_type second_begin() {
                return std::begin(seq2_);
            }
            seen_elements_set init_seen_elements() {
                return seen_elements_set(deref_cmp<proxy_cmp<Pred>>(proxy_cmp<Pred>(pred_)));
            }

            // Returns next distinct element in either sequence or nullptr when done
            auto get_next(first_iterator_type& icur1, second_iterator_type& icur2, seen_elements_set& seen) -> enum_ptr_type {
                // First look for an element in first sequence
                enum_ptr_type pobj = get_next_in_seq(icur1, iend1_, seen);

                // If we did not find an element in first sequence, try in second sequence
                if (pobj == nullptr) {
                    pobj = get_next_in_seq(icur2, iend2_, seen);
                }

                return pobj;
            }
        };
        using union_info_sp = std::shared_ptr<union_info>;

        union_info_sp spinfo_;          // Shared info
        first_iterator_type icur1_;     // Iterator pointing at current element in first sequence
        second_iterator_type icur2_;    // Iterator pointing at current element in second sequence
        seen_elements_set seen_;        // Set of seen elements

    public:
        next_impl(Seq1&& seq1, Seq2&& seq2, Pred&& pred)
            : spinfo_(std::make_shared<union_info>(std::forward<Seq1>(seq1),
                                                   std::forward<Seq2>(seq2),
                                                   std::forward<Pred>(pred))),
              icur1_(spinfo_->first_begin()),
              icur2_(spinfo_->second_begin()),
              seen_(spinfo_->init_seen_elements()) { }

        auto operator()() -> decltype(spinfo_->get_next(icur1_, icur2_, seen_)) {
            return spinfo_->get_next(icur1_, icur2_, seen_);
        }
    };

private:
    Seq2 seq2_;     // Second sequence to union.
    Pred pred_;     // Predicate used to compare elements

public:
    explicit union_impl(Seq2&& seq2, Pred&& pred)
        : seq2_(std::forward<Seq2>(seq2)),
          pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    union_impl(const union_impl&) = delete;
    union_impl(union_impl&&) = default;
    union_impl& operator=(const union_impl&) = delete;
    union_impl& operator=(union_impl&&) = default;

    template<typename Seq1>
    auto operator()(Seq1&& seq1)
        -> enumerable<typename next_impl<Seq1>::enum_type>
    {
        return next_impl<Seq1>(std::forward<Seq1>(seq1),
                               std::forward<Seq2>(seq2_),
                               std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::where()</tt> et al implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::where()</tt> and
 * <tt>coveo::linq::where_with_index()</tt> LINQ operators.
 *
 * @tparam Pred Predicate used to filter elements.
 * @see coveo::linq::where()
 * @see coveo::linq::where_with_index()
 */
template<typename Pred>
class where_impl
{
public:
    // Implementation of next delegate for where/where_with_index operators.
    template<typename Seq>
    class next_impl
    {
    private:
        // Iterator for the sequence.
        using iterator_type = typename seq_traits<Seq>::iterator_type;

        // Bean storing info shared among delegates.
        struct where_info {
            Seq seq_;               // Sequence being iterated.
            iterator_type iend_;    // Iterator pointing at end of seq_
            Pred pred_;             // Predicate to satisfy

            where_info(Seq&& seq, Pred&& pred)
                : seq_(std::forward<Seq>(seq)),
                  iend_(std::end(seq_)),
                  pred_(std::forward<Pred>(pred)) { }

            // Cannot copy/move, stored in a shared_ptr
            where_info(const where_info&) = delete;
            where_info& operator=(const where_info&) = delete;
        };
        using where_info_sp = std::shared_ptr<where_info>;

        where_info_sp spinfo_;      // Shared info containing sequence and predicate.
        iterator_type icur_;        // Pointer at current element.
        std::size_t idx_;           // Index of current element.

    public:
        next_impl(Seq&& seq, Pred&& pred)
            : spinfo_(std::make_shared<where_info>(std::forward<Seq>(seq),
                                                   std::forward<Pred>(pred))),
              icur_(std::begin(spinfo_->seq_)), idx_(0) { }

        auto operator()() -> typename seq_traits<Seq>::pointer {
            typename seq_traits<Seq>::pointer pobj = nullptr;
            for (; pobj == nullptr && icur_ != spinfo_->iend_; ++icur_, ++idx_) {
                typename seq_traits<Seq>::reference robjtmp = *icur_;
                auto pobjtmp = std::addressof(robjtmp);
                if (spinfo_->pred_(*pobjtmp, idx_)) {
                    // This element satistifies the predicate, return it.
                    pobj = pobjtmp;
                }
            }
            return pobj;
        }
    };

private:
    Pred pred_;     // Predicate to satisfy.

public:
    explicit where_impl(Pred&& pred)
        : pred_(std::forward<Pred>(pred)) { }

    // Movable but not copyable
    where_impl(const where_impl&) = delete;
    where_impl(where_impl&&) = default;
    where_impl& operator=(const where_impl&) = delete;
    where_impl& operator=(where_impl&&) = default;

    template<typename Seq>
    auto operator()(Seq&& seq)
        -> enumerable<typename seq_traits<Seq>::value_type>
    {
        return next_impl<Seq>(std::forward<Seq>(seq), std::forward<Pred>(pred_));
    }
};

/**
 * @internal
 * @brief <tt>coveo::linq::zip()</tt> implementation.
 * @headerfile linq_detail.h <coveo/linq/detail/linq_detail.h>
 *
 * Implementation of the <tt>coveo::linq::zip()</tt> LINQ operator.
 *
 * @tparam Seq2 Second sequence to zip together. The first sequence will
 *              have been provided by the call to <tt>coveo::linq::from()</tt>.
 * @tparam ResultSelector Selector used to "zip" two sequence elements.
 * @see coveo::linq::zip()
 */
template<typename Seq2, typename ResultSelector>
class zip_impl
{
private:
    // Implementation of next delegate for this operator.
    template<typename Seq1, typename CU, typename RU>
    class next_impl
    {
    private:
        // Iterator types for the sequences.
        using first_iterator_type   = typename seq_traits<Seq1>::iterator_type;
        using second_iterator_type  = typename seq_traits<Seq2>::iterator_type;

        // Containers storing zipped elements.
        using zipped_v              = std::vector<RU>;
        using zipped_l              = std::forward_list<RU>;
        using zipped_l_iterator     = typename zipped_l::iterator;

        // Bean storing info shared among delegates.
        struct zip_info {
            Seq1 seq1_;                     // First sequence to zip.
            first_iterator_type icur1_;     // Iterator pointing at current element of seq1_.
            first_iterator_type iend1_;     // Iterator pointing at end of seq1_.
            Seq2 seq2_;                     // Second sequence to zip.
            second_iterator_type icur2_;    // Iterator pointing at current element of seq2_.
            second_iterator_type iend2_;    // Iterator pointing at end of seq2_.
            ResultSelector result_sel_;     // Selector producing the results.
            zipped_v vzipped_;              // Vector of zipped elements.
            zipped_l lzipped_;              // List of zipped elements.
            zipped_l_iterator llast_;       // Iterator pointing to last element in lzipped_ (before end()).
            bool use_vector_;               // Whether we use lzipped_ (false) or vzipped_ (true).

            zip_info(Seq1&& seq1, Seq2&& seq2, ResultSelector&& result_sel,
                     const typename enumerable<CU>::size_delegate& siz)
                : seq1_(std::forward<Seq1>(seq1)),
                  icur1_(std::begin(seq1_)),
                  iend1_(std::end(seq1_)),
                  seq2_(std::forward<Seq2>(seq2)),
                  icur2_(std::begin(seq2_)),
                  iend2_(std::end(seq2_)),
                  result_sel_(std::forward<ResultSelector>(result_sel)),
                  vzipped_(),
                  lzipped_(),
                  llast_(lzipped_.before_begin()),
                  use_vector_(siz != nullptr)
            {
                if (siz != nullptr) {
                    vzipped_.reserve(siz());
                }
            }

            // Cannot copy/move, stored in a shared_ptr
            zip_info(const zip_info&) = delete;
            zip_info& operator=(const zip_info&) = delete;

            auto get_next_in_vector(std::size_t& vncur) -> CU* {
                while (vzipped_.size() <= vncur && icur1_ != iend1_ && icur2_ != iend2_) {
                    vzipped_.emplace_back(result_sel_(*icur1_, *icur2_));
                    ++icur1_;
                    ++icur2_;
                }
                return vzipped_.size() > vncur ? std::addressof(vzipped_[vncur++])
                                               : nullptr;
            }

            auto get_next_in_list(zipped_l_iterator& licur) -> CU* {
                while (licur == llast_ && icur1_ != iend1_ && icur2_ != iend2_) {
                    llast_ = lzipped_.emplace_after(llast_, result_sel_(*icur1_, *icur2_));
                    ++icur1_;
                    ++icur2_;
                }
                return licur != llast_ ? std::addressof(*++licur)
                                       : nullptr;
            }

            auto get_next(std::size_t& vncur, zipped_l_iterator& licur) -> CU* {
                return use_vector_ ? get_next_in_vector(vncur)
                                   : get_next_in_list(licur);
            }
        };
        using zip_info_sp = std::shared_ptr<zip_info>;

        zip_info_sp spinfo_;            // Bean containing shared info.
        std::size_t vncur_;             // Index of current element (if in vector).
        zipped_l_iterator licur_;       // Iterator pointing at current element (if in list).

    public:
        next_impl(Seq1&& seq1, Seq2&& seq2, ResultSelector&& result_sel,
                  const typename enumerable<CU>::size_delegate& siz)
            : spinfo_(std::make_shared<zip_info>(std::forward<Seq1>(seq1),
                                                 std::forward<Seq2>(seq2),
                                                 std::forward<ResultSelector>(result_sel),
                                                 siz)),
              vncur_(0),
              licur_(spinfo_->lzipped_.before_begin()) { }

        auto operator()() -> CU* {
            return spinfo_->get_next(vncur_, licur_);
        }
    };

private:
    Seq2 seq2_;                     // Second sequence to zip.
    ResultSelector result_sel_;     // Selector producing the results.

public:
    zip_impl(Seq2&& seq2, ResultSelector&& result_sel)
        : seq2_(std::forward<Seq2>(seq2)),
          result_sel_(std::forward<ResultSelector>(result_sel)) { }

    // Movable but not copyable
    zip_impl(const zip_impl&) = delete;
    zip_impl(zip_impl&&) = default;
    zip_impl& operator=(const zip_impl&) = delete;
    zip_impl& operator=(zip_impl&&) = default;

    template<typename Seq1,
             typename _SelectorRes = decltype(std::declval<ResultSelector>()(std::declval<typename seq_traits<Seq1>::reference>(),
                                                                             std::declval<typename seq_traits<Seq2>::reference>())),
             typename _CU = typename seq_element_traits<_SelectorRes>::const_value_type,
             typename _RU = typename seq_element_traits<_SelectorRes>::raw_value_type>
    auto operator()(Seq1&& seq1) -> enumerable<_CU> {
        auto siz1 = try_get_size_delegate(seq1);
        auto siz2 = try_get_size_delegate(seq2_);
        typename enumerable<_CU>::size_delegate siz;
        if (siz1 != nullptr && siz2 != nullptr) {
            const std::size_t min_size = std::min(siz1(), siz2());
            siz = [min_size]() -> std::size_t { return min_size; };
        }
        return enumerable<_CU>(next_impl<Seq1, _CU, _RU>(std::forward<Seq1>(seq1),
                                                         std::forward<Seq2>(seq2_),
                                                         std::forward<ResultSelector>(result_sel_),
                                                         siz),
                               siz);
    }
};

} // detail
} // linq
} // coveo

#endif // COVEO_LINQ_DETAIL_H
