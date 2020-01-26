/**
 * @file
 * @brief C++ implementation of .NET's IEnumerable-like data structure.
 *
 * This file contains the definition of <tt>coveo::enumerable</tt>, which
 * is a wrapper for a sequence similar to .NET's <tt>IEnumerable&lt;T&gt;</tt>.
 * Also includes helper methods and functions to create <tt>enumerable</tt>s
 * from single values, containers, etc.
 *
 * @copyright 2016-2019, Coveo Solutions Inc.
 *            Distributed under the Apache License, Version 2.0 (see <a href="https://github.com/coveo/linq/blob/master/LICENSE">LICENSE</a>).
 */

#ifndef COVEO_ENUMERABLE_H
#define COVEO_ENUMERABLE_H

#include "coveo/seq/sequence_util.h"
#include <coveo/seq/detail/enumerable_detail.h>

#include <cstddef>
#include <iterator>
#include <functional>
#include <memory>
#include <type_traits>

namespace coveo {

/**
 * @brief Type-erased sequence wrapper.
 * @headerfile enumerable.h <coveo/seq/enumerable.h>
 *
 * Inspired by .NET's <tt>IEnumerable&lt;T&gt;</tt>, <tt>coveo::enumerable</tt>
 * is a type-erased wrapper for a multipass, forward-only sequence of elements.
 * It is possible to iterate over the elements using <tt>begin()</tt> / <tt>end()</tt>
 * (or <tt>cbegin()</tt> / <tt>cend()</tt>, which is equivalent since this class is
 * immutable).
 *
 * In order to fetch the elements to return, this class uses a <em>next delegate</em> -
 * a function that returns a pointer to the next element every time it is called,
 * finally returning @c nullptr when the enumeration is over. The next delegate
 * is provided at construction time and cannot change.
 *
 * Optionally, it is also possible to specify a <em>size delegeate</em> for the
 * @c enumerable at construction time. If provided, it will be used to fetch the
 * number of elements in the sequence when <tt>size()</tt> is called; otherwise,
 * a slower algorithm is used (iterating over the entire sequence using
 * <tt>std::distance()</tt>). It is possible to determine if the @c enumerable
 * has a fast size delegate by using <tt>has_fast_size()</tt>.
 *
 * While this class is immutable, the elements it returns are not necessarily so.
 * In order to wrap a sequence of @c const elements, @c const must also be specified
 * in this class' template argument:
 *
 * @code
 *   coveo::enumerable<const int> e1; // Iterates over const integers
 *   coveo::enumerable<int> e2;       // Iterates over non-const integers
 * @endcode
 *
 * @tparam T Type of elements stored in the sequence.
 */
template<typename T>
class enumerable
{
/// @cond

    // Allows compatible enumerables to be friend with each other
    template<typename> friend class enumerable;

/// @endcond

public:
    /**
     * @brief Type of element in the sequence.
     *
     * Type of element stored in the sequence.
     */
    using value_type = typename seq_element_traits<T>::value_type;

    /**
     * @brief Raw type of element in the sequence.
     *
     * Same as <tt>coveo::enumerable::value_type</tt>, but "raw", e.g. without @c const or @c volatile.
     */
    using raw_value_type = typename seq_element_traits<T>::raw_value_type;

    /**
     * @brief Pointer to a sequence element.
     *
     * Pointer to an element in the sequence.
     * Corresponds to <tt>coveo::enumerable::value_type*</tt>.
     * This is the type of pointer returned by the <tt>coveo::enumerable::next_delegate</tt>.
     */
    using pointer = typename seq_element_traits<T>::pointer;

    /**
     * @brief Reference to a sequence element.
     *
     * Reference to an element in the sequence.
     * Corresponds to <tt>coveo::enumerable::value_type&</tt>.
     * This is the type of reference returned by the <tt>coveo::enumerable::iterator</tt>s.
     */
    using reference = typename seq_element_traits<T>::reference;

    /**
     * @brief Delegate to fetch next element.
     *
     * Delegate that will be called by the @c enumerable to fetch
     * the next element in the sequence when needed. The delegate
     * must return a <tt>coveo::enumerable::pointer</tt> to the next
     * element if there is one, or @c nullptr when done.
     */
    using next_delegate = std::function<pointer()>;

