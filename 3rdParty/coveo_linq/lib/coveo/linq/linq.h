/**
 * @file
 * @brief C++ LINQ operators.
 *
 * This file contains the definition of all LINQ operators, as well
 * as the entry points (e.g., <tt>coveo::linq::from()</tt>, etc.) and
 * the chaining operator (e.g. <tt>coveo::linq::operator|()</tt>).
 * This is the only file that needs to be included to use the library.
 *
 * @copyright 2016-2019, Coveo Solutions Inc.
 *            Distributed under the Apache License, Version 2.0 (see <a href="https://github.com/coveo/linq/blob/master/LICENSE">LICENSE</a>).
 */

/**
 * @mainpage notitle
 * @tableofcontents
 * @section intro Introduction
 *
 * Welcome to the documentation of the <tt>coveo::linq</tt> library. This library implements
 * .NET-like <a href="https://en.wikipedia.org/wiki/Language_Integrated_Query">LINQ</a> operators in C++.
 * These can be chained to build powerful expressions to query, filter and transform data in any
 * type of sequence, like arrays, containers, etc. (anything that supports <tt>std::begin()</tt>
 * and <tt>std::end()</tt> should work).
 *
 * If this is your first time with the library, start at @ref linq "LINQ expressions".
 *
 * @section example Example
 *
 * Here is an example that chains many operators to produce a filtered, ordered sequence:
 *
 * @code
 *   #include <coveo/linq.h>
 *   #include <iostream>
 *
 *   int main()
 *   {
 *       const int FIRST[] = { 42, 23, 66, 13, 11, 7, 24, 10 };
 *       const int SECOND[] = { 67, 22, 13, 23, 41, 66, 6, 7, 10 };
 *
 *       using namespace coveo::linq;
 *
 *       auto is_even = [](int i) { return i % 2 == 0; };
 *
 *       auto seq = from(FIRST)
 *                | intersect(SECOND)                    // Intersect both arrays
 *                | where([](int i) { return i != 13; }) // I'm supersticious, remove 13
 *                | order_by_descending(is_even)         // Place even numbers first
 *                | then_by([](int i) { return i; });    // Then sort numbers ascending
 *
 *       std::cout << std::endl;
 *       for (auto&& elem : seq) {
 *           std::cout << elem << " ";
 *       }
 *       std::cout << std::endl;
 *       // Prints "10 66 7 23"
 *
 *       return 0;
 *   }
 * @endcode
 *
 * <tt>coveo::linq::operator|()</tt> is used to @ref linq_chaining "chain together" the various
 * @ref linq_operators "LINQ operators", and <tt>coveo::linq::from()</tt> is used as the
 * @ref linq_entry_points "entry point" of the expression, providing the initial sequence on
 * which operators will be applied. The sequence is then transformed by each operator, and
 * the result is passed to the next operator.
 *
 * @section install Installing / requirements
 *
 * The <tt>coveo::linq</tt> library is header-only. Therefore, it is not
 * necessary to "build" it to use. Simply copy the @c lib directory with all
 * its files to a suitable place and add it to your project's include path.
 *
 * The library requires C++11 support. Several compilers meet
 * that requirements, including the following (as tested by
 * <a href="https://travis-ci.org/coveo/linq">Continuous</a>
 * <a href="https://ci.appveyor.com/project/clechasseur/linq/branch/master">Integration</a>):
 *
 * - GCC 5.4.1
 * - Clang 3.4
 * - Visual Studio 2015 Update 3
 *
 * @section help Help / bugs / etc.
 *
 * Need help with the library? Found a bug? Please visit the <tt>coveo::linq</tt>
 * GitHub project page at:
 *
 * &nbsp;&nbsp;&nbsp;&nbsp;https://github.com/coveo/linq
 *
 * There, you can file issues with questions or problems.
 *
 * @copyright 2016-2019, Coveo Solutions Inc.
 *            Distributed under the Apache License, Version 2.0 (see <a href="https://github.com/coveo/linq/blob/master/LICENSE">LICENSE</a>).
 */

/**
 * @defgroup linq LINQ expressions
 *
 * LINQ expressions are comprised of three distinct parts: an @ref linq_entry_points "entry point",
 * one or more @ref linq_operators "operators" and a glue operator to @ref linq_chaining "chain"
 * those elements together. The entry point provides the initial sequence on which to operate and
 * each LINQ operator in turn applies a change to that sequence - be it sorting, filtering, etc.
 *
 * Here is an example with all elements. Here, we filter a sequence of numbers to only keep those
 * above 20, then we order them by numerical value.
 *
 * @code
 *   const int NUMBERS[] = { 42, 23, 11, 66, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMBERS)
 *            | where([](int i) { return i >= 20; })
 *            | order_by([](int i) { return i; });
 *   // seq contains 23, 42, 66
 * @endcode
 *
 * <tt>coveo::linq::from()</tt> is the usual entry point for a LINQ expression, but there are a
 * few others. <tt>coveo::linq::operator|()</tt> is used to chain the LINQ operators together.
 * As for LINQ operators, there are too many of them to list them here. For more information, see:
 *
 * - @ref linq_entry_points
 * - @ref linq_chaining
 * - @ref linq_operators
 */

/**
 * @ingroup linq
 * @defgroup linq_entry_points LINQ expression entry points
 * @brief Functions to start a LINQ expression.
 *
 * An "entry point" is a function that provides the initial sequence on which to apply LINQ operators.
 * It also provides a nice look to the expression, a bit like a database query.
 *
 * The usual entry point for a LINQ expression is <tt>coveo::linq::from()</tt>, which simply returns
 * the sequence passed in. Other entry points generate a sequence for use in the expression.
 */

/**
 * @ingroup linq
 * @defgroup linq_chaining LINQ operator chaining
 * @brief How to chain LINQ operators into an expression.
 *
 * In order to create powerful LINQ expressions, operators need to be chained together so as to be
 * applied in a specific order. To do this, <tt>coveo::linq::operator|()</tt> has been overloaded.
 * Every @ref linq_operators "LINQ operator" returns a function object that is applied on the
 * current sequence by <tt>coveo::linq::operator|()</tt>. By aligning the instances of @c | in the
 * code, LINQ expressions are easy to read:
 *
 * @code
 *   using namespace coveo::linq;
 *   auto seq = from(some_sequence)
 *            | linq_op_1(...)
 *            | linq_op_2(...)
 *            | ...;
 * @endcode
 */

/**
 * @ingroup linq
 * @defgroup linq_operators LINQ operators
 * @brief Information and reference of LINQ operators.
 *
 * A <em>LINQ operator</em> is designed to accept a sequence as input, perform some specific task with it
 * and return a result, which is usually (but not necessarily) another sequence. For example, a LINQ operator
 * might filter the elements of a sequence to remove unwanted ones, or it could concatenate the sequence with
 * another. Some LINQ operators are inspired by database operations, like <tt>coveo::linq::join()</tt>, but
 * not all of them have an equivalent in the database world.
 *
 * Some LINQ operators are identified as @b terminal. This means that they return a result that is not a sequence,
 * but a single value. Because of this, they can only appear at the end of a LINQ expression, since no other
 * operator can be chained after them.
 *
 * In the <tt>coveo::linq</tt> library, LINQ operators are implemented as functions that return a
 * <em>function object</em> implementing the operator logic. The function object is then @e applied
 * to the sequence by <tt>coveo::linq::operator|()</tt>. For more information on this and on how to
 * develop custom LINQ operators, see @ref linq_custom_operators.
 *
 * The <tt>coveo::linq</tt> library includes many different operators for various needs. Most of them
 * are inspired by a corresponding operator in the .NET library, although usage is often different.
 * For a list of operators, see @ref linq_operators_list.
 */

/**
 * @ingroup linq_operators
 * @defgroup linq_operators_list List of built-in LINQ operators
 * @brief Reference for all built-in LINQ operators.
 *
 * This page lists all LINQ operators implemented in the <tt>coveo::linq</tt> library.
 * They are listed alphabetically and grouped by purpose.
 */

/**
 * @ingroup linq_operators
 * @defgroup linq_custom_operators Implementing custom LINQ operators
 * @brief How to design and implement your own LINQ operators.
 *
 * One of the features of the <tt>coveo::linq</tt> library is that LINQ operators are
 * implemented as <em>function objects</em> to be applied to a sequence, instead of
 * member functions (of <tt>coveo::enumerable</tt> for instance.) This makes it easy
 * to extend the library by implementing new operators.
 *
 * To make it easy to apply LINQ operators, the library defines, for each operator, a
 * corresponding function that simply returns the <em>function object</em>. Then,
 * <tt>coveo::linq::operator|()</tt> is used to @e apply the LINQ operator to a sequence.
 * What this means internally is that <tt>operator|()</tt> calls the operator's function
 * object's <tt>operator()()</tt> member function, passing it the sequence to apply it to.
 * The operator's function object must then perform its work and return either a new
 * sequence or perhaps a single value (for a terminal operator).
 *
 * Here is a real-world example of this. Let's say we wanted to implement a new LINQ
 * operator named @c step that steps through a sequence by specific increments. First,
 * we declare the shell of our function object that implements the operator, along with
 * an <tt>operator()()</tt> so that the operator can be applied to a sequence:
 *
 * @code
 *   template<typename = void>
 *   class step_impl
 *   {
 *   private:
 *       std::size_t n_;
 *
 *   public:
 *       explicit step_impl(std::size_t n) : n_(n) { }
 *
 *       template<typename Seq>
 *       auto operator()(Seq&& seq) {
 *           // TODO
 *       }
 *   };
 * @endcode
 *
 * Note that the sequence's type is not specified in the class template itself - when the
 * operator's function object will be created, we won't know the type of sequence yet.
 * Instead, it is specified as a template argument to <tt>operator()()</tt> itself. Also
 * note that the function object's constructor must receive all parameters it needs to
 * work (except for the sequence of course). In our case, we receive the number of steps
 * to use for each invocation.
 *
 * Because our operator must step through elements of the sequence it is applied to, it needs
 * to return a new sequence. Furthermore, that sequence must wrap the source sequence to be
 * able to iterate its elements. One way to do this without implementing complex sequence
 * logic is by using <tt>coveo::enumerable</tt>. This sequence wrapper's only requirement is
 * that we implement a <em>next delegate</em>: a function that, when called, returns a pointer
 * to the next element in the sequence, or @c nullptr when done. <tt>coveo::enumerable</tt>
 * is used extensively in the <tt>coveo::linq</tt> library to implement LINQ operators.
 *
 * Let's add a skeleton for a next delegate to our example:
 *
 * @code
 *   template<typename = void>
 *   class step_impl
 *   {
 *   private:
 *       template<typename Seq>
 *       class next_impl
 *       {
 *       public:
 *           explicit next_impl(Seq&& seq, std::site_t n) {
 *               // TODO
 *           }
 *
 *           auto operator()() {
 *               // TODO
 *           }
 *       };
 *
 *   private:
 *       std::size_t n_;
 *
 *   public:
 *       explicit step_impl(std::size_t n) : n_(n) { }
 *
 *       template<typename Seq>
 *       auto operator()(Seq&& seq)
 *           -> coveo::enumerable<typename coveo::seq_traits<Seq>::value_type>
 *       {
 *           return next_impl<Seq>(std::forward<Seq>(seq), n_);
 *       }
 *   };
 * @endcode
 *
 * Note the use of <tt>coveo::seq_traits</tt> to produce a sequence of the same type
 * of elements as the source sequence. Along with <tt>coveo::seq_element_traits</tt>,
 * these can be used to simplify detection of sequence types in LINQ operator implementations.
 *
 * <tt>coveo::enumerable</tt>'s implementation will @e copy the next delegate every time it
 * is needed - in its copy/move constructors / assignment operators, but also when an iterator
 * is fetched. This is done so that the state of the enumeration can be kept in the next delegate
 * and copied as well. Because of this however, objects to be shared among delegates must be
 * kept in shared memory as well - using <tt>std::shared_ptr</tt> for instance.
 *
 * Let's add storage for our sequence in our example's next delegate:
 *
 * @code
 *   template<typename = void>
 *   class step_impl
 *   {
 *   private:
 *       template<typename Seq>
 *       class next_impl
 *       {
 *       private:
 *           using iterator_type = typename coveo::seq_traits<Seq>::iterator_type;
 *
 *           struct internals {
 *               Seq seq_;
 *               iterator_type end_;
 *               std::size_t n_;
 *
 *               internals(Seq&& seq, std::size_t n)
 *                   : seq_(std::forward<Seq>(seq)),
 *                     end_(std::end(seq_)),
 *                     n_(n) { }
 *               internals(const internals&) = delete;
 *               internals& operator=(const internals&) = delete;
 *           };
 *           using internals_sp = std::shared_ptr<internals>;
 *
 *           internals_sp spint_;
 *           iterator_type it_;
 *       public:
 *           explicit next_impl(Seq&& seq, std::size_t n)
 *               : spint_(std::make_shared<internals>(std::forward<Seq>(seq), n)),
 *                 it_(std::begin(spint_->seq_)) { }
 *
 *           auto operator()() {
 *               // TODO
 *           }
 *       };
 *
 *   private:
 *       std::size_t n_;
 *
 *   public:
 *       explicit step_impl(std::size_t n) : n_(n) { }
 *
 *       template<typename Seq>
 *       auto operator()(Seq&& seq)
 *           -> coveo::enumerable<typename coveo::seq_traits<Seq>::value_type>
 *       {
 *           return next_impl<Seq>(std::forward<Seq>(seq), n_);
 *       }
 *   };
 * @endcode
 *
 * The @c internals class is used to keep all data that is to be shared among next delegates, including
 * the sequence we're wrapping. This allows us to avoid copying the sequence for every delegate copy.
 * The @c internals are kept in a <tt>std::shared_ptr</tt>.
 *
 * Also note that we keep an @c end iterator in the @c internals object, but we keep another iterator
 * in the next delegate directly. This is the @b state of the enumeration: the iterator points to the
 * next element we'll be returning. This iterator is initialized at @c begin initially.
 *
 * Now, all that is left is to implement the operator logic itself: iterating over the sequence, stepping
 * through elements by a specific increment.
 *
 * @code
 *   template<typename = void>
 *   class step_impl
 *   {
 *   private:
 *       template<typename Seq>
 *       class next_impl
 *       {
 *       private:
 *           using iterator_type = typename coveo::seq_traits<Seq>::iterator_type;
 *
 *           struct internals {
 *               Seq seq_;
 *               iterator_type end_;
 *               std::size_t n_;
 *
 *               internals(Seq&& seq, std::size_t n)
 *                   : seq_(std::forward<Seq>(seq)),
 *                     end_(std::end(seq_)),
 *                     n_(n) { }
 *               internals(const internals&) = delete;
 *               internals& operator=(const internals&) = delete;
 *           };
 *           using internals_sp = std::shared_ptr<internals>;
 *
 *           internals_sp spint_;
 *           iterator_type it_;
 *       public:
 *           explicit next_impl(Seq&& seq, std::size_t n)
 *               : spint_(std::make_shared<internals>(std::forward<Seq>(seq), n)),
 *                 it_(std::begin(spint_->seq_)) { }
 *
 *           auto operator()() -> typename coveo::seq_traits<Seq>::pointer {
 *               typename coveo::seq_traits<Seq>::pointer pobj = nullptr;
 *               if (it_ != spint_->end_) {
 *                   // Return this element.
 *                   typename coveo::seq_traits<Seq>::reference robj = *it_;
 *                   pobj = std::addressof(robj);
 *
 *                   // Move to next element by stepping as appropriate.
 *                   for (std::size_t i = 0; it_ != spint_->end_ && i < spint_->n_; ++i, ++it_) {
 *                   }
 *               }
 *               return pobj;
 *           }
 *       };
 *
 *   private:
 *       std::size_t n_;
 *
 *   public:
 *       explicit step_impl(std::size_t n) : n_(n) { }
 *
 *       template<typename Seq>
 *       auto operator()(Seq&& seq)
 *           -> coveo::enumerable<typename coveo::seq_traits<Seq>::value_type>
 *       {
 *           return next_impl<Seq>(std::forward<Seq>(seq), n_);
 *       }
 *   };
 *
 *   template<typename = void>
 *   auto step(std::size_t n) -> step_impl<> {
 *       return step_impl<>(n);
 *   }
 * @endcode
 *
 * Note that we also added a "helper function" that returns our operator's function object's implementation.
 * This function will be the one used to invoke our LINQ operator:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66, 11, 7, 67 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | step(2);
 *   // seq == { 42, 66, 7 };
 * @endcode
 *
 * Implementing a terminal operator is even easier, because there's no need to provide a sequence implementation.
 * Let's say we want to implement a @c prod operator that calculates the product of all sequence elements.
 * Like the <tt>coveo::linq::sum()</tt> operator, it could be done by using a <em>numerical function</em> to
 * get a numerical value for each sequence element. It could be done like this:
 *
 * @code
 *   template<typename F>
 *   class prod_impl
 *   {
 *   private:
 *       const F& num_f_;
 *
 *   public:
 *       explicit prod_impl(const F& num_f) : num_f_(num_f) { }
 *
 *       template<typename Seq>
 *       auto operator()(Seq&& seq) -> typename std::decay<decltype(num_f_(*std::begin(seq)))>::type {
 *           auto it = std::begin(seq);
 *           auto end = std::end(seq);
 *           if (it == end) {
 *               coveo::linq::throw_linq_empty_sequence();
 *           }
 *           auto product = num_f_(*it);
 *           for (++it; it != end; ++it) {
 *               product *= num_f_(*it);
 *           }
 *           return product;
 *       }
 *   };
 *
 *   template<typename F>
 *   auto prod(const F& num_f) -> prod_impl<F> {
 *       return prod_impl<F>(num_f);
 *   }
 * @endcode
 *
 * Now, using the operator is as easy as using <tt>coveo::linq::sum()</tt>:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto product = from(NUMS)
 *                | prod([](int i) { return i; });
 *   // product == 63756
 * @endcode
 *
 * For more examples on how to implement LINQ operators, the easiest way is probably to have
 * a look at the implementation of the operators included in the <tt>coveo::linq</tt> library
 * itself. While the helper functions are documented @ref linq_operators_list "here", it is
 * possible to look at the internal implementations in the file @ref coveo/linq/detail/linq_detail.h :
 *
 * @include coveo/linq/detail/linq_detail.h
 */

