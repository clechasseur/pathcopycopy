// Copyright (c) 2016, Charles Lechasseur
// Distributed under the MIT license (see LICENSE).

// Implementation of C++17's std::optional

#ifndef CL_OPTIONAL_H
#define CL_OPTIONAL_H

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>


// Compiler detection
#if defined(_MSC_VER)
#   if _MSC_VER >= 1800 // Visual Studio 2013
#       define CL_OPTIONAL_VARIADIC
#       define CL_OPTIONAL_EXPLICIT_OP_BOOL
#       define CL_OPTIONAL_TEMPLATE_FUNCS_DEFAULT_PARAMS
#   endif
#   if _MSC_VER >= 1900 // Visual Studio 2015
//#       define CL_OPTIONAL_NOEXCEPT
#       define CL_OPTIONAL_NORETURN
#       define CL_OPTIONAL_MEMBERS_REF_QUALIFIERS
#   endif
#   if _MSC_VER >= 1910 // Visual Studio 2017
#       define CL_OPTIONAL_CONSTEXPR
#   endif
#elif defined(__clang__)
#   if __has_feature(cxx_noexcept)
#       define CL_OPTIONAL_NOEXCEPT
#   endif
#   if __has_feature(cxx_constexpr) && __cplusplus >= 201402L
#       define CL_OPTIONAL_CONSTEXPR
#   endif
#   if __has_feature(cxx_attributes)
#       define CL_OPTIONAL_NORETURN
#   endif
#   if __has_feature(cxx_variadic_templates)
#       define CL_OPTIONAL_VARIADIC
#   endif
#   if __has_feature(cxx_reference_qualified_functions) && __cplusplus >= 201402L
#       define CL_OPTIONAL_MEMBERS_REF_QUALIFIERS
#   endif
#   if __has_feature(cxx_explicit_conversions)
#       define CL_OPTIONAL_EXPLICIT_OP_BOOL
#   endif
#   if __has_feature(cxx_default_function_template_args)
#       define CL_OPTIONAL_TEMPLATE_FUNCS_DEFAULT_PARAMS
#   endif
#elif defined(__GNUG__)
#   if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#       define CL_OPTIONAL_NOEXCEPT
#       define CL_OPTIONAL_NORETURN
#       define CL_OPTIONAL_VARIADIC
#       define CL_OPTIONAL_MEMBERS_REF_QUALIFIERS
#       define CL_OPTIONAL_EXPLICIT_OP_BOOL
#       define CL_OPTIONAL_TEMPLATE_FUNCS_DEFAULT_PARAMS
#   endif
#   if __GNUC__ >= 5
#       define CL_OPTIONAL_CONSTEXPR
#   endif
#endif


// Define CL_OPTIONAL_NOEXCEPT to get noexcept specifications
#ifdef CL_OPTIONAL_NOEXCEPT
#   define _CL_OPT_NOEXCEPT         noexcept
#   define _CL_OPT_NOEXCEPTEX(...)  noexcept(__VA_ARGS__)
#else
#   define _CL_OPT_NOEXCEPT
#   define _CL_OPT_NOEXCEPTEX(...)
#endif

// Define CL_OPTIONAL_CONSTEXPR to get constexpr specifications
#ifdef CL_OPTIONAL_CONSTEXPR
#   define _CL_OPT_CONSTEXPR            constexpr
#   define _CL_OPT_CONSTEXPR_OR_CONST   constexpr
#else
#   define _CL_OPT_CONSTEXPR
#   define _CL_OPT_CONSTEXPR_OR_CONST   const
#endif

// Define CL_OPTIONAL_VARIADIC to get variadic templates
#ifdef CL_OPTIONAL_VARIADIC
#   define _CL_OPT_VARIADIC 1
#else
#   define _CL_OPT_VARIADIC 0
#endif

// Define CL_OPTIONAL_NORETURN to get noreturn specifications
#ifdef CL_OPTIONAL_NORETURN
#   define _CL_OPT_NORETURN [[noreturn]]
#else
#   define _CL_OPT_NORETURN
#endif