    /**
     * @brief Delegate to fetch sequence size.
     *
     * Delegate that will be called by the @c enumerable to fetch
     * the number of elements in the sequence when <tt>size()</tt>
     * is called.
     */
    using size_delegate = std::function<std::size_t()>;

    // Forward declaration of iterator class.
    class iterator;

    /**
     * @brief @c iterator alias.
     *
     * Alias for <tt>coveo::enumerable::iterator</tt>. Provided because even though
     * <tt>begin()</tt> and <tt>end()</tt> are already @c const, some generic code
     * might want to use @c const_iterator.
     */
    using const_iterator = iterator;

private:
    next_delegate zero_;    // Next delegate which we will clone to iterate sequence.
    size_delegate size_;    // Optional size delegate.

public:
    /**
     * @brief Default constructor.
     *
     * Default constructor. Wraps an empty sequence.
     *
     * Using this constructor is equivalent to calling <tt>empty()</tt>.
     *
     * @see empty()
     */
    enumerable()
        : zero_([]() -> pointer { return nullptr; }),
          size_([]() -> std::size_t { return 0; }) { }

    /**
     * @brief Constructor with delegates.
     *
     * Constructor that accepts a <tt>coveo::enumerable::next_delegate</tt>
     * as well as an optional <tt>coveo::enumerable::size_delegate</tt>.
     * The <em>next delegate</em> is used to fetch the elements to return
     * during iteration. The <em>size delegate</em>, if provided, will
     * be called to know the <tt>size()</tt> of the sequence.
     *
     * @param next Function object to use as next delegate.
     * @param siz Optional function object to use as size delegate. Defaults to @c nullptr.
     * @see coveo::enumerable::next_delegate
     * @see coveo::enumerable::size_delegate
     * @see size()
     * @see has_fast_size()
     */
    template<typename F,
             typename _S = std::nullptr_t,
             typename = typename std::enable_if<!is_enumerable<typename std::decay<F>::type>::value &&
                                                (!detail::has_begin<typename std::decay<F>::type>::value ||
                                                 !detail::has_end<typename std::decay<F>::type>::value), void>::type>
    enumerable(F&& next, _S&& siz = nullptr)
        : zero_(std::forward<F>(next)), size_(std::forward<_S>(siz)) { }

    /**
     * @brief Constructor with container.
     *
     * Constructor that wraps a container. The container must have the
     * following methods to be accepted by this constructor:
     *
     * - @c begin (or a specialization of <tt>std::begin()</tt>)
     * - @c end (or a specialization of <tt>std::end()</tt>)
     *
     * The container will be wrapped in the enumerable, either by
     * keeping a reference to it or by storing it internally, depending
     * on how the container is passed to the method (if it is <tt>move</tt>d,
     * it will be stored internally).
     *
     * Using this constructor is equivalent to calling <tt>for_container()</tt>.
     * It is provided implicitely so that it is possible to call a function
     * declared as accepting a <tt>coveo::enumerable</tt> with a container
     * instance directly.
     *
     * @param cnt Container to wrap.
     * @see for_container()
     */
    template<typename C,
             typename = typename std::enable_if<!is_enumerable<typename std::decay<C>::type>::value &&
                                                detail::has_begin<typename std::decay<C>::type>::value &&
                                                detail::has_end<typename std::decay<C>::type>::value, void>::type>
    enumerable(C&& cnt)
        : zero_(), size_() { *this = for_container(std::forward<C>(cnt)); }

    /**
     * @brief Constructor for non-<tt>const</tt> to @c const conversion.
     *
     * Constructor that creates an enumerable over @c const elements from
     * an enumerable over non-<tt>const</tt> elements of the same type.
     * Allows implicit non-<tt>const</tt> to @c const conversion.
     *
     * @param e <tt>coveo::enumerable</tt> to convert.
     */
    template<typename U,
             typename = typename std::enable_if<!std::is_const<U>::value &&
                                                std::is_same<T, typename std::add_const<U>::type>::value, void>::type>
    enumerable(enumerable<U> e)
        : zero_(std::move(e.zero_)), size_(std::move(e.size_)) { }

    /**
     * @brief Assignment operator for non-<tt>const</tt> to @c const conversion.
     *
     * Assignment operator that allows an enumerable over non-<tt>const</tt>
     * elements to be assigned to an enumerable over @c const elements of
     * the same type. Allows non-<tt>const</tt> to @c const conversion.
     *
     * @param e <tt>coveo::enumerable</tt> to convert.
     * @return Reference to @c this enumerable.
     */
    template<typename U,
             typename = typename std::enable_if<!std::is_const<U>::value &&
                                                std::is_same<T, typename std::add_const<U>::type>::value, void>::type>
    enumerable& operator=(enumerable<U> e) {
        zero_ = std::move(e.zero_);
        size_ = std::move(e.size_);
        return *this;
    }

