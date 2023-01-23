/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2023 Andrey Semashev
 */
/*!
 * \file scope/detail/is_not_like.hpp
 *
 * This header contains definition of \c is_not_like type trait.
 */

#ifndef BOOST_SCOPE_DETAIL_IS_NOT_LIKE_HPP_INCLUDED_
#define BOOST_SCOPE_DETAIL_IS_NOT_LIKE_HPP_INCLUDED_

#include <type_traits>
#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace boost {
namespace scope {
namespace detail {

template< typename T, template< typename... > class Template >
struct is_not_like : public std::true_type { };
template< typename T, template< typename... > class Template >
struct is_not_like< T&, Template > : public is_not_like< T, Template > { };
template< template< typename... > class Template, typename... Ts >
struct is_not_like< Template< Ts... >, Template > : public std::false_type { };
template< template< typename... > class Template, typename... Ts >
struct is_not_like< const Template< Ts... >, Template > : public std::false_type { };
template< template< typename... > class Template, typename... Ts >
struct is_not_like< volatile Template< Ts... >, Template > : public std::false_type { };
template< template< typename... > class Template, typename... Ts >
struct is_not_like< const volatile Template< Ts... >, Template > : public std::false_type { };

} // namespace detail
} // namespace scope
} // namespace boost

#endif // BOOST_SCOPE_DETAIL_IS_NOT_LIKE_HPP_INCLUDED_