// Define CL_OPTIONAL_MEMBERS_REF_QUALIFIERS to get lvalue- and rvalue-qualified members
#ifdef CL_OPTIONAL_MEMBERS_REF_QUALIFIERS
#   define _CL_OPT_MEMBERS_REF_QUALIFIERS   1
#   define _CL_OPT_LVALUE_REF_QUALIF        &
#else
#   define _CL_OPT_MEMBERS_REF_QUALIFIERS   0
#   define _CL_OPT_LVALUE_REF_QUALIF
#endif

// Define CL_OPTIONAL_EXPLICIT_OP_BOOL to get explicit operator bool()
#ifdef CL_OPTIONAL_EXPLICIT_OP_BOOL
#   define _CL_OPT_OP_EXPLICIT  explicit
#else
#   define _CL_OPT_OP_EXPLICIT
#endif

// Define CL_OPTIONAL_TEMPLATE_FUNCS_DEFAULT_PARAMS to get template methods
// with default template params, like assignment operator from value
#ifdef CL_OPTIONAL_TEMPLATE_FUNCS_DEFAULT_PARAMS
#   define _CL_OPT_TEMPL_FUNC_DEF_PARAMS    1
#else
#   define _CL_OPT_TEMPL_FUNC_DEF_PARAMS    0
#endif


#if _CL_OPT_VARIADIC
#   include <initializer_list>
#endif

namespace cl {

// Type indicating an optional object with no value. See:
// http://en.cppreference.com/w/cpp/utility/optional/nullopt_t
struct nullopt_t {
    _CL_OPT_CONSTEXPR nullopt_t(int) { }
};

// Instance of nullopt_t passed to optional's members. See:
// http://en.cppreference.com/w/cpp/utility/optional/nullopt
_CL_OPT_CONSTEXPR_OR_CONST nullopt_t nullopt = 0;

#if _CL_OPT_VARIADIC
// Empty type used for in_place functions. See:
// http://en.cppreference.com/w/cpp/utility/in_place_tag
struct in_place_tag {
    in_place_tag() = delete;
private:
    in_place_tag(int) { }
    friend in_place_tag in_place();
};

// Function to identify an optional method with in-place construction. See:
// http://en.cppreference.com/w/cpp/utility/in_place
inline in_place_tag in_place() { return in_place_tag(0); }
using in_place_t = in_place_tag (&)();
#endif // _CL_OPT_VARIADIC

// Exception thrown by optional when trying to access
// an optional with no value. See:
// http://en.cppreference.com/w/cpp/utility/optional/bad_optional_access
class bad_optional_access : public std::logic_error
{
public:
    bad_optional_access(const std::string& what_arg)
        : std::logic_error(what_arg) { }
    bad_optional_access(const char* what_arg)
        : std::logic_error(what_arg) { }
};

// Wrapper for an optional value. Part of future C++17 API. See:
// http://en.cppreference.com/w/cpp/utility/optional
template<typename T>
class optional
{
private:
    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type stg_;   // We store any value here.
    T* const pval_;                                                                     // Pointer to value stored in stg_.
    bool has_value_;                                                                    // Whether we have a value in stg_.

    // Exception helpers
    _CL_OPT_NORETURN void throw_bad_optional_access() const {
        throw bad_optional_access("bad_optional_access");
    }

    // Helper for swap when one has a value and the other does not
    static void swap_helper(optional& from, optional& to) _CL_OPT_NOEXCEPTEX(std::is_nothrow_move_constructible<T>::value &&
                                                                             std::is_nothrow_destructible<T>::value)
    {
        new (to.pval_) T(std::move(*from.pval_));
        from.pval_->~T();
        to.has_value_ = true;
        from.has_value_ = false;
    }

public:
    typedef T   value_type;

    // Constructors of optional without a value
    _CL_OPT_CONSTEXPR optional() _CL_OPT_NOEXCEPT
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(false) { }
    _CL_OPT_CONSTEXPR optional(nullopt_t) _CL_OPT_NOEXCEPT
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(false) { }