    /**
     * @brief Iterator to beginning of sequence.
     *
     * Returns a <tt>coveo::enumerable::iterator</tt> pointing at
     * the beginning of the sequence. Together with <tt>end()</tt>,
     * it can be used to enumerate the elements in the sequence.
     *
     * The iterator allows the elements to be modified if the
     * enumerable's type @c T is not @c const.
     *
     * @return @c iterator to beginning of sequence.
     * @see end()
     */
    iterator begin() const {
        return iterator(*this, false);
    }

    /**
     * @brief Alias for <tt>begin()</tt>.
     *
     * Since this class is immutable, calling this is
     * equivalent to calling <tt>begin()</tt>.
     *
     * @return @c iterator to beginning of sequence.
     * @see begin()
     * @see cend()
     */
    iterator cbegin() const {
        return begin();
    }

    /**
     * @brief Iterator to end of sequence.
     *
     * Returns a <tt>coveo::enumerable::iterator</tt> pointing at
     * the end of the sequence. Together with <tt>begin()</tt>,
     * it can be used to enumerable the elements in the sequence.
     *
     * @return @c iterator to end of sequence.
     * @see begin()
     */
    iterator end() const {
        return iterator(*this, true);
    }

    /**
     * @brief Alias for <tt>end()</tt>.
     *
     * Since this class is immutable, calling this is
     * equivalent to calling <tt>end()</tt>.
     *
     * @return @c iterator to end of sequence.
     * @see end()
     * @see cbegin()
     */
    iterator cend() const {
        return end();
    }

    /**
     * @brief Determines if <tt>size()</tt> is fast.
     *
     * Method that can be used to know if this @c enumerable has a
     * <tt>coveo::enumerable::size_delegate</tt>. If so, calling
     * <tt>size()</tt> should be reasonably fast; otherwise,
     * <tt>size()</tt> will have to iterate over the entire sequence
     * in order to return a result.
     *
     * @return @c true if <tt>size()</tt> should be reasonably fast.
     * @see size()
     */
    bool has_fast_size() const {
        // If we have a delegate, size() should be reasonably fast
        return size_ != nullptr;
    }

    /**
     * @brief Size of sequence.
     *
     * Returns the number of elements in the sequence. If this @c enumerable
     * has a <tt>coveo::enumerable::size_delegate</tt>, it will be used to
     * fetch the information; otherwise, a more lenghty process is used
     * (e.g., using <tt>std::distance()</tt>, which forces the iteration
     * over the entire sequence).
     *
     * @return Number of elements in the sequence.
     * @see has_fast_size()
     */
    std::size_t size() const {
        // If we have a delegate, use it, otherwise use distance.
        return size_ != nullptr ? size_()
                                : static_cast<std::size_t>(std::distance(begin(), end()));
    }

    /**
     * @brief Non-<tt>const</tt> to @c const explicit conversion.
     *
     * Returns a copy of this @c enumerable, but iterating over
     * @c const elements. Can be used to explicitely perform a
     * non-<tt>const</tt> to @c const conversion.
     *
     * @return @c enumerable over @c const elements.
     */
    auto as_const() const -> enumerable<typename std::add_const<T>::type> {
        return enumerable<typename std::add_const<T>::type>(*this);
    }

public:
    /**
     * @brief Iterator for elements in the sequence.
     *
     * Iterator for elements in a <tt>coveo::enumerable</tt>'s sequence.
     * Uses the <tt>enumerable</tt>'s <em>next delegate</em> to fetch
     * references to the elements during iteration.
     *
     * Elements are always late-initialized, e.g. they are not fetched
     * from the next delegate until they are accessed.
     *
     * @see coveo::enumerable::begin()
     * @see coveo::enumerable::end()
     */
    class iterator
    {
    public:
        /**
         * @brief Iterator category.
         *
         * Standard iterator typedef for iterator category. Because @c enumerable is a forward-only
         * sequence, this corresponds to <tt>std::forward_iterator_tag</tt> to identify a
         * <a href="https://en.cppreference.com/w/cpp/named_req/ForwardIterator">ForwardIterator</a>.
         */
        using iterator_category = std::forward_iterator_tag;