#ifndef COVEO_LINQ_H
#define COVEO_LINQ_H

#include "coveo/linq/exception.h"

#include <coveo/enumerable.h>
#include <coveo/linq/detail/linq_detail.h>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>

namespace coveo {
namespace linq {

/**
 * @ingroup linq_entry_points
 * @brief Standard LINQ expression entry point.
 * @headerfile linq.h <coveo/linq/linq.h>
 *
 * Standard entry point for a LINQ expression. Specifies the initial
 * sequence on which the first operator will be applied. After this,
 * use <tt>coveo::linq::operator|()</tt> to chain LINQ operators and apply
 * them in a specific order (see @ref linq_chaining "chaining").
 *
 * Use like this:
 *
 * @code
 *   using namespace coveo::linq;
 *   auto result = from(some_sequence)
 *               | linq_operator(...)
 *               | ...;
 * @endcode
 */
template<typename Seq>
auto from(Seq&& seq) -> decltype(std::forward<Seq>(seq)) {
    return std::forward<Seq>(seq);
}

/**
 * @ingroup linq_entry_points
 * @brief LINQ expression entry point from iterators.
 * @headerfile linq.h <coveo/linq/linq.h>
 *
 * Entry point for a LINQ expression that produces a sequence
 * of elements delimited by two iterators. After this, use
 * <tt>coveo::linq::operator|()</tt> to chain LINQ operators and apply
 * them in a specific order (see @ref linq_chaining "chaining").
 *
 * Use like this:
 *
 * @code
 *   using namespace coveo::linq;
 *   auto result = from_range(something.begin(), something.end())
 *               | linq_operator(...)
 *               | ...;
 * @endcode
 *
 * @see coveo::enumerate_range()
 */
template<typename It>
auto from_range(It ibeg, It iend)
    -> decltype(enumerate_range(std::move(ibeg), std::move(iend)))
{
    return enumerate_range(std::move(ibeg), std::move(iend));
}

/**
 * @ingroup linq_entry_points
 * @brief LINQ expression entry point from range of numbers.
 * @headerfile linq.h <coveo/linq/linq.h>
 *
 * Entry point for a LINQ expression that produces a sequence
 * of incrementing numbers from a starting point. After this, use
 * <tt>coveo::linq::operator|()</tt> to chain LINQ operators and apply
 * them in a specific order (see @ref linq_chaining "chaining").
 *
 * Use like this:
 *
 * @code
 *   using namespace coveo::linq;
 *   auto result = from_int_range(1, 10)    // 1, 2, 3...
 *               | linq_operator(...)
 *               | ...;
 * @endcode
 */
template<typename IntT>
auto from_int_range(IntT first, std::size_t count)
    -> coveo::enumerable<const typename std::decay<IntT>::type>
{
    std::vector<typename std::decay<IntT>::type> vvalues;
    vvalues.reserve(count);
    while (count-- > 0) {
        vvalues.push_back(first++);
    }
    return enumerate_container(std::move(vvalues));
}

/**
 * @ingroup linq_entry_points
 * @brief LINQ expression entry point from a repeated value.
 * @headerfile linq.h <coveo/linq/linq.h>
 *
 * Entry point for a LINQ expression that produces a sequence
 * created by repeating a given value multiple times.. After this, use
 * <tt>coveo::linq::operator|()</tt> to chain LINQ operators and apply
 * them in a specific order (see @ref linq_chaining "chaining").
 *
 * Use like this:
 *
 * @code
 *   using namespace coveo::linq;
 *   auto result = from_repeated(std::string("Life"), 7)    // "Life", "Life", "Life"...
 *               | linq_operator(...)
 *               | ...;
 * @endcode
 */
template<typename T>
auto from_repeated(const T& value, std::size_t count)
    -> coveo::enumerable<const typename std::decay<T>::type>
{
    std::vector<typename std::decay<T>::type> vvalues;
    vvalues.reserve(count);
    while (count-- > 0) {
        vvalues.push_back(value);
    }
    return enumerate_container(std::move(vvalues));
}

/**
 * @ingroup linq_chaining
 * @brief Applies LINQ operators and allows chaining.
 * @headerfile linq.h <coveo/linq/linq.h>
 *
 * Applies a given LINQ operator to the current sequence.
 * Can be used multiple times to chain many operators in a specific order.
 *
 * Use like this:
 *
 * @code
 *   using namespace coveo::linq;
 *   auto result = from(some_sequence)
 *               | linq_op_1(...)
 *               | linq_op_2(...);
 * @endcode
 */
template<typename Seq, typename Op>
auto operator|(Seq&& seq, Op&& op) -> decltype(std::forward<Op>(op)(std::forward<Seq>(seq))) {
    return std::forward<Op>(op)(std::forward<Seq>(seq));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_aggregate aggregate
 * @brief Aggregates values in a sequence to produce a single value.
 *
 * The @c aggregate operator, as its name implies, can be used to @e aggregate all values
 * in a sequence into a single value. To achieve this, it needs an <em>aggregation function</em>
 * that will be called repeatedly to add each element in the sequence to the aggregate.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Aggregate
 */

/**
 * @ingroup linq_op_aggregate
 * @brief Aggregates values using an aggregation function.
 *
 * Aggregates all elements in a sequence by repeatedly calling an <em>aggregation function</em>.
 * The function receives two parameters: the current aggregate value, and the sequence element
 * to add. The function must then add the element to the aggregate and return a new aggregate
 * value. On the first call, the aggregate value is the first sequence element.
 *
 * Does not work on empty sequences.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto agg = from(NUMS)
 *            | aggregate([](int so_far, int i) { return so_far + i; });
 *   // agg == 131
 * @endcode
 *
 * @param agg_f Aggregation function.
 * @return (Once applied) Final aggregate value.
 * @exception coveo::linq::empty_sequence The sequence contains no elements.
 */
template<typename F>
auto aggregate(const F& agg_f)
    -> detail::aggregate_impl_1<F>
{
    return detail::aggregate_impl_1<F>(agg_f);
}

/**
 * @ingroup linq_op_aggregate
 * @brief Aggregates values using an aggregation function, starting with a seed.
 *
 * Aggregates all elements in a sequence by repeatedly calling an <em>aggregation function</em>.
 * The function receives two parameters: the current aggregate value, and the sequence element
 * to add. The function must then add the element to the aggregate and return a new aggregate
 * value. On the first call, the aggregate value is equal to the provided @c seed.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto agg = from(NUMS)
 *            | aggregate(11,
 *                        [](int so_far, int i) { return so_far + i; });
 *   // agg == 142
 * @endcode
 *
 * @param seed Initial aggregate value.
 * @param agg_f Aggregation function.
 * @return (Once applied) Final aggregate value.
 */
template<typename Acc, typename F>
auto aggregate(const Acc& seed, const F& agg_f)
    -> detail::aggregate_impl_2<Acc, F>
{
    return detail::aggregate_impl_2<Acc, F>(seed, agg_f);
}

/**
 * @ingroup linq_op_aggregate
 * @brief Aggregates values using aggregation function, seed and result selector.
 *
 * Aggregates all elements in a sequence by repeatedly calling an <em>aggregation function</em>.
 * The function receives two parameters: the current aggregate value, and the sequence element
 * to add. The function must then add the element to the aggregate and return a new aggregate
 * value. On the first call, the aggregate value is equal to the provided @c seed. Once the
 * final aggregate value is computed, it is passed to a <em>result selector</em> to produce
 * a final value to return.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto agg = from(NUMS)
 *            | aggregate(11,
 *                        [](int so_far, int i) { return so_far + i; },
 *                        [](int so_far) { return so_far / 2; });
 *   // agg == 71
 * @endcode
 *
 * @param seed Initial aggregate value.
 * @param agg_f Aggregation function.
 * @param result_f Function used to produce final result from aggregate.
 * @return (Once applied) Result returned by @c result_f.
 */
template<typename Acc, typename F, typename RF>
auto aggregate(const Acc& seed, const F& agg_f, const RF& result_f)
    -> detail::aggregate_impl_3<Acc, F, RF>
{
    return detail::aggregate_impl_3<Acc, F, RF>(seed, agg_f, result_f);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_all all
 * @brief Checks if all elements in a sequence satisfy a predicate.
 *
 * The @c all operator scans a sequence and validates that all its elements
 * satisfy a given @e predicate.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> All
 */

/**
 * @ingroup linq_op_all
 * @brief Checks elements in a sequence against a predicate.
 *
 * Scans a sequence and calls a @e predicate with each element. The predicate
 * must return @c true if the element satisfies the predicate. The final result
 * will indicate if all elements satisfy the predicate.
 *
 * Works on empty sequences (returns @c true in such a case).
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   bool all_big = from(NUMS)
 *                | all([](int i) { return i >= 10; });
 *   bool all_odd = from(NUMS)
 *                | all([](int i) { return i % 2 != 0; });
 *   // all_big == true
 *   // all_odd == false
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) @c true if all elements in sequence satisfy @c pred.
 */
template<typename Pred>
auto all(const Pred& pred)
    -> detail::all_impl<Pred>
{
    return detail::all_impl<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_any any
 * @brief Checks if a sequence has elements.
 *
 * The @c any operator checks if a sequence has elements, or if it has elements
 * that satisfy a given @e predicate.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Any
 */

/**
 * @ingroup linq_op_any
 * @brief Checks for any element.
 *
 * Checks if a sequence has elements.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> ONE = { 42, 23, 66 };
 *   const std::vector<int> TWO;
 *
 *   using namespace coveo::linq;
 *   bool one_any = from(ONE)
 *                | any();
 *   bool two_any = from(TWO)
 *                | any();
 *   // one_any == true
 *   // two_any == false
 * @endcode
 *
 * @return (Once applied) @c true if sequence has at least one element.
 */
template<typename = void>
auto any()
    -> detail::any_impl_0<>
{
    return detail::any_impl_0<>();
}

/**
 * @ingroup linq_op_any
 * @brief Checks for any element that satisfy a predicate.
 *
 * Checks if a sequence has at least one element that satisfy a @e predicate.
 * The predicate is called with each element and must return @c true if the
 * element satisfy the predicate. The final result indicates if there's at least
 * one element that satisfy the predicate.
 *
 * Works on empty sequences (returns @c false in such a case).
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   bool any_big = from(NUMS)
 *                | any([](int i) { return i >= 90; });
 *   bool any_odd = from(NUMS)
 *                | any([](int i) { return i % 2 != 0; });
 *   // any_big == false
 *   // any_odd == true
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) @c true if at least one element in sequence satisfies @c pred.
 */
template<typename Pred>
auto any(const Pred& pred)
    -> detail::any_impl_1<Pred>
{
    return detail::any_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_average average
 * @brief Computes average of a sequence of values.
 *
 * The @c average operator computes the average of all values in a sequence. To achieve this,
 * it needs a <em>numerical function</em> to extract a numeric value from each sequence element.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Average
 */

/**
 * @ingroup linq_op_average
 * @brief Computes average using numerical function.
 *
 * Computes the average of all elements in a sequence by repeatedly calling a <em>numerical function</em>.
 * The function receives an element as parameter and must return a numerical value for the element.
 * The final result is the average of all such numerical values.
 *
 * Does not work on empty sequences.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::string> STRS = { "42", "23", "66" };
 *
 *   using namespace coveo::linq;
 *   auto avg = from(STRS)
 *            | average([](auto&& s) { return std::stoi(s); });
 *   // avg == 43
 * @endcode
 *
 * @param num_f Function to get numerical value for each element.
 * @return (Once applied) Average of all extracted numerical values.
 * @exception coveo::linq::empty_sequence The sequence contains no elements.
 */
template<typename F>
auto average(const F& num_f)
    -> detail::average_impl<F>
{
    return detail::average_impl<F>(num_f);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_cast cast
 * @brief Casts sequence elements to another type.
 *
 * The @c cast operator modifies a sequence by casting all its element to another type.
 *
 * <b>.NET equivalent:</b> Cast
 */

/**
 * @ingroup linq_op_cast
 * @brief Casts sequence elements to another type.
 *
 * Casts each element in a sequence to another type and returns a new sequence
 * of all such modified elements. The elements are cast using @c static_cast.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto dbls = from(NUMS)
 *             | cast<double>();
 *   // dbls == { 42.0, 23.0, 66.0 }
 * @endcode
 *
 * @tparam U New type to cast elements to.
 * @return (Once applied) Sequence of cast elements.
 */
template<typename U>
auto cast()
    -> detail::select_impl<detail::indexless_selector_proxy<detail::cast_selector<U>>>
{
    return detail::select_impl<detail::indexless_selector_proxy<detail::cast_selector<U>>>(
        detail::indexless_selector_proxy<detail::cast_selector<U>>(detail::cast_selector<U>()));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_concat concat
 * @brief Concatenate two sequences.
 *
 * The @c concat operator concatenates two sequences, producing a new sequence
 * containing all elements from both source sequences.
 *
 * <b>.NET equivalent:</b> Concat
 */

/**
 * @ingroup linq_op_concat
 * @brief Concatenates two sequences.
 *
 * Concatenates two sequences, producing a sequence that contains all elements
 * from both source sequences.
 *
 * For this to work, both source sequences must contain compatible elements.
 *
 * The resulting sequence's elements will be @c const if at least one source
 * sequence's elements are @c const.
 *
 * Use like this:
 *
 * @code
 *   const int ONE[] = { 42, 23, 66 };
 *   const int TWO[] = { 67, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(ONE)
 *            | concat(TWO);
 *   // seq == { 42, 23, 66, 67, 11, 7 }
 * @endcode
 *
 * @param seq2 Second sequence to concatenate.
 * @return (Once applied) Concatenation of both sequences.
 */
template<typename Seq2>
auto concat(Seq2&& seq2)
    -> detail::concat_impl<Seq2>
{
    return detail::concat_impl<Seq2>(std::forward<Seq2>(seq2));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_contains contains
 * @brief Looks for an element in a sequence.
 *
 * The @c contains operator determines if a sequence contains a specific element.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Contains
 */

/**
 * @ingroup linq_op_contains
 * @brief Look for element in a sequence.
 *
 * Scans a sequence, looking for the provided element. Elements are compared
 * using <tt>operator==()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto has_42 = from(NUMS)
 *               | contains(42);
 *   auto has_67 = from(NUMS)
 *               | contains(67);
 *   // has_42 == true
 *   // has_67 == false
 * @endcode
 *
 * @param obj Element to look for.
 * @return (Once applied) @c true if @c obj was found in sequence.
 */
template<typename T>
auto contains(const T& obj)
    -> detail::contains_impl_1<T>
{
    return detail::contains_impl_1<T>(obj);
}

/**
 * @ingroup linq_op_contains
 * @brief Look for element in a sequence using a predicate.
 *
 * Scans a sequence, looking for the provided element. Elements are compared
 * using the provided predicate.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   auto fuzzy_equal = [](int i, int j) { return std::abs(i - j) <= 2; };
 *
 *   using namespace coveo::linq;
 *   auto has_67 = from(NUMS)
 *               | contains(67, fuzzy_equal);
 *   auto has_11 = from(NUMS)
 *               | contains(11, fuzzy_equal);
 *   // has_67 == true
 *   // has_11 == false
 * @endcode
 *
 * @param obj Element to look for.
 * @param pred Predicate used to compare the elements. Always receives
 *             @c obj as its second argument.
 * @return (Once applied) @c true if @c obj was found in sequence.
 */
template<typename T, typename Pred>
auto contains(const T& obj, const Pred& pred)
    -> detail::contains_impl_2<T, Pred>
{
    return detail::contains_impl_2<T, Pred>(obj, pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_count count
 * @brief Counts elements in a sequence.
 *
 * The @c count operator computes the number of elements in a sequence,
 * or the number of elements satisfying a given predicate.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Count
 */

/**
 * @ingroup linq_op_count
 * @brief Counts elements in sequence.
 *
 * Computes the number of elements in a sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto num = from(NUMS)
 *            | count();
 *   // num == 3
 * @endcode
 *
 * @return (Once applied) Number of elements in sequence.
 */
template<typename = void>
auto count()
    -> detail::count_impl_0<>
{
    return detail::count_impl_0<>();
}

/**
 * @ingroup linq_op_count
 * @brief Counts elements in sequence satisfying a predicate.
 *
 * Computes the number of elements in a sequence that satisfy the given predicate.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto num = from(NUMS)
 *            | count([](int i) { return i % 2 == 0; });
 *   // num == 2
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Number of elements in sequence for which
 *         @c pred has returned @c true.
 */
template<typename Pred>
auto count(const Pred& pred)
    -> detail::count_impl_1<Pred>
{
    return detail::count_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_def_empty default_if_empty
 * @brief Ensures a sequence has at least one element.
 *
 * The @c default_if_empty operator either returns the source sequence or,
 * if that sequence is empty, a new sequence with a single, default element.
 *
 * <b>.NET equivalent:</b> DefaultIfEmpty
 */

/**
 * @ingroup linq_op_def_empty
 * @brief Ensures sequence has at least one element.
 *
 * Either returns the source sequence or, if that sequence is empty,
 * a new sequence containing a single, default element.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> THREE = { 42, 23, 66 };
 *   const std::vector<int> EMPTY;
 *
 *   using namespace coveo::linq;
 *   auto seq1 = from(THREE)
 *             | default_if_empty();
 *   auto seq2 = from(EMPTY)
 *             | default_if_empty();
 *   // seq1 == { 42, 23, 66 }
 *   // seq2 == { 0 }
 * @endcode
 *
 * @return (Once applied) Source sequence, or sequence with
 *         single, default element.
 */
template<typename = void>
auto default_if_empty()
    -> detail::default_if_empty_impl_0<>
{
    return detail::default_if_empty_impl_0<>();
}

/**
 * @ingroup linq_op_def_empty
 * @brief Ensures sequence has at least one element, specifying said element.
 *
 * Either returns the source sequence or, if that sequence is empty,
 * a new sequence containing the provided default element.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> THREE = { 42, 23, 66 };
 *   const std::vector<int> EMPTY;
 *
 *   using namespace coveo::linq;
 *   auto seq1 = from(THREE)
 *             | default_if_empty(11);
 *   auto seq2 = from(EMPTY)
 *             | default_if_empty(11);
 *   // seq1 == { 42, 23, 66 }
 *   // seq2 == { 11 }
 * @endcode
 *
 * @param obj Default element to use if sequence is empty.
 * @return (Once applied) Source sequence, or sequence with
 *         single element @c obj.
 */
template<typename T>
auto default_if_empty(const T& obj)
    -> detail::default_if_empty_impl_1<T>
{
    return detail::default_if_empty_impl_1<T>(obj);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_distinct distinct
 * @brief Filters out duplicate elements in a sequence.
 *
 * The @c distinct operator filters out duplicate elements in a sequence.
 * Unique elements are returned in the same order they appear in the source sequence.
 *
 * <b>.NET equivalent:</b> Distinct
 */

/**
 * @ingroup linq_op_distinct
 * @brief Filters out duplicate elements in sequence.
 *
 * Filters out duplicate elements in a sequence, returning the unique
 * elements in the same order they appear in the source sequence.
 *
 * To filter out duplicates, elements are sorted using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 11, 66, 11, 42, 7, 66, 67 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | distinct();
 *   // seq = { 42, 23, 11, 66, 7, 67 }
 * @endcode
 *
 * @return (Once applied) Sequence containing all unique elements
 *         from source sequence.
 */
template<typename = void>
auto distinct()
    -> detail::distinct_impl<detail::less<>>
{
    return detail::distinct_impl<detail::less<>>(detail::less<>());
}

/**
 * @ingroup linq_op_distinct
 * @brief Filters out duplicate elements in sequence using predicate.
 *
 * Filters out duplicate elements in a sequence, returning the unique
 * elements in the same order they appear in the source sequence.
 *
 * To filter out duplicates, the provided predicate is used to sort
 * the elements. The predicate must provide a strict ordering of the
 * elements, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 11, 66, 11, 42, 7, 66, 67 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | distinct([](int i, int j) { return i > j; });
 *   // seq = { 42, 23, 11, 66, 7, 67 }
 * @endcode
 *
 * @param pred Predicate used to order the elements in order to
 *             remove the duplicates.
 * @return (Once applied) Sequence containing all unique elements
 *         from source sequence.
 */
template<typename Pred>
auto distinct(Pred&& pred)
    -> detail::distinct_impl<Pred>
{
    return detail::distinct_impl<Pred>(std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_elem_at element_at
 * @brief Returns nth element in a sequence.
 *
 * The @c element_at operator returns the nth element in the sequence.
 * If the sequence does not have enough elements, an exception is thrown.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> ElementAt
 */

/**
 * @ingroup linq_op_elem_at
 * @brief Returns nth element in sequence.
 *
 * Returns the <tt>n</tt>th element in a sequence. If the sequence
 * does not have enough elements, <tt>coveo::linq::out_of_range</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto second = from(NUMS)
 *               | element_at(1);
 *   // second == 23
 *   // This throws an exception:
 *   // auto fourth = from(NUMS)
 *   //             | element_at(3);
 * @endcode
 *
 * @param n 0-based index of element to return.
 * @return (Once applied) <tt>n</tt>th element in sequence.
 * @exception coveo::linq::out_of_range The sequence does not have enough elements.
 */
template<typename = void>
auto element_at(std::size_t n)
    -> detail::element_at_impl<>
{
    return detail::element_at_impl<>(n);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_elem_at_or_def element_at_or_default
 * @brief Returns nth element in a sequence, or a default value.
 *
 * The @c element_at operator returns the nth element in the sequence.
 * If the sequence does not have enough elements, a default value is returned instead.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> ElementAtOrDefault
 */

/**
 * @ingroup linq_op_elem_at_or_def
 * @brief Returns nth element in sequence or default value.
 *
 * Returns the <tt>n</tt>th element in a sequence. If the sequence
 * does not have enough elements, a <em>default-initialized</em>
 * value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto second = from(NUMS)
 *               | element_at(1);
 *   auto fourth = from(NUMS)
 *               | element_at(3);
 *   // second == 23
 *   // fourth == 0
 * @endcode
 *
 * @param n 0-based index of element to return.
 * @return (Once applied) <tt>n</tt>th element in sequence or,
 *         if sequence does not have enough elements, a default value.
 */
template<typename = void>
auto element_at_or_default(std::size_t n)
    -> detail::element_at_or_default_impl<>
{
    return detail::element_at_or_default_impl<>(n);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_except except
 * @brief Performs a set difference between two sequences.
 *
 * The @c except operator returns all elements in the first sequence
 * that are not also found in the second sequence (essentially a set
 * difference). The elements are returned in the order that they appear
 * in the first sequence.
 *
 * <b>.NET equivalent:</b> Except
 */

/**
 * @ingroup linq_op_except
 * @brief Performs set difference between two sequences.
 *
 * Returns a sequence containing all elements from the first source sequence
 * that are not also in the second source sequence (essentially a set difference).
 * Elements are returned in the order that they appear in the first sequence.
 *
 * To filter out elements, elements are sorted using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int YES[] = { 42, 23, 66, 11, 7, 67 };
 *   const int NO[]  = { 10, 7, 60, 42, 43, 50 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(YES)
 *            | except(NO);
 *   // seq = { 23, 66, 11, 67 }
 * @endcode
 *
 * @param seq2 Second source sequence, containing the elements
 *             to filter out.
 * @return (Once applied) Sequence containing elements from first
 *         source sequence that are not in @c seq2.
 */
template<typename Seq2>
auto except(Seq2&& seq2)
    -> detail::except_impl<Seq2, detail::less<>>
{
    return detail::except_impl<Seq2, detail::less<>>(std::forward<Seq2>(seq2), detail::less<>());
}

/**
 * @ingroup linq_op_except
 * @brief Performs set difference between two sequences using predicate.
 *
 * Returns a sequence containing all elements from the first source sequence
 * that are not also in the second source sequence (essentially a set difference).
 * Elements are returned in the order that they appear in the first sequence.
 *
 * To filter out elements, the provided predicate is used to sort the elements.
 * The predicate must provide a strict ordering of the elements, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int YES[] = { 42, 23, 66, 11, 7, 67 };
 *   const int NO[]  = { 10, 7, 60, 42, 43, 50 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(YES)
 *            | except(NO, [](int i, int j) { return i > j; });
 *   // seq = { 23, 66, 11, 67 }
 * @endcode
 *
 * @param seq2 Second source sequence, containing the elements
 *             to filter out.
 * @param pred Predicate used to order elements to filter out.
 * @return (Once applied) Sequence containing elements from first
 *         source sequence that are not in @c seq2.
 */
template<typename Seq2, typename Pred>
auto except(Seq2&& seq2, Pred&& pred)
    -> detail::except_impl<Seq2, Pred>
{
    return detail::except_impl<Seq2, Pred>(std::forward<Seq2>(seq2), std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_first first
 * @brief Returns first element in a sequence.
 *
 * The @c first operator returns the first element in a sequence,
 * or the first element to satisfy a predicate. If the sequence does
 * not have such an element, an exception is thrown.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> First
 */

/**
 * @ingroup linq_op_first
 * @brief Returns first element in sequence.
 *
 * Returns the first element in a sequence. If the sequence
 * does not have elements, <tt>coveo::linq::empty_sequence</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto fir1 = from(YES)
 *             | first();
 *   // fir1 == 42
 *   // This throws an exception:
 *   // auto fir2 = from(NAY)
 *   //           | first();
 * @endcode
 *
 * @return (Once applied) First element in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 */
template<typename = void>
auto first()
    -> detail::first_impl_0<>
{
    return detail::first_impl_0<>();
}

/**
 * @ingroup linq_op_first
 * @brief Returns first element in sequence that satisfy predicate.
 *
 * Returns the first element in a sequence for which the given
 * predicate returns @c true. If the sequence does not have elements,
 * <tt>coveo::linq::empty_sequence</tt> is thrown; if the sequence does
 * not contain an element that satisfy the predicate,
 * <tt>coveo::linq::out_of_range</tt> is thrown.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto odd = from(NUMS)
 *            | first([](int i) { return i % 2 != 0; });
 *   // odd == 23
 *   // This throws an exception:
 *   // auto big = from(NUMS)
 *   //          | first([](int i) { return  i >= 90; });
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) First element in sequence for which @c pred returns @c true.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 * @exception coveo::linq::out_of_range The sequence has no element that satisfy @c pred.
 */
template<typename Pred>
auto first(const Pred& pred)
    -> detail::first_impl_1<Pred>
{
    return detail::first_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_first_or_def first_or_default
 * @brief Returns first element in a sequence, or a default value.
 *
 * The @c first_or_default operator returns the first element in a sequence,
 * or the first element to satisfy a predicate. If the sequence does
 * not have such an element, a default value is returned.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> FirstOrDefault
 */

/**
 * @ingroup linq_op_first_or_def
 * @brief Returns first element in sequence, or default value.
 *
 * Returns the first element in a sequence. If the sequence
 * does not have elements, a <em>default-initialized</em>
 * value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto fir1 = from(YES)
 *             | first();
 *   auto fir2 = from(NAY)
 *             | first();
 *   // fir1 == 42
 *   // fir2 == 0
 * @endcode
 *
 * @return (Once applied) First element in sequence, or a default value
 *         if sequence does not have elements.
 */
template<typename = void>
auto first_or_default()
    -> detail::first_or_default_impl_0<>
{
    return detail::first_or_default_impl_0<>();
}

/**
 * @ingroup linq_op_first_or_def
 * @brief Returns first element in sequence that satisfy predicate, or default value.
 *
 * Returns the first element in a sequence for which the given
 * predicate returns @c true. If the sequence does not have elements
 * or does not contain an element that satisfy the predicate, a
 * <em>default-initialized</em> value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto odd = from(NUMS)
 *            | first([](int i) { return i % 2 != 0; });
 *   auto big = from(NUMS)
 *            | first([](int i) { return  i >= 90; });
 *   // odd == 23
 *   // big == 0
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) First element in sequence for which @c pred returns @c true
 *         or, if no such element exists in sequence, a default value.
 */
template<typename Pred>
auto first_or_default(const Pred& pred)
    -> detail::first_or_default_impl_1<Pred>
{
    return detail::first_or_default_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_group_by group_by / group_values_by / group_by_and_fold / group_values_by_and_fold
 * @brief Groups elements in a sequence according to their keys.
 *
 * The @c group_by operator (and its siblings) group elements in a sequence according to their keys.
 * Keys are extracted from elements using a <em>key selector</em>. Variants of the operator can also
 * extract values from elements using a <em>value selector</em>, or modify the resulting sequence using
 * a <em>result selector</em>.
 *
 * <b>.NET equivalent:</b> GroupBy
 */

/**
 * @ingroup linq_op_group_by
 * @brief Groups elements in sequence according to their keys.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em>. Then, creates groups of elements
 * that have a common key. The result is a sequence of <tt>pair</tt>s
 * whose @c first element is a key and whose @c second element is a sequence
 * of elements matching that key. The groups are returned in ascending order
 * of key, as determined by <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto groups = from(DATA)
 *               | group_by([](std::pair<int, std::string> p) { return p.first; });
 *   auto it = std::begin(groups);
 *   auto group1 = *it++;
 *   auto group2 = *it++;
 *   auto group3 = *it++;
 *   // group1.first == 23, group1.second == { { 23, "Hangar" }, { 23, "Jeep" } }
 *   // group2.first == 42, group2.second == { { 42, "Life" }, { 42, "Universe" } }
 *   // group3.first == 66, group3.second == { { 66, "Route" } }
 *   // it == std::end(groups)
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @return (Once applied) Sequence of <tt>pair</tt>s whose @c first element is a
 *         key and whose @c second element is a sequence of matching elements.
 */
template<typename KeySelector>
auto group_by(KeySelector&& key_sel)
    -> detail::group_by_impl<KeySelector,
                             detail::identity<>,
                             detail::pair_of<>,
                             detail::less<>>
{
    return detail::group_by_impl<KeySelector,
                                 detail::identity<>,
                                 detail::pair_of<>,
                                 detail::less<>>(std::forward<KeySelector>(key_sel),
                                                 detail::identity<>(),
                                                 detail::pair_of<>(),
                                                 detail::less<>());
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups elements in sequence according to their keys using predicate.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em>. Then, creates groups of elements
 * that have a common key. The result is a sequence of <tt>pair</tt>s
 * whose @c first element is a key and whose @c second element is a sequence
 * of elements matching that key. The groups are returned in order of key,
 * as determined by the provided predicate. The predicate must provide a
 * strict ordering of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto groups = from(DATA)
 *               | group_by([](std::pair<int, std::string> p) { return p.first; },
 *                          [](int i, int j) { return i > j; });
 *   auto it = std::begin(groups);
 *   auto group1 = *it++;
 *   auto group2 = *it++;
 *   auto group3 = *it++;
 *   // group1.first == 66, group1.second == { { 66, "Route" } }
 *   // group2.first == 42, group2.second == { { 42, "Life" }, { 42, "Universe" } }
 *   // group3.first == 23, group3.second == { { 23, "Hangar" }, { 23, "Jeep" } }
 *   // it == std::end(groups)
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of <tt>pair</tt>s whose @c first element is a
 *         key and whose @c second element is a sequence of matching elements.
 */
template<typename KeySelector,
         typename Pred>
auto group_by(KeySelector&& key_sel,
              Pred&& pred)
    -> detail::group_by_impl<KeySelector,
                             detail::identity<>,
                             detail::pair_of<>,
                             Pred>
{
    return detail::group_by_impl<KeySelector,
                                 detail::identity<>,
                                 detail::pair_of<>,
                                 Pred>(std::forward<KeySelector>(key_sel),
                                       detail::identity<>(),
                                       detail::pair_of<>(),
                                       std::forward<Pred>(pred));
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups values in sequence according to their keys.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em> and a value using the provided
 * <em>value selector</em>. Then, creates groups of values that have
 * a common key. The result is a sequence of <tt>pair</tt>s whose @c first
 * element is a key and whose @c second element is a sequence of values
 * matching that key. The groups are returned in ascending order of key,
 * as determined by <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto groups = from(DATA)
 *               | group_values_by([](std::pair<int, std::string> p) { return p.first; },
 *                                 [](std::pair<int, std::string> p) { return p.second; });
 *   auto it = std::begin(groups);
 *   auto group1 = *it++;
 *   auto group2 = *it++;
 *   auto group3 = *it++;
 *   // group1.first == 23, group1.second == { "Hangar", "Jeep" }
 *   // group2.first == 42, group2.second == { "Life", "Universe" }
 *   // group3.first == 66, group3.second == { "Route" }
 *   // it == std::end(groups)
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param value_sel Value selector, used to extract a value for a sequence element.
 * @return (Once applied) Sequence of <tt>pair</tt>s whose @c first element is a
 *         key and whose @c second element is a sequence of matching values.
 */
template<typename KeySelector,
         typename ValueSelector>
auto group_values_by(KeySelector&& key_sel,
                     ValueSelector&& value_sel)
    -> detail::group_by_impl<KeySelector,
                             ValueSelector,
                             detail::pair_of<>,
                             detail::less<>>
{
    return detail::group_by_impl<KeySelector,
                                 ValueSelector,
                                 detail::pair_of<>,
                                 detail::less<>>(std::forward<KeySelector>(key_sel),
                                                 std::forward<ValueSelector>(value_sel),
                                                 detail::pair_of<>(),
                                                 detail::less<>());
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups values in sequence according to their keys using predicate.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em> and a value using the provided
 * <em>value selector</em>. Then, creates groups of values that have
 * a common key. The result is a sequence of <tt>pair</tt>s whose @c first
 * element is a key and whose @c second element is a sequence of values
 * matching that key. The groups are returned in order of key, as determined
 * by the provided predicate. The predicate must provide a strict ordering
 * of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto groups = from(DATA)
 *               | group_values_by([](std::pair<int, std::string> p) { return p.first; },
 *                                 [](std::pair<int, std::string> p) { return p.second; },
 *                                 [](int i, int j) { return i > j; });
 *   auto it = std::begin(groups);
 *   auto group1 = *it++;
 *   auto group2 = *it++;
 *   auto group3 = *it++;
 *   // group1.first == 66, group1.second == { "Route" }
 *   // group2.first == 42, group2.second == { "Life", "Universe" }
 *   // group3.first == 23, group3.second == { "Hangar", "Jeep" }
 *   // it == std::end(groups)
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param value_sel Value selector, used to extract a value for a sequence element.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of <tt>pair</tt>s whose @c first element is a
 *         key and whose @c second element is a sequence of matching values.
 */
template<typename KeySelector,
         typename ValueSelector,
         typename Pred>
auto group_values_by(KeySelector&& key_sel,
                     ValueSelector&& value_sel,
                     Pred&& pred)
    -> detail::group_by_impl<KeySelector,
                             ValueSelector,
                             detail::pair_of<>,
                             Pred>
{
    return detail::group_by_impl<KeySelector,
                                 ValueSelector,
                                 detail::pair_of<>,
                                 Pred>(std::forward<KeySelector>(key_sel),
                                       std::forward<ValueSelector>(value_sel),
                                       detail::pair_of<>(),
                                       std::forward<Pred>(pred));
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups elements in sequence according to their keys then folds the results.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em>. Then, creates groups of elements
 * that have a common key and uses the provided <em>result selector</em>
 * to convert the groups. The result selector is called with two arguments:
 * a key, and a sequence of elements matching that key. The final result is
 * a sequence of the values returned by the result selector. The result
 * selector is called in ascending order of key, as determined by
 * <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto res = from(DATA)
 *            | group_by_and_fold([](std::pair<int, std::string> p) { return p.first; },
 *                                [](int k, coveo::enumerable<const std::pair<int, std::string>> e) { k + return e.size(); });
 *   // res == { 25, 44, 67 }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param result_sel Result selector, used to fold groups into final results.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename KeySelector,
         typename ResultSelector>
auto group_by_and_fold(KeySelector&& key_sel,
                       ResultSelector&& result_sel)
    -> detail::group_by_impl<KeySelector,
                             detail::identity<>,
                             ResultSelector,
                             detail::less<>>
{
    return detail::group_by_impl<KeySelector,
                                 detail::identity<>,
                                 ResultSelector,
                                 detail::less<>>(std::forward<KeySelector>(key_sel),
                                                 detail::identity<>(),
                                                 std::forward<ResultSelector>(result_sel),
                                                 detail::less<>());
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups elements in sequence according to their keys using predicate, then folds the results.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em>. Then, creates groups of elements
 * that have a common key and uses the provided <em>result selector</em>
 * to convert the groups. The result selector is called with two arguments:
 * a key, and a sequence of elements matching that key. The final result is
 * a sequence of the values returned by the result selector. The result
 * selector is called in order of key, as determined by the provided
 * predicate. The predicate must provide a strict ordering of the keys,
 * like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto res = from(DATA)
 *            | group_by_and_fold([](std::pair<int, std::string> p) { return p.first; },
 *                                [](int k, coveo::enumerable<const std::pair<int, std::string>> e) { k + return e.size(); },
 *                                [](int i, int j) { return i > j; });
 *   // res == { 67, 44, 25 }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param result_sel Result selector, used to fold groups into final results.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename KeySelector,
         typename ResultSelector,
         typename Pred>
auto group_by_and_fold(KeySelector&& key_sel,
                       ResultSelector&& result_sel,
                       Pred&& pred)
    -> detail::group_by_impl<KeySelector,
                             detail::identity<>,
                             ResultSelector,
                             Pred>
{
    return detail::group_by_impl<KeySelector,
                                 detail::identity<>,
                                 ResultSelector,
                                 Pred>(std::forward<KeySelector>(key_sel),
                                       detail::identity<>(),
                                       std::forward<ResultSelector>(result_sel),
                                       std::forward<Pred>(pred));
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups values in sequence according to their keys then folds the results.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em> and a value using the provided
 * <em>value selector</em>. Then, creates groups of values that have
 * a common key and uses the provided <em>result selector</em> to convert
 * the groups. The result selector is called with two arguments: a key,
 * and a sequence of values matching that key. The final result is a
 * sequence of the values returned by the result selector. The result
 * selector is called in ascending order of key, as determined by
 * <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto res = from(DATA)
 *            | group_values_by_and_fold([](std::pair<int, std::string> p) { return p.first; },
 *                                       [](std::pair<int, std::string> p) { return p.second; },
 *                                       [](int k, coveo::enumerable<const std::string> e) { k + return e.begin()->size(); });
 *   // res == { 29, 46, 71 }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param value_sel Value selector, used to extract a value for a sequence element.
 * @param result_sel Result selector, used to fold groups into final results.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename KeySelector,
         typename ValueSelector,
         typename ResultSelector>
auto group_values_by_and_fold(KeySelector&& key_sel,
                              ValueSelector&& value_sel,
                              ResultSelector&& result_sel)
    -> detail::group_by_impl<KeySelector,
                             ValueSelector,
                             ResultSelector,
                             detail::less<>>
{
    return detail::group_by_impl<KeySelector,
                                 ValueSelector,
                                 ResultSelector,
                                 detail::less<>>(std::forward<KeySelector>(key_sel),
                                                 std::forward<ValueSelector>(value_sel),
                                                 std::forward<ResultSelector>(result_sel),
                                                 detail::less<>());
}

/**
 * @ingroup linq_op_group_by
 * @brief Groups values in sequence according to their keys using predicate, then folds the results.
 *
 * Scans the input sequence and, for each element, fetches a key using
 * the provided <em>key selector</em> and a value using the provided
 * <em>value selector</em>. Then, creates groups of values that have
 * a common key and uses the provided <em>result selector</em> to convert
 * the groups. The result selector is called with two arguments: a key,
 * and a sequence of values matching that key. The final result is a
 * sequence of the values returned by the result selector. The result
 * selector is called in order of key, as determined by the provided
 * predicate. The predicate must provide a strict ordering of the keys,
 * like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::pair<int, std::string>> DATA = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 42, "Universe" },
 *       { 66, "Route" },
 *       { 23, "Jeep" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto res = from(DATA)
 *            | group_values_by_and_fold([](std::pair<int, std::string> p) { return p.first; },
 *                                       [](std::pair<int, std::string> p) { return p.second; },
 *                                       [](int k, coveo::enumerable<const std::string> e) { k + return e.begin()->size(); },
 *                                       [](int i, int j) { return i > j; });
 *   // res == { 71, 46, 29 }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param value_sel Value selector, used to extract a value for a sequence element.
 * @param result_sel Result selector, used to fold groups into final results.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename KeySelector,
         typename ValueSelector,
         typename ResultSelector,
         typename Pred>
auto group_values_by_and_fold(KeySelector&& key_sel,
                              ValueSelector&& value_sel,
                              ResultSelector&& result_sel,
                              Pred&& pred)
    -> detail::group_by_impl<KeySelector,
                             ValueSelector,
                             ResultSelector,
                             Pred>
{
    return detail::group_by_impl<KeySelector,
                                 ValueSelector,
                                 ResultSelector,
                                 Pred>(std::forward<KeySelector>(key_sel),
                                       std::forward<ValueSelector>(value_sel),
                                       std::forward<ResultSelector>(result_sel),
                                       std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_group_join group_join
 * @brief Joins and groups elements in two sequences according to their keys.
 *
 * The @c group_join operator scans two sequences: an <em>outer sequence</em> and an
 * <em>inner sequence</em>. For each element in the sequences, it extracts a key using
 * some <em>key selectors</em>. Then, it creates groups of elements from the inner sequence
 * and joins them to elements in the outer sequence with matching keys. Finally, a
 * <em>result selector</em> is used to fold the groups into the final results.
 *
 * <b>.NET equivalent:</b> GroupJoin
 */

/**
 * @ingroup linq_op_group_join
 * @brief Joins and groups elements in two sequences according to their keys.
 *
 * Extracts keys for the elements of an <em>outer sequence</em> (the one on which
 * this operator is applied, e.g. the one passed to <tt>coveo::linq::from()</tt>)
 * and the provided <em>inner sequence</em> using the provided <em>key selectors</em>.
 * Next, creates groups of elements from the inner sequence with keys matching that
 * of the elements in the outer sequence. Finally, the provided <em>result selector</em>
 * is used to convert the groups into the final results. The result selector is called
 * with two arguments: an element from the outer sequence and a group of elements from
 * the inner sequence that share the same key.
 *
 * In order to match the keys, they are compared using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   using person_data = std::pair<int, std::string>;
 *   using person_messages = std::pair<int, std::string>;
 *
 *   const std::vector<person_data> PERSONS = {
 *       { 1, "John Doe" },
 *       { 2, "Jane Smith" },
 *   };
 *   const std::vector<person_messages> MESSAGES = {
 *       { 1, "This is a test message" },
 *       { 2, "Hello Jane!" },
 *       { 1, "Doctors hate him!" },
 *       { 1, "Welcome to the company" },
 *       { 2, "Todo before we leave for vacation" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(PERSONS)
 *            | group_join(MESSAGES,
 *                         [](const person_data& pd) { return pd.first; },
 *                         [](const person_messages& pm) { return pm.first; }
 *                         [](const person_data& pd, const coveo::enumerable<const person_messages>& e) {
 *                             std::string res = pd.second + ": ";
 *                             for (auto&& pm : e) {
 *                                 res += pm.second + ", ";
 *                             }
 *                             return res;
 *                         });
 *   // seq == {
 *   //            "John Doe: This is a test message, Doctors hate him!, Welcome to the company, ",
 *   //            "Jane Smith: Hello Jane!, Todo before we leave for vacation, "
 *   //        }
 * @endcode
 *
 * @param inner_seq Inner sequence to scan to create groups.
 * @param outer_key_sel Selector to get keys for elements in the outer sequence.
 * @param inner_key_sel Selector to get keys for elements in the inner sequence.
 * @param result_sel Result selector used to produce final results.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename InnerSeq,
         typename OuterKeySelector,
         typename InnerKeySelector,
         typename ResultSelector>
auto group_join(InnerSeq&& inner_seq,
                OuterKeySelector&& outer_key_sel,
                InnerKeySelector&& inner_key_sel,
                ResultSelector&& result_sel)
    -> detail::group_join_impl<InnerSeq,
                               OuterKeySelector,
                               InnerKeySelector,
                               ResultSelector,
                               detail::less<>>
{
    return detail::group_join_impl<InnerSeq,
                                   OuterKeySelector,
                                   InnerKeySelector,
                                   ResultSelector,
                                   detail::less<>>(std::forward<InnerSeq>(inner_seq),
                                                   std::forward<OuterKeySelector>(outer_key_sel),
                                                   std::forward<InnerKeySelector>(inner_key_sel),
                                                   std::forward<ResultSelector>(result_sel),
                                                   detail::less<>());
}

/**
 * @ingroup linq_op_group_join
 * @brief Joins and groups elements in two sequences according to their keys using predicate.
 *
 * Extracts keys for the elements of an <em>outer sequence</em> (the one on which
 * this operator is applied, e.g. the one passed to <tt>coveo::linq::from()</tt>)
 * and the provided <em>inner sequence</em> using the provided <em>key selectors</em>.
 * Next, creates groups of elements from the inner sequence with keys matching that
 * of the elements in the outer sequence. Finally, the provided <em>result selector</em>
 * is used to convert the groups into the final results. The result selector is called
 * with two arguments: an element from the outer sequence and a group of elements from
 * the inner sequence that share the same key.
 *
 * In order to match the keys, they are compared using the provided predicate. The
 * predicate must provide a strict ordering of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   using person_data = std::pair<int, std::string>;
 *   using person_messages = std::pair<int, std::string>;
 *
 *   const std::vector<person_data> PERSONS = {
 *       { 1, "John Doe" },
 *       { 2, "Jane Smith" },
 *   };
 *   const std::vector<person_messages> MESSAGES = {
 *       { 1, "This is a test message" },
 *       { 2, "Hello Jane!" },
 *       { 1, "Doctors hate him!" },
 *       { 1, "Welcome to the company" },
 *       { 2, "Todo before we leave for vacation" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(PERSONS)
 *            | group_join(MESSAGES,
 *                         [](const person_data& pd) { return pd.first; },
 *                         [](const person_messages& pm) { return pm.first; }
 *                         [](const person_data& pd, const coveo::enumerable<const person_messages>& e) {
 *                             std::string res = pd.second + ": ";
 *                             for (auto&& pm : e) {
 *                                 res += pm.second + ", ";
 *                             }
 *                             return res;
 *                         },
 *                         [](int i, int j) { return i > j; });
 *   // seq == {
 *   //            "John Doe: This is a test message, Doctors hate him!, Welcome to the company, ",
 *   //            "Jane Smith: Hello Jane!, Todo before we leave for vacation, "
 *   //        }
 * @endcode
 *
 * @param inner_seq Inner sequence to scan to create groups.
 * @param outer_key_sel Selector to get keys for elements in the outer sequence.
 * @param inner_key_sel Selector to get keys for elements in the inner sequence.
 * @param result_sel Result selector used to produce final results.
 * @param pred Predicate used to compare keys.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename InnerSeq,
         typename OuterKeySelector,
         typename InnerKeySelector,
         typename ResultSelector,
         typename Pred>
auto group_join(InnerSeq&& inner_seq,
                OuterKeySelector&& outer_key_sel,
                InnerKeySelector&& inner_key_sel,
                ResultSelector&& result_sel,
                Pred&& pred)
    -> detail::group_join_impl<InnerSeq,
                               OuterKeySelector,
                               InnerKeySelector,
                               ResultSelector,
                               Pred>
{
    return detail::group_join_impl<InnerSeq,
                                   OuterKeySelector,
                                   InnerKeySelector,
                                   ResultSelector,
                                   Pred>(std::forward<InnerSeq>(inner_seq),
                                         std::forward<OuterKeySelector>(outer_key_sel),
                                         std::forward<InnerKeySelector>(inner_key_sel),
                                         std::forward<ResultSelector>(result_sel),
                                         std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_intersect intersect
 * @brief Performs a set intersection of two sequences.
 *
 * The @c intersect operator returns all elements in the first sequence
 * that are also found in the second sequence (essentially a set intersection).
 * The elements are returned in the order that they appear in the first sequence.
 *
 * <b>.NET equivalent:</b> Intersect
 */

/**
 * @ingroup linq_op_intersect
 * @brief Performs set intersection of two sequences.
 *
 * Returns a sequence containing all elements from the first source sequence
 * that are also in the second source sequence (essentially a set intersection).
 * Elements are returned in the order that they appear in the first sequence.
 *
 * Elements are found using <tt>operator&lt;()</tt> to compare them.
 *
 * Use like this:
 *
 * @code
 *   const int ONE[] = { 42, 23, 66, 11, 7, 67 };
 *   const int TWO[] = { 10, 7, 60, 42, 43, 23 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(ONE)
 *            | intersect(TWO);
 *   // seq = { 42, 23, 7 }
 * @endcode
 *
 * @param seq2 Second source sequence.
 * @return (Once applied) Sequence containing elements from first
 *         source sequence that are also in @c seq2.
 */
template<typename Seq2>
auto intersect(Seq2&& seq2)
    -> detail::intersect_impl<Seq2, detail::less<>>
{
    return detail::intersect_impl<Seq2, detail::less<>>(std::forward<Seq2>(seq2),
                                                        detail::less<>());
}

/**
 * @ingroup linq_op_intersect
 * @brief Performs set intersection of two sequences using predicate.
 *
 * Returns a sequence containing all elements from the first source sequence
 * that are also in the second source sequence (essentially a set intersection).
 * Elements are returned in the order that they appear in the first sequence.
 *
 * Elements are found using the provided predicate to compare them. The predicate
 * must provide a strict ordering of the elements, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int ONE[] = { 42, 23, 66, 11, 7, 67 };
 *   const int TWO[] = { 10, 7, 60, 42, 43, 23 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(ONE)
 *            | intersect(TWO, [](int i, int j) { return i > j; });
 *   // seq = { 42, 23, 7 }
 * @endcode
 *
 * @param seq2 Second source sequence.
 * @param pred Predicate used to compare the elements.
 * @return (Once applied) Sequence containing elements from first
 *         source sequence that are also in @c seq2.
 */
template<typename Seq2, typename Pred>
auto intersect(Seq2&& seq2, Pred&& pred)
    -> detail::intersect_impl<Seq2, Pred>
{
    return detail::intersect_impl<Seq2, Pred>(std::forward<Seq2>(seq2),
                                              std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_join join
 * @brief Joins elements in two sequences according to their keys.
 *
 * The @c join operator scans two sequences: an <em>outer sequence</em> and an
 * <em>inner sequence</em>. For each element in the sequences, it extracts a key using
 * some <em>key selectors</em>. Then, it joins elements from the inner sequence to
 * elements in the outer sequence with matching keys. Finally, a <em>result selector</em>
 * is used to fold the elements into the final results. (This operator is similar to
 * a database JOIN.)
 *
 * <b>.NET equivalent:</b> Join
 */

/**
 * @ingroup linq_op_join
 * @brief Joins elements in two sequences according to their keys.
 *
 * Extracts keys for the elements of an <em>outer sequence</em> (the one on which
 * this operator is applied, e.g. the one passed to <tt>coveo::linq::from()</tt>)
 * and the provided <em>inner sequence</em> using the provided <em>key selectors</em>.
 * Next, joins elements from the inner sequence with  elements in the outer sequence
 * with matching keys. Finally, the provided <em>result selector</em> is used to convert
 * the elements into the final results. The result selector is called with two arguments:
 * an element from the outer sequence and an element from the inner sequence that share
 * the same key.
 *
 * In order to match the keys, they are compared using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   using person_data = std::pair<int, std::string>;
 *   using person_messages = std::pair<int, std::string>;
 *
 *   const std::vector<person_data> PERSONS = {
 *       { 1, "John Doe" },
 *       { 2, "Jane Smith" },
 *   };
 *   const std::vector<person_messages> MESSAGES = {
 *       { 1, "This is a test message" },
 *       { 2, "Hello Jane!" },
 *       { 1, "Doctors hate him!" },
 *       { 1, "Welcome to the company" },
 *       { 2, "Todo before we leave for vacation" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(PERSONS)
 *            | join(MESSAGES,
 *                   [](const person_data& pd) { return pd.first; },
 *                   [](const person_messages& pm) { return pm.first; }
 *                   [](const person_data& pd, const person_messages& pm) {
 *                       return pd.second + ": " + pm.second;
 *                   });
 *   // seq == {
 *   //            "John Doe: This is a test message",
 *   //            "John Doe: Doctors hate him!",
 *   //            "John Doe: Welcome to the company",
 *   //            "Jane Smith: Hello Jane!",
 *   //            "Jane Smith: Todo before we leave for vacation"
 *   //        }
 * @endcode
 *
 * @param inner_seq Inner sequence to scan.
 * @param outer_key_sel Selector to get keys for elements in the outer sequence.
 * @param inner_key_sel Selector to get keys for elements in the inner sequence.
 * @param result_sel Result selector used to produce final results.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename InnerSeq,
         typename OuterKeySelector,
         typename InnerKeySelector,
         typename ResultSelector>
auto join(InnerSeq&& inner_seq,
          OuterKeySelector&& outer_key_sel,
          InnerKeySelector&& inner_key_sel,
          ResultSelector&& result_sel)
    -> detail::join_impl<InnerSeq,
                         OuterKeySelector,
                         InnerKeySelector,
                         ResultSelector,
                         detail::less<>>
{
    return detail::join_impl<InnerSeq,
                             OuterKeySelector,
                             InnerKeySelector,
                             ResultSelector,
                             detail::less<>>(std::forward<InnerSeq>(inner_seq),
                                             std::forward<OuterKeySelector>(outer_key_sel),
                                             std::forward<InnerKeySelector>(inner_key_sel),
                                             std::forward<ResultSelector>(result_sel),
                                             detail::less<>());
}

/**
 * @ingroup linq_op_join
 * @brief Joins elements in two sequences according to their keys using predicate.
 *
 * Extracts keys for the elements of an <em>outer sequence</em> (the one on which
 * this operator is applied, e.g. the one passed to <tt>coveo::linq::from()</tt>)
 * and the provided <em>inner sequence</em> using the provided <em>key selectors</em>.
 * Next, joins elements from the inner sequence with  elements in the outer sequence
 * with matching keys. Finally, the provided <em>result selector</em> is used to convert
 * the elements into the final results. The result selector is called with two arguments:
 * an element from the outer sequence and an element from the inner sequence that share
 * the same key.
 *
 * In order to match the keys, they are compared using the provided predicate. The
 * predicate must provide a strict ordering of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   using person_data = std::pair<int, std::string>;
 *   using person_messages = std::pair<int, std::string>;
 *
 *   const std::vector<person_data> PERSONS = {
 *       { 1, "John Doe" },
 *       { 2, "Jane Smith" },
 *   };
 *   const std::vector<person_messages> MESSAGES = {
 *       { 1, "This is a test message" },
 *       { 2, "Hello Jane!" },
 *       { 1, "Doctors hate him!" },
 *       { 1, "Welcome to the company" },
 *       { 2, "Todo before we leave for vacation" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(PERSONS)
 *            | join(MESSAGES,
 *                   [](const person_data& pd) { return pd.first; },
 *                   [](const person_messages& pm) { return pm.first; }
 *                   [](const person_data& pd, const person_messages& pm) {
 *                       return pd.second + ": " + pm.second;
 *                   },
 *                   [](int i, int j) { return i > j; });
 *   // seq == {
 *   //            "John Doe: This is a test message",
 *   //            "John Doe: Doctors hate him!",
 *   //            "John Doe: Welcome to the company",
 *   //            "Jane Smith: Hello Jane!",
 *   //            "Jane Smith: Todo before we leave for vacation"
 *   //        }
 * @endcode
 *
 * @param inner_seq Inner sequence to scan.
 * @param outer_key_sel Selector to get keys for elements in the outer sequence.
 * @param inner_key_sel Selector to get keys for elements in the inner sequence.
 * @param result_sel Result selector used to produce final results.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of values returned by @c result_sel.
 */
template<typename InnerSeq,
         typename OuterKeySelector,
         typename InnerKeySelector,
         typename ResultSelector,
         typename Pred>
auto join(InnerSeq&& inner_seq,
          OuterKeySelector&& outer_key_sel,
          InnerKeySelector&& inner_key_sel,
          ResultSelector&& result_sel,
          Pred&& pred)
    -> detail::join_impl<InnerSeq,
                         OuterKeySelector,
                         InnerKeySelector,
                         ResultSelector,
                         Pred>
{
    return detail::join_impl<InnerSeq,
                             OuterKeySelector,
                             InnerKeySelector,
                             ResultSelector,
                             Pred>(std::forward<InnerSeq>(inner_seq),
                                   std::forward<OuterKeySelector>(outer_key_sel),
                                   std::forward<InnerKeySelector>(inner_key_sel),
                                   std::forward<ResultSelector>(result_sel),
                                   std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_last last
 * @brief Returns last element in a sequence.
 *
 * The @c last operator returns the last element in a sequence,
 * or the last element to satisfy a predicate. If the sequence does
 * not have such an element, an exception is thrown.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Last
 */

/**
 * @ingroup linq_op_last
 * @brief Returns last element in sequence.
 *
 * Returns the last element in a sequence. If the sequence
 * does not have elements, <tt>coveo::linq::empty_sequence</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto las1 = from(YES)
 *             | last();
 *   // las1 == 66
 *   // This throws an exception:
 *   // auto las2 = from(NAY)
 *   //           | last();
 * @endcode
 *
 * @return (Once applied) Last element in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 */
template<typename = void>
auto last()
    -> detail::last_impl_0<>
{
    return detail::last_impl_0<>();
}

/**
 * @ingroup linq_op_last
 * @brief Returns last element in sequence that satisfy predicate.
 *
 * Returns the last element in a sequence for which the given
 * predicate returns @c true. If the sequence does not have elements,
 * <tt>coveo::linq::empty_sequence</tt> is thrown; if the sequence does
 * not contain an element that satisfy the predicate,
 * <tt>coveo::linq::out_of_range</tt> is thrown.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto even = from(NUMS)
 *             | last([](int i) { return i % 2 == 0; });
 *   // even == 66
 *   // This throws an exception:
 *   // auto big = from(NUMS)
 *   //          | last([](int i) { return  i >= 90; });
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Last element in sequence for which @c pred returns @c true.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 * @exception coveo::linq::out_of_range The sequence has no element that satisfy @c pred.
 */
template<typename Pred>
auto last(const Pred& pred)
    -> detail::last_impl_1<Pred>
{
    return detail::last_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_last_or_def last_or_default
 * @brief Returns last element in a sequence, or a default value.
 *
 * The @c last_or_default operator returns the last element in a sequence,
 * or the last element to satisfy a predicate. If the sequence does
 * not have such an element, a default value is returned.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> LastOrDefault
 */

/**
 * @ingroup linq_op_last_or_def
 * @brief Returns last element in sequence, or default value.
 *
 * Returns the last element in a sequence. If the sequence
 * does not have elements, a <em>default-initialized</em>
 * value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto las1 = from(YES)
 *             | last();
 *   auto las2 = from(NAY)
 *             | last();
 *   // las1 == 66
 *   // las2 == 0
 * @endcode
 *
 * @return (Once applied) Last element in sequence, or a default value
 *         if sequence does not have elements.
 */
template<typename = void>
auto last_or_default()
    -> detail::last_or_default_impl_0<>
{
    return detail::last_or_default_impl_0<>();
}

/**
 * @ingroup linq_op_last_or_def
 * @brief Returns last element in sequence that satisfy predicate, or default value.
 *
 * Returns the last element in a sequence for which the given
 * predicate returns @c true. If the sequence does not have elements
 * or does not contain an element that satisfy the predicate, a
 * <em>default-initialized</em> value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto even = from(NUMS)
 *             | last([](int i) { return i % 2 == 0; });
 *   auto big = from(NUMS)
 *            | last([](int i) { return  i >= 90; });
 *   // even == 66
 *   // big == 0
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Last element in sequence for which @c pred returns @c true
 *         or, if no such element exists in sequence, a default value.
 */
template<typename Pred>
auto last_or_default(const Pred& pred)
    -> detail::last_or_default_impl_1<Pred>
{
    return detail::last_or_default_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_max max
 * @brief Returns maximum element in a sequence.
 *
 * The @c max operator returns the maximum element in a sequence. If the
 * sequence does not have elements, an exception is thrown.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Max
 */

/**
 * @ingroup linq_op_max
 * @brief Returns maximum element in sequence.
 *
 * Returns the maximum element in a sequence. If the sequence
 * does not have elements, <tt>coveo::linq::empty_sequence</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66, 11, 7 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto max1 = from(YES)
 *             | max();
 *   // max1 == 66
 *   // This throws an exception:
 *   // auto max2 = from(NAY)
 *   //           | max();
 * @endcode
 *
 * @return (Once applied) Maximum element in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 */
template<typename = void>
auto max()
    -> detail::max_impl_0<>
{
    return detail::max_impl_0<>();
}

/**
 * @ingroup linq_op_max
 * @brief Returns maximum projected value in sequence.
 *
 * Returns the maximum value in a sequence by projecting each element
 * to a different value using the provided <em>selector</em>. If the
 * sequence does not have elements, <tt>coveo::linq::empty_sequence</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66, 11, 7 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto max1 = from(YES)
 *             | max([](int i) { return -i; });
 *   // max1 == -7
 *   // This throws an exception:
 *   // auto max2 = from(NAY)
 *   //           | max([](int i) { return -i; });
 * @endcode
 *
 * @param sel Selector called to project each element into a different value.
 * @return (Once applied) Maximum projected value in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 */
template<typename Selector>
auto max(const Selector& sel)
    -> detail::max_impl_1<Selector>
{
    return detail::max_impl_1<Selector>(sel);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_min min
 * @brief Returns minimum element in a sequence.
 *
 * The @c min operator returns the minimum element in a sequence. If the
 * sequence does not have elements, an exception is thrown.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Min
 */

/**
 * @ingroup linq_op_min
 * @brief Returns minimum element in sequence.
 *
 * Returns the minimum element in a sequence. If the sequence
 * does not have elements, <tt>coveo::linq::empty_sequence</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66, 11, 7 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto min1 = from(YES)
 *             | min();
 *   // min1 == 7
 *   // This throws an exception:
 *   // auto min2 = from(NAY)
 *   //           | min();
 * @endcode
 *
 * @return (Once applied) Minimum element in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 */
template<typename = void>
auto min()
    -> detail::min_impl_0<>
{
    return detail::min_impl_0<>();
}

/**
 * @ingroup linq_op_min
 * @brief Returns minimum projected value in sequence.
 *
 * Returns the minimum value in a sequence by projecting each element
 * to a different value using the provided <em>selector</em>. If the
 * sequence does not have elements, <tt>coveo::linq::empty_sequence</tt>
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66, 11, 7 };
 *   const std::vector<int> NAY;
 *
 *   using namespace coveo::linq;
 *   auto min1 = from(YES)
 *             | min([](int i) { return -i; });
 *   // min1 == -66
 *   // This throws an exception:
 *   // auto min2 = from(NAY)
 *   //           | min([](int i) { return -i; });
 * @endcode
 *
 * @param sel Selector called to project each element into a different value.
 * @return (Once applied) Minimum projected value in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 */
template<typename Selector>
auto min(const Selector& sel)
    -> detail::min_impl_1<Selector>
{
    return detail::min_impl_1<Selector>(sel);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_none none
 * @brief Checks if no element in a sequence satisfy a predicate.
 *
 * The @c none operator scans a sequence and validates that none of its elements
 * satisfy a given @e predicate.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> <em>n/a</em>
 */

/**
 * @ingroup linq_op_none
 * @brief Checks elements in a sequence against a predicate.
 *
 * Scans a sequence and calls a @e predicate with each element. The predicate
 * must return @c true if the element satisfies the predicate. The final result
 * will indicate if no elements satisfy the predicate.
 *
 * Works on empty sequences (returns @c true in such a case).
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   bool no_big = from(NUMS)
 *               | none([](int i) { return i >= 90; });
 *   bool no_odd = from(NUMS)
 *               | none([](int i) { return i % 2 != 0; });
 *   // no_big == true
 *   // no_odd == false
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) @c true if no element in sequence satisfy @c pred.
 */
template<typename Pred>
auto none(const Pred& pred)
    -> detail::none_impl<Pred>
{
    return detail::none_impl<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_order_by order_by / order_by_descending / then_by / then_by_descending
 * @brief Orders elements in a sequence.
 *
 * The @c order_by operator (and its siblings) extract keys from each element in a sequence,
 * then orders those elements according to those keys. Depending on the operator used, the
 * ordering will be ascending or descending. If multiple operators are chained (using
 * <tt>coveo::linq::then_by()</tt>), elements will be ordered according to the first key
 * extracted and elements with equivalent keys will be further ordered by the other keys.
 *
 * <b>.NET equivalent:</b> OrderBy / OrderByDescending / ThenBy / ThenByDescending
 */

/**
 * @ingroup linq_op_order_by
 * @brief Orders elements in sequence by ascending key.
 *
 * For each element in the input sequence, extracts a key using the
 * provided <em>key selector</em>. Then, returns a sequence containing
 * the same elements, ordered according to the keys in ascending order.
 *
 * Keys are compared using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 10, 2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 14, 29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by([](const coord& c) { return c.x; });
 *   // seq == {
 *   //            { -1, 1,  12 },
 *   //            { 2,  20, 8  },
 *   //            { 3,  3,  3  },
 *   //            { 10, 2,  -4 },
 *   //            { 14, 29, 1  }
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @return (Once applied) Sequence of elements ordered by ascending key.
 */
template<typename KeySelector>
auto order_by(KeySelector&& key_sel)
    -> detail::order_by_impl<detail::order_by_comparator<KeySelector, detail::less<>, false>>
{
    typedef detail::order_by_comparator<KeySelector, detail::less<>, false> comparator;
    return detail::order_by_impl<comparator>(detail::make_unique<comparator>(std::forward<KeySelector>(key_sel), detail::less<>()));
}

/**
 * @ingroup linq_op_order_by
 * @brief Orders elements in sequence by ascending key using predicate.
 *
 * For each element in the input sequence, extracts a key using the
 * provided <em>key selector</em>. Then, returns a sequence containing
 * the same elements, ordered according to the keys in ascending order.
 *
 * Keys are compared using the provided predicate. The predicate must
 * provide a strict ordering of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 10, 2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 14, 29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by([](const coord& c) { return c.x; },
 *                       [](int i, int j) { return i < j; });
 *   // seq == {
 *   //            { -1, 1,  12 },
 *   //            { 2,  20, 8  },
 *   //            { 3,  3,  3  },
 *   //            { 10, 2,  -4 },
 *   //            { 14, 29, 1  }
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of elements ordered by ascending key.
 */
template<typename KeySelector, typename Pred>
auto order_by(KeySelector&& key_sel, Pred&& pred)
    -> detail::order_by_impl<detail::order_by_comparator<KeySelector, Pred, false>>
{
    typedef detail::order_by_comparator<KeySelector, Pred, false> comparator;
    return detail::order_by_impl<comparator>(detail::make_unique<comparator>(std::forward<KeySelector>(key_sel), std::forward<Pred>(pred)));
}

/**
 * @ingroup linq_op_order_by
 * @brief Orders elements in sequence by descending key.
 *
 * For each element in the input sequence, extracts a key using the
 * provided <em>key selector</em>. Then, returns a sequence containing
 * the same elements, ordered according to the keys in descending order.
 *
 * Keys are compared using <tt>operator&lt;()</tt>, but the result is reversed
 * to produce descending order.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 10, 2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 14, 29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by_descending([](const coord& c) { return c.x; });
 *   // seq == {
 *   //            { 14, 29, 1  }
 *   //            { 10, 2,  -4 },
 *   //            { 3,  3,  3  },
 *   //            { 2,  20, 8  },
 *   //            { -1, 1,  12 },
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @return (Once applied) Sequence of elements ordered by descending key.
 */
template<typename KeySelector>
auto order_by_descending(KeySelector&& key_sel)
    -> detail::order_by_impl<detail::order_by_comparator<KeySelector, detail::less<>, true>>
{
    typedef detail::order_by_comparator<KeySelector, detail::less<>, true> comparator;
    return detail::order_by_impl<comparator>(detail::make_unique<comparator>(std::forward<KeySelector>(key_sel), detail::less<>()));
}

/**
 * @ingroup linq_op_order_by
 * @brief Orders elements in sequence by descending key using predicate.
 *
 * For each element in the input sequence, extracts a key using the
 * provided <em>key selector</em>. Then, returns a sequence containing
 * the same elements, ordered according to the keys in descending order.
 *
 * Keys are compared using the provided predicate, but the result is reversed
 * to produce descending order. The predicate must provide a strict ordering
 * of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 10, 2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 14, 29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by_descending([](const coord& c) { return c.x; },
 *                                  [](int i, int j) { return i < j; });
 *   // seq == {
 *   //            { 14, 29, 1  }
 *   //            { 10, 2,  -4 },
 *   //            { 3,  3,  3  },
 *   //            { 2,  20, 8  },
 *   //            { -1, 1,  12 },
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of elements ordered by descending key.
 */
template<typename KeySelector, typename Pred>
auto order_by_descending(KeySelector&& key_sel, Pred&& pred)
    -> detail::order_by_impl<detail::order_by_comparator<KeySelector, Pred, true>>
{
    typedef detail::order_by_comparator<KeySelector, Pred, true> comparator;
    return detail::order_by_impl<comparator>(detail::make_unique<comparator>(std::forward<KeySelector>(key_sel), std::forward<Pred>(pred)));
}

/**
 * @ingroup linq_op_order_by
 * @brief Further orders elements in sequence by ascending key.
 *
 * Further orders a sequence previously ordered via <tt>coveo::linq::order_by()</tt>
 * or similar operator. Elements that were previously considered equivalent will
 * be further ordered by ascending order of keys as extracted by the provided
 * <em>key selector</em>.
 *
 * Keys are compared using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 2,  2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 3,  29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by([](const coord& c) { return c.x; })
 *            | then_by([](const coord& c) { return c.y; });
 *   // seq == {
 *   //            { -1, 1,  12 },
 *   //            { 2,  2,  -4 },
 *   //            { 2,  20, 8  },
 *   //            { 3,  3,  3  },
 *   //            { 3,  29, 1  }
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @return (Once applied) Sequence of elements further ordered by ascending key.
 */
template<typename KeySelector>
auto then_by(KeySelector&& key_sel)
    -> decltype(order_by(std::forward<KeySelector>(key_sel)))
{
    return order_by(std::forward<KeySelector>(key_sel));
}

/**
 * @ingroup linq_op_order_by
 * @brief Further orders elements in sequence by ascending key using predicate.
 *
 * Further orders a sequence previously ordered via <tt>coveo::linq::order_by()</tt>
 * or similar operator. Elements that were previously considered equivalent will
 * be further ordered by ascending order of keys as extracted by the provided
 * <em>key selector</em>.
 *
 * Keys are compared using the provided predicate. The predicate must
 * provide a strict ordering of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 2,  2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 3,  29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by([](const coord& c) { return c.x; },
 *                       [](int i, int j) { return i < j; })
 *            | then_by([](const coord& c) { return c.y; },
 *                      [](int i, int j) { return i < j; });
 *   // seq == {
 *   //            { -1, 1,  12 },
 *   //            { 2,  2,  -4 },
 *   //            { 2,  20, 8  },
 *   //            { 3,  3,  3  },
 *   //            { 3,  29, 1  }
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of elements further ordered by ascending key.
 */
template<typename KeySelector, typename Pred>
auto then_by(KeySelector&& key_sel, Pred&& pred)
    -> decltype(order_by(std::forward<KeySelector>(key_sel), std::forward<Pred>(pred)))
{
    return order_by(std::forward<KeySelector>(key_sel), std::forward<Pred>(pred));
}

/**
 * @ingroup linq_op_order_by
 * @brief Further orders elements in sequence by descending key.
 *
 * Further orders a sequence previously ordered via <tt>coveo::linq::order_by()</tt>
 * or similar operator. Elements that were previously considered equivalent will
 * be further ordered by descending order of keys as extracted by the provided
 * <em>key selector</em>.
 *
 * Keys are compared using <tt>operator&lt;()</tt>, but the result is reversed
 * to produce descending order.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 2,  2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 3,  29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by([](const coord& c) { return c.x; })
 *            | then_by_descending([](const coord& c) { return c.y; });
 *   // seq == {
 *   //            { -1, 1,  12 },
 *   //            { 2,  20, 8  },
 *   //            { 2,  2,  -4 },
 *   //            { 3,  29, 1  }
 *   //            { 3,  3,  3  },
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @return (Once applied) Sequence of elements further ordered by descending key.
 */
template<typename KeySelector>
auto then_by_descending(KeySelector&& key_sel)
    -> decltype(order_by_descending(std::forward<KeySelector>(key_sel)))
{
    return order_by_descending(std::forward<KeySelector>(key_sel));
}

/**
 * @ingroup linq_op_order_by
 * @brief Further orders elements in sequence by descending key using predicate.
 *
 * Further orders a sequence previously ordered via <tt>coveo::linq::order_by()</tt>
 * or similar operator. Elements that were previously considered equivalent will
 * be further ordered by descending order of keys as extracted by the provided
 * <em>key selector</em>.
 *
 * Keys are compared using the provided predicate, but the result is reversed
 * to produce descending order. The predicate must provide a strict ordering
 * of the keys, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   using coord = std::tuple<int, int, int>;
 *   const std::vector<coord> COORDS = {
 *       { 2,  2,  -4 },
 *       { 2,  20, 8  },
 *       { -1, 1,  12 },
 *       { 3,  29, 1  },
 *       { 3,  3,  3  },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(COORDS)
 *            | order_by([](const coord& c) { return c.x; },
 *                       [](int i, int j) { return i < j; })
 *            | then_by_descending([](const coord& c) { return c.y; },
 *                                 [](int i, int j) { return i < j; });
 *   // seq == {
 *   //            { -1, 1,  12 },
 *   //            { 2,  20, 8  },
 *   //            { 2,  2,  -4 },
 *   //            { 3,  29, 1  }
 *   //            { 3,  3,  3  },
 *   //        }
 * @endcode
 *
 * @param key_sel Key selector, used to extract a key for a sequence element.
 * @param pred Predicate used to compare the keys.
 * @return (Once applied) Sequence of elements further ordered by descending key.
 */
template<typename KeySelector, typename Pred>
auto then_by_descending(KeySelector&& key_sel, Pred&& pred)
    -> decltype(order_by_descending(std::forward<KeySelector>(key_sel), std::forward<Pred>(pred)))
{
    return order_by_descending(std::forward<KeySelector>(key_sel), std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_reverse reverse
 * @brief Reverses elements in a sequence.
 *
 * The @c reverse operator, as its name implies, scans a sequence a produces a new
 * sequence in which the elements are reversed.
 *
 * <b>.NET equivalent:</b> Reverse
 */

/**
 * @ingroup linq_op_reverse
 * @brief Reverses elements in sequence.
 *
 * Produces a new sequence in which elements in the source sequence
 * are in reverse order.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | reverse();
 *   // seq == { 7, 11, 66, 23, 42 }
 * @endcode
 *
 * @return (Once applied) Sequence of elements in reverse order.
 */
template<typename = void>
auto reverse()
    -> detail::reverse_impl<>
{
    return detail::reverse_impl<>();
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_select select / select_with_index / select_many / select_many_with_index
 * @brief Projects elements in a sequence into another form.
 *
 * The @c select operator scans a sequence and projects each of its element into another
 * form using a <em>selector</em>, returning a sequence of the results. A little similar
 * to <tt>std::transform()</tt>.
 *
 * <b>.NET equivalent:</b> Select / SelectMany
 */

/**
 * @ingroup linq_op_select
 * @brief Projects elements in sequence into another form.
 *
 * Produces a new sequence by projecting each element in the source
 * sequence into another form, a little like <tt>std::transform()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::string> STRS = {
 *       "Life, the Universe and Everything",
 *       "Hangar",
 *       "Route",
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(STRS)
 *            | select([](const std::string& s) { return s.size(); });
 *   // seq == { 33, 6, 5 }
 * @endcode
 *
 * @param sel Selector used to project each element in source sequence
 *            into another form.
 * @return (Once applied) Sequence of projected values.
 */
template<typename Selector>
auto select(Selector&& sel)
    -> detail::select_impl<detail::indexless_selector_proxy<Selector>>
{
    return detail::select_impl<detail::indexless_selector_proxy<Selector>>(
        detail::indexless_selector_proxy<Selector>(std::forward<Selector>(sel)));
}

/**
 * @ingroup linq_op_select
 * @brief Projects elements in sequence into another form using element index.
 *
 * Produces a new sequence by projecting each element in the source
 * sequence into another form, a little like <tt>std::transform()</tt>.
 * The selector receives, as second argument, the index of the element
 * in the source sequence.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::string> STRS = {
 *       "Life, the Universe and Everything",
 *       "Hangar",
 *       "Route",
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(STRS)
 *            | select_with_index([](const std::string& s, std::size_t i) { return s.size() * (i + 1); });
 *   // seq == { 33, 12, 15 }
 * @endcode
 *
 * @param sel Selector used to project each element in source sequence
 *            into another form. Receives index of element as second argument.
 * @return (Once applied) Sequence of projected values.
 */
template<typename Selector>
auto select_with_index(Selector&& sel)
    -> detail::select_impl<Selector>
{
    return detail::select_impl<Selector>(std::forward<Selector>(sel));
}

/**
 * @ingroup linq_op_select
 * @brief Projects elements in sequence into many values and flattens them.
 *
 * Produces a new sequence by projecting each element in the source
 * sequence into a sequence of new values, then flattens all those sequences.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::string> STRS = {
 *       "Life, the Universe and Everything",
 *       "Hangar",
 *       "Route",
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(STRS)
 *            | select_many([](const std::string& s) {
 *                              return std::vector<std::size_t>{
 *                                  s.size(),
 *                                  static_cast<std::size_t>(s.front() - 'A'),
 *                              };
 *                          });
 *   // seq == { 33, 11, 6, 7, 5, 17 }
 * @endcode
 *
 * @param sel Selector used to project each element in source sequence
 *            into a sequence of values.
 * @return (Once applied) Sequence of flattened projected values.
 */
template<typename Selector>
auto select_many(Selector&& sel)
    -> detail::select_many_impl<detail::indexless_selector_proxy<Selector>>
{
    return detail::select_many_impl<detail::indexless_selector_proxy<Selector>>(
        detail::indexless_selector_proxy<Selector>(std::forward<Selector>(sel)));
}

/**
 * @ingroup linq_op_select
 * @brief Projects elements in sequence into many values using element index and flattens them.
 *
 * Produces a new sequence by projecting each element in the source
 * sequence into a sequence of new values, then flattens all those sequences.
 * The selector receives, as second argument, the index of the element
 * in the source sequence.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::string> STRS = {
 *       "Life, the Universe and Everything",
 *       "Hangar",
 *       "Route",
 *   };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(STRS)
 *            | select_many_with_index([](const std::string& s, std::size_t i) {
 *                                         return std::vector<std::size_t>{
 *                                             s.size(),
 *                                             static_cast<std::size_t>(s.front() - 'A'),
 *                                             i + 1,
 *                                         };
 *                                     });
 *   // seq == { 33, 11, 1, 6, 7, 2, 5, 17, 3 }
 * @endcode
 *
 * @param sel Selector used to project each element in source sequence
 *            into a sequence of values. Receives index of element as
 *            second argument.
 * @return (Once applied) Sequence of flattened projected values.
 */
template<typename Selector>
auto select_many_with_index(Selector&& sel)
    -> detail::select_many_impl<Selector>
{
    return detail::select_many_impl<Selector>(std::forward<Selector>(sel));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_seq_equal sequence_equal
 * @brief Compares two sequences.
 *
 * The @c sequence_equal operator compares two sequences and validates
 * that they contain the same elements.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> SequenceEqual
 */

/**
 * @ingroup linq_op_seq_equal
 * @brief Compares elements in two sequences.
 *
 * Scans two sequences and compares each corresponding element using
 * <tt>operator==()</tt>. Returns @c true if the two sequences contain
 * the same elements in the same order.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS1 = { 42, 23, 66 };
 *   const std::vector<int> NUMS2 = { 42, 23, 66 };
 *   const std::vector<int> NUMS3 = { 67, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   bool one_and_two = from(NUMS1)
 *                    | sequence_equal(NUMS2);
 *   bool one_and_three = from(NUMS1)
 *                      | sequence_equal(NUMS3);
 *   // one_and_two   == true
 *   // one_and_three == false
 * @endcode
 *
 * @param seq2 Second sequence to compare. The first sequence is the one on
 *             which the operator will be applied (e.g., the sequence passed
 *             to <tt>coveo::linq::from()</tt>).
 * @return (Once applied) @c true both sequences contain the same elements
 *         in the same order.
 */
template<typename Seq2>
auto sequence_equal(const Seq2& seq2)
    -> detail::sequence_equal_impl_1<Seq2>
{
    return detail::sequence_equal_impl_1<Seq2>(seq2);
}

/**
 * @ingroup linq_op_seq_equal
 * @brief Compares elements in two sequences using predicate.
 *
 * Scans two sequences and compares each corresponding element using
 * the provided predicate. Returns @c true if the two sequences contain
 * the same elements in the same order.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS1 = { 42, 23, 66 };
 *   const std::vector<int> NUMS2 = { 41, 24, 67 };
 *   const std::vector<int> NUMS3 = { 30, 30, 90 };
 *
 *   auto fuzzy_equal = [](int i, int j) { return std::abs(i - j) <= 2; };
 *
 *   using namespace coveo::linq;
 *   bool one_and_two = from(NUMS1)
 *                    | sequence_equal(NUMS2, fuzzy_equal);
 *   bool one_and_three = from(NUMS1)
 *                      | sequence_equal(NUMS3, fuzzy_equal);
 *   // one_and_two   == true
 *   // one_and_three == false
 * @endcode
 *
 * @param seq2 Second sequence to compare. The first sequence is the one on
 *             which the operator will be applied (e.g., the sequence passed
 *             to <tt>coveo::linq::from()</tt>).
 * @param pred Predicate used to compare the elements.
 * @return (Once applied) @c true both sequences contain the same elements
 *         in the same order.
 */
template<typename Seq2, typename Pred>
auto sequence_equal(const Seq2& seq2, const Pred& pred)
    -> detail::sequence_equal_impl_2<Seq2, Pred>
{
    return detail::sequence_equal_impl_2<Seq2, Pred>(seq2, pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_single single
 * @brief Returns the only element in a sequence.
 *
 * The @c single operator returns the only element in a sequence,
 * or the only element to satisfy a predicate. If the sequence does
 * not have such an element or has more than one of them, an exception
 * is thrown.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Single
 */

/**
 * @ingroup linq_op_single
 * @brief Returns only element in sequence.
 *
 * Returns the only element in a sequence. If the sequence
 * does not have elements or has more than one element, an
 * exception is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42 };
 *   const std::vector<int> NAY1;
 *   const std::vector<int> NAY2 = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto val1 = from(YES)
 *             | single();
 *   // val1 == 42
 *   // This throws an exception:
 *   // auto val2 = from(NAY1)
 *   //           | single();
 *   // This also throws an exception:
 *   // auto val3 = from(NAY2)
 *   //           | single();
 * @endcode
 *
 * @return (Once applied) Only element in sequence.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 * @exception coveo::linq::out_of_range The sequence has more than one element.
 */
template<typename = void>
auto single()
    -> detail::single_impl_0<>
{
    return detail::single_impl_0<>();
}

/**
 * @ingroup linq_op_single
 * @brief Returns only element in sequence satisfying predicate.
 *
 * Returns the only element in a sequence that satisfy the given
 * predicate. If no element in the sequence or if more than one
 * element in the sequence satisfy the predicate, an exception
 * is thrown.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66 };
 *   const std::vector<int> NAY1 = { 67, 11, 7 };
 *   const std::vector<int> NAY2 = { 42, 24, 66 };
 *
 *   auto is_odd = [](int i) { return i % 2 != 0; };
 *
 *   using namespace coveo::linq;
 *   auto val1 = from(YES)
 *             | single(is_odd);
 *   // val1 == 23
 *   // This throws an exception:
 *   // auto val2 = from(NAY1)
 *   //           | single(is_odd);
 *   // This also throws an exception:
 *   // auto val3 = from(NAY2)
 *   //           | single(is_odd);
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Only element in sequence for which @c pred returned @c true.
 * @exception coveo::linq::empty_sequence The sequence does not have elements.
 * @exception coveo::linq::out_of_range No element satisfy @c pred, or more than one
 *                                      element satisfy @c pred.
 */
template<typename Pred>
auto single(const Pred& pred)
    -> detail::single_impl_1<Pred>
{
    return detail::single_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_single_or_def single_or_default
 * @brief Returns the only element in a sequence, or a default value.
 *
 * The @c single_or_default operator returns the only element in a sequence,
 * or the only element to satisfy a predicate. If the sequence does not have
 * such an element or has more than one of them, a default value is returned
 * instead.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> SingleOrDefault
 */

/**
 * @ingroup linq_op_single_or_def
 * @brief Returns only element in sequence, or default value.
 *
 * Returns the only element in a sequence. If the sequence
 * does not have elements or has more than one element, a
 * <em>default-initialized</em> value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42 };
 *   const std::vector<int> NAY1;
 *   const std::vector<int> NAY2 = { 42, 23, 66 };
 *
 *   using namespace coveo::linq;
 *   auto val1 = from(YES)
 *             | single_or_default();
 *   auto val2 = from(NAY1)
 *             | single_or_default();
 *   auto val3 = from(NAY2)
 *             | single_or_default();
 *   // val1 == 42
 *   // val2 == 0
 *   // val3 == 0
 * @endcode
 *
 * @return (Once applied) Only element in sequence, or default value
 *         if sequence does not have elements or if it has more than
 *         one element.
 */
template<typename = void>
auto single_or_default()
    -> detail::single_or_default_impl_0<>
{
    return detail::single_or_default_impl_0<>();
}

/**
 * @ingroup linq_op_single_or_def
 * @brief Returns only element in sequence satisfying predicate, or default value.
 *
 * Returns the only element in a sequence that satisfy the given
 * predicate. If no element in the sequence or if more than one
 * element in the sequence satisfy the predicate, a
 * <em>default-initialized</em> value is returned instead.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<int> YES = { 42, 23, 66 };
 *   const std::vector<int> NAY1 = { 67, 11, 7 };
 *   const std::vector<int> NAY2 = { 42, 24, 66 };
 *
 *   auto is_odd = [](int i) { return i % 2 != 0; };
 *
 *   using namespace coveo::linq;
 *   auto val1 = from(YES)
 *             | single_or_default(is_odd);
 *   auto val2 = from(NAY1)
 *             | single_or_default(is_odd);
 *   auto val3 = from(NAY2)
 *             | single_or_default(is_odd);
 *   // val1 == 23
 *   // val2 == 0
 *   // val3 == 0
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Only element in sequence for which @c pred returned @c true,
 *         or a default value if either no element satisfy @c pred or more than one
 *         element satisfy @c pred.
 */
template<typename Pred>
auto single_or_default(const Pred& pred)
    -> detail::single_or_default_impl_1<Pred>
{
    return detail::single_or_default_impl_1<Pred>(pred);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_skip skip / skip_while / skip_while_with_index
 * @brief Skips the first elements in a sequence.
 *
 * The @c skip operator (and its siblings) skip the first elements in a
 * sequence, either by skipping a certain number or using a predicate.
 *
 * <b>.NET equivalent:</b> Skip / SkipWhile
 */

/**
 * @ingroup linq_op_skip
 * @brief Skips the first N elements in sequence.
 *
 * Given a source sequence, returns a new sequence that skips
 * the first @c n elements of the source sequence. If the source
 * sequence contains less than @c n elements, returns an empty
 * sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | skip(3);
 *   // seq == { 11, 7 }
 * @endcode
 *
 * @param n Number of elements to skip.
 * @return (Once applied) New sequence skipping the first @c n
 *         elements of source sequence.
 */
template<typename = void>
auto skip(std::size_t n)
    -> detail::skip_impl<detail::skip_n_pred<>>
{
    return detail::skip_impl<detail::skip_n_pred<>>(detail::skip_n_pred<>(n), n);
}

/**
 * @ingroup linq_op_skip
 * @brief Skips the first elements in sequence satisfying predicate.
 *
 * Given a source sequence, returns a new sequence that skips
 * the first elements of the source sequence that satisfy the
 * provided predicate. If the source sequence contains only
 * elements that satisfy the predicate, returns an empty sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | skip_while([](int i) { return i < 60; });
 *   // seq == { 66, 11, 7 }
 * @endcode
 *
 * @param pred Predicate used to skip elements.
 * @return (Once applied) New sequence skipping the first
 *         elements of source sequence satisfying @c pred.
 */
template<typename Pred>
auto skip_while(Pred&& pred)
    -> detail::skip_impl<detail::indexless_selector_proxy<Pred>>
{
    return detail::skip_impl<detail::indexless_selector_proxy<Pred>>(detail::indexless_selector_proxy<Pred>(std::forward<Pred>(pred)),
                                                                     static_cast<std::size_t>(-1));
}

/**
 * @ingroup linq_op_skip
 * @brief Skips the first elements in sequence satisfying predicate using element index.
 *
 * Given a source sequence, returns a new sequence that skips
 * the first elements of the source sequence that satisfy the
 * provided predicate. If the source sequence contains only
 * elements that satisfy the predicate, returns an empty sequence.
 *
 * The predicate is called with two parameters every time: an
 * element from the source sequence, and its position in the sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | skip_while_with_index([](int i, std::size_t idx) { return i < 90 && idx < 3; });
 *   // seq == { 11, 7 }
 * @endcode
 *
 * @param pred Predicate used to skip elements.
 * @return (Once applied) New sequence skipping the first
 *         elements of source sequence satisfying @c pred.
 */
template<typename Pred>
auto skip_while_with_index(Pred&& pred)
    -> detail::skip_impl<Pred>
{
    return detail::skip_impl<Pred>(std::forward<Pred>(pred),
                                   static_cast<std::size_t>(-1));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_sum sum
 * @brief Computes sum of a sequence of values.
 *
 * The @c sum operator computes the sum of all values in a sequence. To achieve this,
 * it needs a <em>numerical function</em> to extract a numeric value from each sequence element.
 *
 * This is a @b terminal operator.
 *
 * <b>.NET equivalent:</b> Sum
 */

/**
 * @ingroup linq_op_sum
 * @brief Computes sum using numerical function.
 *
 * Computes the sum of all elements in a sequence by repeatedly calling a <em>numerical function</em>.
 * The function receives an element as parameter and must return a numerical value for the element.
 * The final result is the sum of all such numerical values.
 *
 * Does not work on empty sequences.
 *
 * Use like this:
 *
 * @code
 *   const std::vector<std::string> STRS = { "42", "23", "66" };
 *
 *   using namespace coveo::linq;
 *   auto tot = from(STRS)
 *            | sum([](auto&& s) { return std::stoi(s); });
 *   // tot == 131
 * @endcode
 *
 * @param num_f Function to get numerical value for each element.
 * @return (Once applied) Sum of all extracted numerical values.
 * @exception coveo::linq::empty_sequence The sequence contains no elements.
 */
template<typename F>
auto sum(const F& num_f)
    -> detail::sum_impl<F>
{
    return detail::sum_impl<F>(num_f);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_take take / take_while / take_while_with_index
 * @brief Keeps only the first elements in a sequence.
 *
 * The @c take operator (and its siblings) keep only the first elements in
 * a sequence, either by taking a certain number or using a predicate.
 *
 * <b>.NET equivalent:</b> Take / TakeWhile
 */

/**
 * @ingroup linq_op_take
 * @brief Keeps the first N elements in sequence.
 *
 * Given a source sequence, returns a new sequence that contains
 * only the first @c n elements of the source sequence. If the
 * source sequence contains less than @c n elements, returns as
 * many elements as possible.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | take(3);
 *   // seq == { 42, 23, 66 }
 * @endcode
 *
 * @param n Number of elements to take.
 * @return (Once applied) New sequence containing the first @c n
 *         elements of source sequence.
 */
template<typename = void>
auto take(std::size_t n)
    -> detail::take_impl<detail::skip_n_pred<>>
{
    return detail::take_impl<detail::skip_n_pred<>>(detail::skip_n_pred<>(n), n);
}

/**
 * @ingroup linq_op_take
 * @brief Keeps the first elements in sequence satisfying predicate.
 *
 * Given a source sequence, returns a new sequence that contains
 * only the first elements of the source sequence that satisfy the
 * provided predicate. If the source sequence contains no element
 * that satisfy the predicate, returns an empty sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | take_while([](int i) { return i > 20; });
 *   // seq == { 42, 23, 66 }
 * @endcode
 *
 * @param pred Predicate used to take elements.
 * @return (Once applied) New sequence containing the first
 *         elements of source sequence satisfying @c pred.
 */
template<typename Pred>
auto take_while(Pred&& pred)
    -> detail::take_impl<detail::indexless_selector_proxy<Pred>>
{
    return detail::take_impl<detail::indexless_selector_proxy<Pred>>(detail::indexless_selector_proxy<Pred>(std::forward<Pred>(pred)),
                                                                     static_cast<std::size_t>(-1));
}

/**
 * @ingroup linq_op_take
 * @brief Keeps the first elements in sequence satisfying predicate using element index.
 *
 * Given a source sequence, returns a new sequence that contains
 * only the first elements of the source sequence that satisfy the
 * provided predicate. If the source sequence contains no element
 * that satisfy the predicate, returns an empty sequence.
 *
 * The predicate is called with two parameters every time: an
 * element from the source sequence, and its position in the sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | take_while_with_index([](int i, std::size_t idx) { return i > 20 || idx < 4; });
 *   // seq == { 42, 23, 66, 11 }
 * @endcode
 *
 * @param pred Predicate used to take elements.
 * @return (Once applied) New sequence containing the first
 *         elements of source sequence satisfying @c pred.
 */
template<typename Pred>
auto take_while_with_index(Pred&& pred)
    -> detail::take_impl<Pred>
{
    return detail::take_impl<Pred>(std::forward<Pred>(pred),
                                   static_cast<std::size_t>(-1));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_to to / to_vector / to_associative / to_map
 * @brief Converts a sequence into another container type.
 *
 * The @c to operator (and its siblings) converts a sequence into
 * another type of container.
 *
 * This is a terminal operator if the resulting container is not
 * in itself a valid sequence.
 *
 * <b>.NET equivalent:</b> ToArray / ToDictionary / ToList / ToLookup
 */

/**
 * @ingroup linq_op_to
 * @brief Converts sequence into another container.
 *
 * Given a source sequence, returns a new container of the provided type
 * that contains the source sequence's elements.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto lst = from(NUMS)
 *            | to<std::list<int>>();
 *   // lst == std::list<int>{ 42, 23, 66, 11, 7 }
 * @endcode
 *
 * @tparam Container Type of container to convert to.
 * @return (Once applied) Object of type @c Container
 *         storing the same elements as source sequence.
 */
template<typename Container>
auto to()
    -> detail::to_impl<Container>
{
    return detail::to_impl<Container>();
}

/**
 * @ingroup linq_op_to
 * @brief Converts sequence into <tt>std::vector</tt>.
 *
 * Given a source sequence, returns a new <tt>std::vector</tt> that
 * contains the source sequence's elements. The vector's element type
 * is auto-detected from the source sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto vec = from(NUMS)
 *            | to_vector();
 *   // vec == std::vector<int>{ 42, 23, 66, 11, 7 }
 * @endcode
 *
 * @return (Once applied) <tt>std::vector</tt> storing the same
 *         elements as source sequence.
 */
template<typename = void>
auto to_vector()
    -> detail::to_vector_impl<>
{
    return detail::to_vector_impl<>();
}

/**
 * @ingroup linq_op_to
 * @brief Converts sequence into associative container using key selector.
 *
 * Given a source sequence, returns a new associative container of the
 * provided type that maps each element's key, as extracted by the provided
 * <em>key selector</em>, to the corresponding element.
 *
 * Use like this:
 *
 * @code
 *   using our_pair = std::pair<int, std::string>;
 *   using our_mm = std::multimap<int, our_pair>;
 *
 *   const std::vector<our_pair> = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 66, "Route" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto omp = from(NUMS)
 *            | to_associative<our_mm>([](our_pair p) { return p.first; });
 *   // omp == our_mm{
 *   //            23 => { 23, "Hangar" },
 *   //            42 => { 42, "Life" },
 *   //            66 => { 66, "Route" }
 *   //        }
 * @endcode
 *
 * @tparam Container Type of associative container to convert to.
 * @param key_sel Selector used to extract keys for sequence elements.
 * @return (Once applied) Object of type @c Container
 *         mapping the source sequence's elements as specified.
 */
template<typename Container,
         typename KeySelector>
auto to_associative(const KeySelector& key_sel)
    -> detail::to_associative_impl_1<Container, KeySelector>
{
    return detail::to_associative_impl_1<Container, KeySelector>(key_sel);
}

/**
 * @ingroup linq_op_to
 * @brief Converts sequence into associative container using key and element selector.
 *
 * Given a source sequence, returns a new associative container of the
 * provided type that maps each element's key, as extracted by the provided
 * <em>key selector</em>, to the corresponding mapped value, as extracted by
 * the provided <em>element selector</em>.
 *
 * Use like this:
 *
 * @code
 *   using our_pair = std::pair<int, std::string>;
 *   using our_mm = std::multimap<int, std::string>;
 *
 *   const std::vector<our_pair> = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 66, "Route" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto omp = from(NUMS)
 *            | to_associative<our_mm>([](our_pair p) { return p.first; },
 *                                     [](our_pair p) { return p.second; });
 *   // omp == our_mm{
 *   //            23 => "Hangar",
 *   //            42 => "Life",
 *   //            66 => "Route"
 *   //        }
 * @endcode
 *
 * @tparam Container Type of associative container to convert to.
 * @param key_sel Selector used to extract keys for sequence elements.
 * @param elem_sel Selector used to extract mapped values for sequence elements.
 * @return (Once applied) Object of type @c Container
 *         mapping the source sequence's elements as specified.
 */
template<typename Container,
         typename KeySelector,
         typename ElementSelector>
auto to_associative(const KeySelector& key_sel,
                    const ElementSelector& elem_sel)
    -> detail::to_associative_impl_2<Container, KeySelector, ElementSelector>
{
    return detail::to_associative_impl_2<Container, KeySelector, ElementSelector>(key_sel, elem_sel);
}

/**
 * @ingroup linq_op_to
 * @brief Converts sequence into <tt>std::map</tt> using key selector.
 *
 * Given a source sequence, returns a new <tt>std::map</tt> that maps
 * each element's key, as extracted by the provided <em>key selector</em>,
 * to the corresponding element. The map's type is auto-detected from
 * the source sequence and selector.
 *
 * Use like this:
 *
 * @code
 *   using our_pair = std::pair<int, std::string>;
 *
 *   const std::vector<our_pair> = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 66, "Route" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto omp = from(NUMS)
 *            | to_map([](our_pair p) { return p.first; });
 *   // omp == std::map<int, our_pair>{
 *   //            23 => { 23, "Hangar" },
 *   //            42 => { 42, "Life" },
 *   //            66 => { 66, "Route" }
 *   //        }
 * @endcode
 *
 * @param key_sel Selector used to extract keys for sequence elements.
 * @return (Once applied) <tt>std::map</tt> mapping the source sequence's
 *         elements as specified.
 */
template<typename KeySelector>
auto to_map(const KeySelector& key_sel)
    -> detail::to_map_impl_1<KeySelector>
{
    return detail::to_map_impl_1<KeySelector>(key_sel);
}

/**
 * @ingroup linq_op_to
 * @brief Converts sequence into <tt>std::map</tt> using key and element selector.
 *
 * Given a source sequence, returns a new <tt>std::map</tt> that maps
 * each element's key, as extracted by the provided <em>key selector</em>,
 * to the corresponding mapped value, as extracted by the provided
 * <em>element selector</em>. The map's type is auto-detected from
 * the source sequence and selectors.
 *
 * Use like this:
 *
 * @code
 *   using our_pair = std::pair<int, std::string>;
 *
 *   const std::vector<our_pair> = {
 *       { 42, "Life" },
 *       { 23, "Hangar" },
 *       { 66, "Route" },
 *   };
 *
 *   using namespace coveo::linq;
 *   auto omp = from(NUMS)
 *            | to_map([](our_pair p) { return p.first; },
 *                     [](our_pair p) { return p.second; });
 *   // omp == std::map<int, std::string>{
 *   //            23 => "Hangar",
 *   //            42 => "Life",
 *   //            66 => "Route"
 *   //        }
 * @endcode
 *
 * @param key_sel Selector used to extract keys for sequence elements.
 * @param elem_sel Selector used to extract mapped values for sequence elements.
 * @return (Once applied) <tt>std::map</tt> mapping the source sequence's
 *         elements as specified.
 */
template<typename KeySelector,
         typename ElementSelector>
auto to_map(const KeySelector& key_sel,
            const ElementSelector& elem_sel)
    -> detail::to_map_impl_2<KeySelector, ElementSelector>
{
    return detail::to_map_impl_2<KeySelector, ElementSelector>(key_sel, elem_sel);
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_union union_with
 * @brief Performs a set union of two sequences.
 *
 * The @c union_with operator returns the union of all elements in the first and
 * second sequence (essentially a set union). Similar to a concatenation, except
 * duplicate elements are filtered out. The elements are returned in the order
 * that they appear in the sequences.
 *
 * <b>.NET equivalent:</b> Union
 */

/**
 * @ingroup linq_op_union
 * @brief Performs set union of two sequences.
 *
 * Returns a sequence containing all elements from the two source sequences
 * (essentially a set union). Duplicate elements are filtered out. Elements
 * are returned in the order that they appear in the sequences.
 *
 * Elements are compared using <tt>operator&lt;()</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int ONE[] = { 42, 23, 66, 11, 7, 67 };
 *   const int TWO[] = { 10, 7, 60, 42, 43, 23 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(ONE)
 *            | union_with(TWO);
 *   // seq = { 42, 23, 66, 11, 7, 67, 10, 60, 43 }
 * @endcode
 *
 * @param seq2 Second source sequence.
 * @return (Once applied) Sequence containing all elements from
 *         first source sequence and from @c seq2.
 */
template<typename Seq2>
auto union_with(Seq2&& seq2)
    -> detail::union_impl<Seq2, detail::less<>>
{
    return detail::union_impl<Seq2, detail::less<>>(std::forward<Seq2>(seq2),
                                                    detail::less<>());
}

/**
 * @ingroup linq_op_union
 * @brief Performs set union of two sequences using predicate.
 *
 * Returns a sequence containing all elements from the two source sequences
 * (essentially a set union). Duplicate elements are filtered out. Elements
 * are returned in the order that they appear in the sequences.
 *
 * Elements are compared using the provided predicate. The predicate must
 * provide a strict ordering of the elements, like <tt>std::less</tt>.
 *
 * Use like this:
 *
 * @code
 *   const int ONE[] = { 42, 23, 66, 11, 7, 67 };
 *   const int TWO[] = { 10, 7, 60, 42, 43, 23 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(ONE)
 *            | union_with(TWO, [](int i, int j) { return i > j; });
 *   // seq = { 42, 23, 66, 11, 7, 67, 10, 60, 43 }
 * @endcode
 *
 * @param seq2 Second source sequence.
 * @param pred Predicate used to compare sequence elements.
 * @return (Once applied) Sequence containing all elements from
 *         first source sequence and from @c seq2.
 */
template<typename Seq2, typename Pred>
auto union_with(Seq2&& seq2, Pred&& pred)
    -> detail::union_impl<Seq2, Pred>
{
    return detail::union_impl<Seq2, Pred>(std::forward<Seq2>(seq2),
                                          std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_where where / where_with_index
 * @brief Filters a sequence.
 *
 * The @c where operator (and its siblings) filter the elements in a sequence,
 * returning a new sequence containing only the elements that satisfy a predicate.
 *
 * <b>.NET equivalent:</b> Where
 */

/**
 * @ingroup linq_op_where
 * @brief Filters sequence elements using predicate.
 *
 * Returns a sequence that contains only the elements from the
 * source sequence that satisfy the given predicate. Order of
 * the elements is preserved.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | where([](int i) { return i % 2 != 0; });
 *   // seq = { 23, 11, 7 }
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Sequence containing all elements from
 *         source sequence for which @c pred returned @c true.
 */
template<typename Pred>
auto where(Pred&& pred)
    -> detail::where_impl<detail::indexless_selector_proxy<Pred>>
{
    return detail::where_impl<detail::indexless_selector_proxy<Pred>>(
        detail::indexless_selector_proxy<Pred>(std::forward<Pred>(pred)));
}

/**
 * @ingroup linq_op_where
 * @brief Filters sequence elements using predicate and element index.
 *
 * Returns a sequence that contains only the elements from the
 * source sequence that satisfy the given predicate. Order of
 * the elements is preserved.
 *
 * The predicate receives two arguments: a sequence element and its
 * position in the sequence.
 *
 * Use like this:
 *
 * @code
 *   const int NUMS[] = { 42, 23, 66, 11, 7 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(NUMS)
 *            | where_with_index([](int i, std::size_t idx) { return i % 2 != 0 || idx == 0; });
 *   // seq = { 42, 23, 11, 7 }
 * @endcode
 *
 * @param pred Predicate to satisfy.
 * @return (Once applied) Sequence containing all elements from
 *         source sequence for which @c pred returned @c true.
 */
template<typename Pred>
auto where_with_index(Pred&& pred)
    -> detail::where_impl<Pred>
{
    return detail::where_impl<Pred>(std::forward<Pred>(pred));
}

/**
 * @ingroup linq_operators_list
 * @defgroup linq_op_zip zip
 * @brief Zips two sequences by combining corresponding elements.
 *
 * The @c zip operator scans two source sequences and and passes elements
 * with the same index to a <em>result selector</em>, then produces a
 * sequence containing the results returned by that selector.
 *
 * <b>.NET equivalent:</b> Zip
 */

/**
 * @ingroup linq_op_zip
 * @brief Zips two sequences.
 *
 * Scans two source sequences and calls the provided
 * <em>result selector</em> with elements of the two
 * sequences that have the same index. Then, produces
 * a sequence of the results returned by that selector.
 * The resulting sequence will be as long as the shortest
 * of the two input sequences.
 *
 * Use like this:
 *
 * @code
 *   const int ONE[] = { 42, 23, 66, 11, 7 };
 *   const int TWO[] = { 8, 21, 90, 4 };
 *
 *   using namespace coveo::linq;
 *   auto seq = from(ONE)
 *            | zip(TWO, [](int i, int j) { return i + j; });
 *   // seq = { 50, 44, 156, 15 }
 * @endcode
 *
 * @param seq2 Second sequence to scan. The first sequence is the
 *             one to which the operator is applied, e.g. the
 *             one passed to <tt>coveo::linq::from()</tt>.
 * @param result_sel Selector used to "zip" two corresponding
 *                   sequence elements.
 * @return (Once applied) Sequence containing zipped elements
 *         from the two sequences.
 */
template<typename Seq2,
         typename ResultSelector>
auto zip(Seq2&& seq2, ResultSelector&& result_sel)
    -> detail::zip_impl<Seq2, ResultSelector>
{
    return detail::zip_impl<Seq2, ResultSelector>(std::forward<Seq2>(seq2),
                                                  std::forward<ResultSelector>(result_sel));
}

} // linq
} // coveo

#endif // COVEO_LINQ_H