    // Copy constructor
    optional(const optional& obj) _CL_OPT_NOEXCEPTEX(std::is_nothrow_copy_constructible<T>::value)
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(obj.has_value_)
    {
        if (has_value_) {
            new (pval_) T(*obj.pval_);
        }
    }

    // Move constructor (note: does not steal has_value_, see API)
    optional(optional&& obj) _CL_OPT_NOEXCEPTEX(std::is_nothrow_move_constructible<T>::value)
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(obj.has_value_)
    {
        if (has_value_) {
            new (pval_) T(std::move(*obj.pval_));
        }
    }

    // Constructors of optional with a value
    optional(const T& val) _CL_OPT_NOEXCEPTEX(std::is_nothrow_copy_constructible<T>::value)
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(true)
    {
        new (pval_) T(val);
    }
    optional(T&& val) _CL_OPT_NOEXCEPTEX(std::is_nothrow_move_constructible<T>::value)
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(true)
    {
        new (pval_) T(std::move(val));
    }

#if _CL_OPT_VARIADIC
    // In-place constructor of optional with a value
    template<typename... Args>
    _CL_OPT_CONSTEXPR explicit optional(in_place_t, Args&&... args)
                _CL_OPT_NOEXCEPTEX(std::is_nothrow_constructible<T, Args&&...>::value)
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(true)
    {
        new (pval_) T(std::forward<Args>(args)...);
    }

    // Same but with an initializer_list thrown in just for fun
    template<typename U, typename... Args,
             typename = typename std::enable_if<std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value, void>::type>
    _CL_OPT_CONSTEXPR explicit optional(in_place_t,
                                         std::initializer_list<U> ilist,
                                         Args&&... args)
                _CL_OPT_NOEXCEPTEX(std::is_nothrow_constructible<T, std::initializer_list<U>&, Args&&...>::value)
        : stg_(), pval_(reinterpret_cast<T*>(&stg_)), has_value_(true)
    {
        new (pval_) T(ilist, std::forward<Args>(args)...);
    }
#endif // _CL_OPT_VARIADIC

    // Destructor
    ~optional() _CL_OPT_NOEXCEPTEX(std::is_nothrow_destructible<T>::value) {
        if (has_value_) {
            // #clp TODO we should not do this for trivially-destructible types as per API
            pval_->~T();
        }
    }

    // Assignment operator that clears any held value
    optional& operator=(nullopt_t) _CL_OPT_NOEXCEPTEX(std::is_nothrow_destructible<T>::value)
    {
        if (has_value_) {
            pval_->~T();
            has_value_ = false;
        }
        return *this;
    }

    // Copy assignment operator
    optional& operator=(const optional& obj) _CL_OPT_NOEXCEPTEX(std::is_nothrow_copy_constructible<T>::value &&
                                                                std::is_nothrow_copy_assignable<T>::value &&
                                                                std::is_nothrow_destructible<T>::value)
    {
        if (obj.has_value_) {
            if (has_value_) {
                // Can assign
                *pval_ = *obj.pval_;
            } else {
                // Need to construct
                new (pval_) T(*obj.pval_);
                has_value_ = true;
            }
        } else if (has_value_) {
            pval_->~T();
            has_value_ = false;
        }
        return *this;
    }