        /**
         * @brief Type of elements returned by iterator.
         *
         * Type of the elements returned by this iterator.
         *
         * @see coveo::enumerable::value_type
         */
        using value_type = typename enumerable<T>::value_type;

        /**
         * @brief Raw type of elements returned by iterator.
         *
         * Type of the elements returned by this iterator, but "raw",
         * e.g. without @c const or @c volatile.
         *
         * @see coveo::enumerable::raw_value_type
         */
        using raw_value_type = typename enumerable<T>::raw_value_type;

        /**
         * @brief Difference type.
         *
         * Standard iterator typedef for difference between iterators.
         * Corresponds to <tt>std::ptfdiff_t</tt>.
         */
        using difference_type = std::ptrdiff_t;

        /**
         * @brief Pointer to elements returned by iterator.
         *
         * Pointer to the type of elements returned by this iterator.
         *
         * @see coveo::enumerable::pointer
         */
        using pointer = typename enumerable<T>::pointer;

        /**
         * @brief Reference to elements returned by iterator.
         *
         * Reference to the type of elements returned by this iterator.
         * This is the type of reference returned by <tt>operator*()</tt>.
         *
         * @see coveo::enumerable::reference.
         */
        using reference = typename enumerable<T>::reference;

    private:
        const enumerable<T>* pparent_ = nullptr;        // Parent enumerable or nullptr if unbound
        mutable next_delegate next_;                    // Delegate used to fetch elements
        mutable pointer pcur_ = nullptr;                // Pointer to current element or nullptr when at end of sequence
        mutable bool has_cur_ = true;                   // Whether pcur_ has been loaded for current element
        size_t pos_ = 0;                                // Position in sequence, to compare iterators

        // Fetches value of pcur_, late-loading it if needed
        pointer get_pcur() const {
            if (!has_cur_) {
                pcur_ = next_();
                has_cur_ = true;
            }
            return pcur_;
        }
    
    public:
        /**
         * @brief Default constructor.
         *
         * Default constructor. Creates an invalid iterator that
         * cannot be dereferenced.
         */
        iterator() = default;

        /**
         * @brief Constructor from @c enumerable.
         *
         * Constructor that is used by <tt>coveo::enumerable</tt>
         * to create its iterators.
         *
         * @param parent Parent @c enumerable to iterate.
         * @param is_end Whether this iterator should point to the
         *               beginning of the sequence (@c false) or to
         *               the end of the sequence (@c true).
         */
        iterator(const enumerable<T>& parent, bool is_end)
            : pparent_(&parent), next_(!is_end ? parent.zero_ : next_delegate()), has_cur_(is_end) { }

        /**
         * @brief Copy constructor.
         *
         * Copy constructor. Creates a copy of another iterator that
         * will point to the same point in the enumeration.
         *
         * @param obj Iterator to copy.
         */
        iterator(const iterator& obj) = default;

        /**
         * @brief Move constructor.
         *
         * Move constructor. Creates an iterator by moving the internals
         * from another iterator. After this method returns, @c obj is
         * invalid and cannot be used.
         *
         * @param obj Iterator to move.
         */
        iterator(iterator&& obj)
            : pparent_(obj.pparent_), next_(std::move(obj.next_)), pcur_(obj.pcur_),
              has_cur_(obj.has_cur_), pos_(obj.pos_)
        {
            obj.pparent_ = nullptr;
            obj.pcur_ = nullptr;
            obj.has_cur_ = true;
            obj.pos_ = 0;
        }

        /**
         * @brief Assignment operator.
         *
         * Assignment operator. Copies another iterator, now pointing
         * to the same point in the enumeration.
         *
         * @param obj Iterator to copy.
         * @return Reference to @c this iterator.
         */
        iterator& operator=(const iterator& obj) = default;

        /**
         * @brief Move assignment operator.
         *
         * Move assignment operator. Moves the internals of another
         * iterator to this one. After this method returns, @c obj
         * is invalid and cannot be used.
         *
         * @param obj Iterator to move.
         * @return Reference to @c this iterator.
         */
        iterator& operator=(iterator&& obj) {
            pparent_ = obj.pparent_;
            next_ = std::move(obj.next_);
            pcur_ = obj.pcur_;
            has_cur_ = obj.has_cur_;
            pos_ = obj.pos_;

            obj.pparent_ = nullptr;
            obj.pcur_ = nullptr;
            obj.has_cur_ = true;
            obj.pos_ = 0;

            return *this;
        }
        
        /**
         * @brief Element access.
         *
         * Returns a reference to the current element in the enumeration,
         * fetching it from the next delegate on the first call.
         *
         * @return Reference to current element.
         */
        reference operator*() const {
            return *get_pcur();
        }

        /**
         * @brief Element pointer access.
         *
         * Returns a pointer to the current element in the enumeration,
         * fetching it from the next delegate on the first call. Can be
         * used to call methods or access members of the element.
         *
         * @return Pointer to current element.
         */
        pointer operator->() const {
            return get_pcur();
        }
        
        /**
         * @brief Moves to next element (pre-increment).
         *
         * Moves forward to the next element in the enumeration.
         * Cannot be used if this iterator points to the end of
         * the enumeration.
         *
         * @return Reference to @c this iterator AFTER the move.
         */
        iterator& operator++() {
            if (has_cur_) {
                pcur_ = nullptr;
                has_cur_ = false;
            } else {
                next_();
            }
            ++pos_;
            return *this;
        }

        /**
         * @brief Moves to next element (post-increment).
         *
         * Moves forward to the next element in the enumeration.
         * Cannot be used if this iterator points to the end of
         * the enumeration.
         *
         * @return Iterator BEFORE the move.
         */
        iterator operator++(int) {
            iterator it(*this);
            ++*this;
            return it;
        }
        
        /**
         * @brief Compares iterator for equality.
         *
         * Determines if two iterators point to the same point in
         * the same sequence.
         *
         * @param left First iterator to compare.
         * @param right Second iterator to compare.
         * @return @c true if @c left and @c right point to the same
         *         point in the same sequence.
         */
        friend bool operator==(const iterator& left, const iterator& right) {
            return left.pparent_ == right.pparent_ &&
                   (left.get_pcur() == nullptr) == (right.get_pcur() == nullptr) &&
                   (left.get_pcur() == nullptr || left.pos_ == right.pos_);
        }

        /**
         * @brief Compares iterator for inequality.
         *
         * Determines if two iterators point to a different point
         * in perhaps different sequences.
         *
         * @param left First iterator to compare.
         * @param right Second iterator to compare.
         * @return @c true if @c left does not point to the same point
         *         in the sequence as @c right.
         * @see operator==()
         */
        friend bool operator!=(const iterator& left, const iterator& right) {
            return !(left == right);
        }
    };

public:
    // Helper static methods

    /**
     * @brief Returns @c enumerable over empty sequence.
     *
     * Static method that returns a <tt>coveo::enumerable</tt> over an
     * empty sequence. Equivalent to using the default constructor.
     *
     * @return @c enumerable over empty sequence.
     */
    static enumerable<T> empty() {
        return enumerable<T>();
    }

    /**
     * @brief Returns @c enumerable over sequence of one element (stored internally).
     *
     * Static method that returns a <tt>coveo::enumerable</tt> over
     * a sequence of one element. The @c enumerable will store the
     * element internally, moving it if possible.
     *
     * In order to auto-discover the enumerable's type, use
     * <tt>coveo::enumerate_one()</tt>.
     *
     * @param obj Object to store as the sequence's only element.
     * @return @c enumerable over sequence of one element.
     * @see coveo::enumerate_one()
     */
    template<typename U>
    static enumerable<T> for_one(U&& obj) {
        auto spobj = std::make_shared<value_type>(std::forward<U>(obj));
        bool available = true;
        return enumerable<T>([spobj, available]() mutable {
            pointer pobj = nullptr;
            if (available) {
                pobj = spobj.get();
                available = false;
            }
            return pobj;
        }, []() -> std::size_t {
            return 1;
        });
    }

    /**
     * @brief Returns @c enumerable over sequence of one element (stored externally).
     *
     * Static method that returns a <tt>coveo::enumerable</tt> over
     * a sequence of one element. The @c enumerable will only store
     * a reference to the element.
     *
     * In order to auto-discover the enumerable's type, use
     * <tt>coveo::enumerate_one_ref()</tt>.
     *
     * @param obj Object to use as the sequence's only element.
     * @return @c enumerable over sequence of one element.
     * @see coveo::enumerate_one_ref()
     */
    static enumerable<T> for_one_ref(reference obj) {
        bool available = true;
        return enumerable<T>([&obj, available]() mutable {
            pointer pobj = nullptr;
            if (available) {
                pobj = std::addressof(obj);
                available = false;
            }
            return pobj;
        }, []() -> std::size_t {
            return 1;
        });
    }