    // Move assignment (note: does not steal has_value_, see API)
    optional& operator=(optional&& obj) _CL_OPT_NOEXCEPTEX(std::is_nothrow_move_constructible<T>::value &&
                                                           std::is_nothrow_move_assignable<T>::value &&
                                                           std::is_nothrow_destructible<T>::value)
    {
        if (obj.has_value_) {
            if (has_value_) {
                // Can assign
                *pval_ = std::move(*obj.pval_);
            } else {
                // Need to construct
                new (pval_) T(std::move(*obj.pval_));
                has_value_ = true;
            }
        } else if (has_value_) {
            pval_->~T();
            has_value_ = false;
        }
        return *this;
    }

#if _CL_OPT_TEMPL_FUNC_DEF_PARAMS
    // Assignment from a value
    template<typename U,
             typename = typename std::enable_if<std::is_same<typename std::decay<U>::type, T>::value, void>::type>
    optional& operator=(U&& val)
            _CL_OPT_NOEXCEPTEX(std::is_nothrow_constructible<T, U>::value &&
                               std::is_nothrow_assignable<T, U>::value)
    {
        if (has_value_) {
            // Can assign
            *pval_ = std::forward<U>(val);
        } else {
            // Need to construct
            new (pval_) T(std::forward<U>(val));
            has_value_ = true;
        }
        return *this;
    }
#endif // _CL_OPT_TEMPL_FUNC_DEF_PARAMS

    // Unchecked operators that access the value
    _CL_OPT_CONSTEXPR const T* operator->() const {
        return pval_;
    }
    _CL_OPT_CONSTEXPR T* operator->() {
        return pval_;
    }
    _CL_OPT_CONSTEXPR const T& operator*() const _CL_OPT_LVALUE_REF_QUALIF {
        return *pval_;
    }
    _CL_OPT_CONSTEXPR T& operator*() _CL_OPT_LVALUE_REF_QUALIF {
        return *pval_;
    }
#if _CL_OPT_MEMBERS_REF_QUALIFIERS
    _CL_OPT_CONSTEXPR const T&& operator*() const && {
        return std::move(*pval_);
    }
    _CL_OPT_CONSTEXPR T&& operator*() && {
        return std::move(*pval_);
    }
#endif // _CL_OPT_MEMBERS_REF_QUALIFIERS

    // Value detection
    _CL_OPT_CONSTEXPR _CL_OPT_OP_EXPLICIT operator bool() const _CL_OPT_NOEXCEPT {
        return has_value_;
    }
    _CL_OPT_CONSTEXPR bool has_value() const _CL_OPT_NOEXCEPT {
        return has_value_;
    }

    // Value accessors that throw if optional doesn't have a value
    _CL_OPT_CONSTEXPR const T& value() const _CL_OPT_LVALUE_REF_QUALIF {
        if (!has_value_) {
            throw_bad_optional_access();
        }
        return *pval_;
    }
    _CL_OPT_CONSTEXPR T& value() _CL_OPT_LVALUE_REF_QUALIF {
        if (!has_value_) {
            throw_bad_optional_access();
        }
        return *pval_;
    }
#if _CL_OPT_MEMBERS_REF_QUALIFIERS
    _CL_OPT_CONSTEXPR const T&& value() const && {
        if (!has_value_) {
            throw_bad_optional_access();
        }
        return std::move(*pval_);
    }
    _CL_OPT_CONSTEXPR T&& value() && {
        if (!has_value_) {
            throw_bad_optional_access();
        }
        return std::move(*pval_);
    }
#endif // _CL_OPT_MEMBERS_REF_QUALIFIERS

    // Value accessors with an alternative if optional doesn't have a value
    template<typename U>
    _CL_OPT_CONSTEXPR T value_or(U&& default_val) const _CL_OPT_LVALUE_REF_QUALIF
                _CL_OPT_NOEXCEPTEX(std::is_nothrow_copy_constructible<T>::value &&
                                   std::is_nothrow_constructible<T, U>::value)
    {
        return has_value_ ? *pval_ : T(std::forward<U>(default_val));
    }
#if _CL_OPT_MEMBERS_REF_QUALIFIERS
    template<typename U>
    _CL_OPT_CONSTEXPR T value_or(U&& default_val) &&
                _CL_OPT_NOEXCEPTEX(std::is_nothrow_move_constructible<T>::value &&
                                   std::is_nothrow_constructible<T, U>::value)
    {
        return has_value_ ? std::move(*pval_) : T(std::forward<U>(default_val));
    }
#endif // _CL_OPT_MEMBERS_REF_QUALIFIERS