    /**
     * @brief Returns @c enumerable over sequence bound by iterators.
     *
     * Static method that returns a <tt>coveo::enumerable</tt>
     * over a sequence bound by two iterators.
     *
     * In order to auto-discover the enumerable's type, use
     * <tt>coveo::enumerate_range()</tt>.
     *
     * @param ibeg Iterator pointing at beginning of range.
     * @param iend Iterator pointing at end of range.
     * @return @c enumerable over sequence bound by <tt>[ibeg, iend[</tt>.
     * @see coveo::enumerate_range()
     */
    template<typename It>
    static enumerable<T> for_range(It ibeg, It iend) {
        auto it = std::move(ibeg);
        return enumerable<T>([it, iend]() mutable {
            pointer pobj = nullptr;
            if (it != iend) {
                reference robj = *it;
                pobj = std::addressof(robj);
                ++it;
            }
            return pobj;
        }, try_get_size_delegate_for_iterators(it, iend));
    }

    /**
     * @brief Returns @c enumerable over container's sequence (stored externally).
     *
     * Static method that returns a <tt>coveo::enumerable</tt> over
     * a sequence stored in a container. Only a reference to the
     * container is kept by the @c enumerable.
     *
     * In order to auto-discover the enumerable's type, use
     * <tt>coveo::enumerate_container()</tt>.
     *
     * @param cnt Container whose elements to wrap.
     * @return @c enumerable over sequence of elements from @c cnt.
     * @see coveo::enumerate_container()
     */
    template<typename C>
    static enumerable<T> for_container(C& cnt) {
        auto it = std::begin(cnt);
        auto end = std::end(cnt);
        return enumerable<T>([it, end]() mutable {
            pointer pobj = nullptr;
            if (it != end) {
                reference robj = *it;
                pobj = std::addressof(robj);
                ++it;
            }
            return pobj;
        }, try_get_size_delegate(cnt));
    }

    /**
     * @brief Returns @c enumerable over container's sequence (stored internally).
     *
     * Static method that returns a <tt>coveo::enumerable</tt> over
     * a sequence stored in a container. The container is moved to
     * the @c enumerable and stored internally.
     *
     * In order to auto-discover the enumerable's type, use
     * <tt>coveo::enumerate_container()</tt>.
     *
     * @param cnt Container to store in the enumerable.
     * @return @c enumerable over sequence of elements from @c cnt.
     * @see coveo::enumerate_container()
     */
    template<typename C,
             typename = typename std::enable_if<!std::is_reference<C>::value, void>::type>
    static enumerable<T> for_container(C&& cnt) {
        auto spcnt = std::make_shared<C>(std::move(cnt));
        auto it = std::begin(*spcnt);
        auto end = std::end(*spcnt);
        return enumerable<T>([spcnt, it, end]() mutable {
            pointer pobj = nullptr;
            if (it != end) {
                reference robj = *it;
                pobj = std::addressof(robj);
                ++it;
            }
            return pobj;
        }, try_get_size_delegate(*spcnt));
    }