    // Swap implementation
    void swap(optional& obj) _CL_OPT_NOEXCEPTEX(std::is_nothrow_move_constructible<T>::value &&
                                                std::is_nothrow_destructible<T>::value &&
                                                noexcept(std::swap(std::declval<T&>(), std::declval<T&>())))  // #clp TODO using std::swap here seems wrong, no?
    {
        if (obj.has_value_) {
            if (has_value_) {
                using std::swap;
                swap(*pval_, *obj.pval_);
            } else {
                swap_helper(obj, *this);
            }
        } else if (has_value_) {
            swap_helper(*this, obj);
        }
    }
    // Non-member version of swap for ADL
    friend void swap(optional& obj1, optional& obj2) {
        obj1.swap(obj2);
    }

    // Removes any value held
    void reset() _CL_OPT_NOEXCEPTEX(std::is_nothrow_destructible<T>::value)
    {
        if (has_value_) {
            pval_->~T();
            has_value_ = false;
        }
    }

#if _CL_OPT_VARIADIC
    // Destroys any held value and constructs a new one in-place
    template<typename... Args>
    void emplace(Args&&... args) _CL_OPT_NOEXCEPTEX(std::is_nothrow_destructible<T>::value &&
                                                    std::is_nothrow_constructible<T, Args...>::value)
    {
        reset();
        new (pval_) T(std::forward<Args>(args)...);
        has_value_ = true;
    }
    template<typename U, typename... Args,
             typename = typename std::enable_if<std::is_constructible<T, std::initializer_list<U>&, Args&&...>::value, void>::type>
    void emplace(std::initializer_list<U> ilist,
                 Args&&... args) _CL_OPT_NOEXCEPTEX(std::is_nothrow_destructible<T>::value &&
                                                    std::is_nothrow_constructible<T, std::initializer_list<U>&, Args&&...>::value)
    {
        reset();
        new (pval_) T(ilist, std::forward<Args>(args)...);
        has_value_ = true;
    }
#endif // _CL_OPT_VARIADIC

    // Compares two optionals. One without a value is less than one with a value.
    friend bool operator==(const optional& lhs, const optional& rhs) {
        return lhs.has_value_ == rhs.has_value_ &&
               (!lhs.has_value_ || *lhs.pval_ == *rhs.pval_);
    }
    friend bool operator!=(const optional& lhs, const optional& rhs) {
        return !(lhs == rhs);
    }
    friend bool operator<(const optional& lhs, const optional& rhs) {
        if (lhs.has_value_) {
            if (rhs.has_value_) {
                return *lhs.pval_ < *rhs.pval_;
            } else {
                return false;
            }
        } else {
            return rhs.has_value_;
        }
    }
    friend bool operator<=(const optional& lhs, const optional& rhs) {
        return !(rhs < lhs);
    }
    friend bool operator>(const optional& lhs, const optional& rhs) {
        return rhs < lhs;
    }
    friend bool operator>=(const optional& lhs, const optional& rhs) {
        return !(lhs < rhs);
    }

    // Compares optional with a nullopt (like comparing with one without a value).
    friend bool operator==(const optional& lhs, nullopt_t) _CL_OPT_NOEXCEPT {
        return !lhs.has_value_;
    }
    friend bool operator==(nullopt_t, const optional& rhs) _CL_OPT_NOEXCEPT {
        return !rhs.has_value_;
    }
    friend bool operator!=(const optional& lhs, nullopt_t) _CL_OPT_NOEXCEPT {
        return lhs.has_value_;
    }
    friend bool operator!=(nullopt_t, const optional& rhs) _CL_OPT_NOEXCEPT {
        return rhs.has_value_;
    }
    friend bool operator<(const optional&, nullopt_t) _CL_OPT_NOEXCEPT {
        return false;
    }
    friend bool operator<(nullopt_t, const optional& rhs) _CL_OPT_NOEXCEPT {
        return rhs.has_value_;
    }
    friend bool operator<=(const optional& lhs, nullopt_t) _CL_OPT_NOEXCEPT {
        return !lhs.has_value_;
    }
    friend bool operator<=(nullopt_t, const optional&) _CL_OPT_NOEXCEPT {
        return true;
    }
    friend bool operator>(const optional& lhs, nullopt_t) _CL_OPT_NOEXCEPT {
        return lhs.has_value_;
    }
    friend bool operator>(nullopt_t, const optional&) _CL_OPT_NOEXCEPT {
        return false;
    }
    friend bool operator>=(const optional&, nullopt_t) _CL_OPT_NOEXCEPT {
        return true;
    }
    friend bool operator>=(nullopt_t, const optional& rhs) _CL_OPT_NOEXCEPT {
        return !rhs.has_value_;
    }