    /**
     * @brief Returns @c enumerable over array's sequence.
     *
     * Static method that returns a <tt>coveo::enumerable</tt> over
     * a sequence stored in a dynamic array. Equivalent to using
     * <tt>for_range()</tt> without using pointer arithmetic.
     *
     * In order to auto-discover the enumerable's type, use
     * <tt>coveo::enumerate_array()</tt>.
     *
     * @param parr Pointer to beginning of array.
     * @param siz Size of array.
     * @return @c enumerable over sequence in array.
     * @see coveo::enumerate_array()
     */
    static enumerable<T> for_array(pointer parr, size_t siz) {
        return for_range(parr, parr + siz);
    }
};

// Helper functions corresponding to the helper static methods in enumerable.

/**
 * @brief Returns @c enumerable over sequence of one element (stored internally).
 *
 * Returns a <tt>coveo::enumerable</tt> over a sequence of one element.
 * The @c enumerable will store the element internally, moving it if possible.
 *
 * The <tt>enumerable</tt>'s type is deduced from the type of the element.
 *
 * @param obj Object to store as the sequence's only element.
 * @return @c enumerable over sequence of one element.
 * @see coveo::enumerable::for_one()
 */
template<typename U>
auto enumerate_one(U&& obj) -> enumerable<typename seq_element_traits<U>::const_value_type> {
    return enumerable<typename seq_element_traits<U>::const_value_type>::for_one(std::forward<U>(obj));
}

/**
 * @brief Returns @c enumerable over sequence of one element (stored externally).
 *
 * Returns a <tt>coveo::enumerable</tt> over a sequence of one element.
 * The @c enumerable will only store a reference to the element.
 *
 * The <tt>enumerable</tt>'s type is deduced from the type of the element.
 *
 * @param obj Object to use as the sequence's only element.
 * @return @c enumerable over sequence of one element.
 * @see coveo::enumerable::for_one_ref()
 */
template<typename T>
auto enumerate_one_ref(T& obj) -> enumerable<T> {
    return enumerable<T>::for_one_ref(obj);
}

/**
 * @brief Returns @c enumerable over sequence bound by iterators.
 *
 * Returns a <tt>coveo::enumerable</tt> over a sequence bound by two iterators.
 *
 * The <tt>enumerable</tt>'s type is deduced from the type of element returned
 * by the iterators.
 *
 * @param ibeg Iterator pointing at beginning of range.
 * @param iend Iterator pointing at end of range.
 * @return @c enumerable over sequence bound by <tt>[ibeg, iend[</tt>.
 * @see coveo::enumerable::for_range()
 */
template<typename It>
auto enumerate_range(It ibeg, It iend)
    -> enumerable<typename seq_element_traits<decltype(*std::declval<It>())>::value_type>
{
    return enumerable<typename seq_element_traits<decltype(*std::declval<It>())>::value_type>::for_range(
        std::move(ibeg), std::move(iend));
}

/**
 * @brief Returns @c enumerable over container's sequence (stored externally).
 *
 * Returns a <tt>coveo::enumerable</tt> over a sequence stored
 * in a container. Only a reference to the container is kept by
 * the @c enumerable.
 *
 * The <tt>enumerable</tt>'s type is deduced from the type of
 * elements stored in the container.
 *
 * @param cnt Container whose elements to wrap.
 * @return @c enumerable over sequence of elements from @c cnt.
 * @see coveo::enumerable::for_container()
 */
template<typename C>
auto enumerate_container(C& cnt)
    -> enumerable<typename seq_element_traits<decltype(*std::begin(std::declval<C>()))>::value_type>
{
    return enumerable<typename seq_element_traits<decltype(*std::begin(std::declval<C>()))>::value_type>::for_container(
        cnt);
}

/**
 * @brief Returns @c enumerable over container's sequence (stored internally).
 *
 * Returns a <tt>coveo::enumerable</tt> over a sequence stored
 * in a container. The container is moved to the @c enumerable
 * and stored internally.
 *
 * The <tt>enumerable</tt>'s type is deduced from the type of
 * elements stored in the container.
 *
 * @param cnt Container to store in the enumerable.
 * @return @c enumerable over sequence of elements from @c cnt.
 * @see coveo::enumerable::for_container()
 */
template<typename C,
         typename = typename std::enable_if<!std::is_reference<C>::value, void>::type>
auto enumerate_container(C&& cnt)
    -> enumerable<typename seq_element_traits<decltype(*std::begin(std::declval<C>()))>::const_value_type>
{
    return enumerable<typename seq_element_traits<decltype(*std::begin(std::declval<C>()))>::const_value_type>::for_container(
        std::move(cnt));
}

/**
 * @brief Returns @c enumerable over array's sequence.
 *
 * Returns a <tt>coveo::enumerable</tt> over a sequence stored in a
 * dynamic array. Equivalent to using <tt>coveo::enumerate_range()</tt>
 * without using pointer arithmetic.
 *
 * The <tt>enumerable</tt>'s type is deduced from the type of elements
 * stored in the array.
 *
 * @param parr Pointer to beginning of array.
 * @param siz Size of array.
 * @return @c enumerable over sequence in array.
 * @see coveo::enumerable::for_array()
 */
template<typename T>
auto enumerate_array(T* parr, size_t siz) -> enumerable<T> {
    return enumerable<T>::for_array(parr, siz);
}

} // coveo

#endif // COVEO_ENUMERABLE_H