    // Compares optional with a value (like comparing with one with a value).
    friend bool operator==(const optional& lhs, const T& rhs) {
        return lhs.has_value_ ? *lhs.pval_ == rhs : false;
    }
    friend bool operator==(const T& lhs, const optional& rhs) {
        return rhs.has_value_ ? lhs == *rhs.pval_ : false;
    }
    friend bool operator!=(const optional& lhs, const T& rhs) {
        return lhs.has_value_ ? *lhs.pval_ != rhs : true;
    }
    friend bool operator!=(const T& lhs, const optional& rhs) {
        return rhs.has_value_ ? lhs != *rhs.pval_ : true;
    }
    friend bool operator<(const optional& lhs, const T& rhs) {
        return lhs.has_value_ ? *lhs.pval_ < rhs : true;
    }
    friend bool operator<(const T& lhs, const optional& rhs) {
        return rhs.has_value_ ? lhs < *rhs.pval_ : false;
    }
    friend bool operator<=(const optional& lhs, const T& rhs) {
        return lhs.has_value_ ? *lhs.pval_ <= rhs : true;
    }
    friend bool operator<=(const T& lhs, const optional& rhs) {
        return rhs.has_value_ ? lhs <= *rhs.pval_ : false;
    }
    friend bool operator>(const optional& lhs, const T& rhs) {
        return lhs.has_value_ ? *lhs.pval_ > rhs : false;
    }
    friend bool operator>(const T& lhs, const optional& rhs) {
        return rhs.has_value_ ? lhs > *rhs.pval_ : true;
    }
    friend bool operator>=(const optional& lhs, const T& rhs) {
        return lhs.has_value_ ? *lhs.pval_ >= rhs : false;
    }
    friend bool operator>=(const T& lhs, const optional& rhs) {
        return rhs.has_value_ ? lhs >= *rhs.pval_ : true;
    }
};

// Creates an optional by inferring type. See:
// http://en.cppreference.com/w/cpp/utility/optional/make_optional
template<typename T>
_CL_OPT_CONSTEXPR auto make_optional(T&& val)
    -> optional<typename std::decay<T>::type>
{
    return optional<typename std::decay<T>::type>(std::forward<T>(val));
}

#if _CL_OPT_VARIADIC
// Creates an optional from arguments. See:
// http://en.cppreference.com/w/cpp/utility/optional/make_optional
template<typename T, typename... Args>
_CL_OPT_CONSTEXPR auto make_optional(Args&&... args)
    -> optional<T>
{
    return optional<T>(in_place, std::forward<Args>(args)...);
}
template<typename T, typename U, typename... Args>
_CL_OPT_CONSTEXPR auto make_optional(std::initializer_list<U> ilist, Args&&... args)
    -> optional<T>
{
    return optional<T>(in_place, ilist, std::forward<Args>(args)...);
}
#endif // _CL_OPT_VARIADIC

} // cl

namespace std {

// Specialize hash for optional
template<typename T>
struct hash<cl::optional<T>> {
    size_t operator()(const cl::optional<T>& obj) const {
        return obj.has_value() ? hash<T>()(*obj) : 0;
    }
};

} // std

#endif // CL_OPTIONAL_H
