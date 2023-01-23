/*
 *             Copyright Andrey Semashev 2023.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   scope_final.cpp
 * \author Andrey Semashev
 *
 * \brief  This file contains tests for \c scope_final.
 */

#include <boost/scope/scope_final.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/config.hpp>
#include <utility>
#include <stdexcept>
#include "function_types.hpp"

int g_n = 0;

void check_normal()
{
    int n = 0;
    {
        boost::scope::scope_final< normal_func > guard{ normal_func(n) };
    }
    BOOST_TEST_EQ(n, 1);

    n = 0;
    {
        normal_func func(n);
        boost::scope::scope_final< normal_func& > guard(func);
    }
    BOOST_TEST_EQ(n, 1);

    struct local
    {
        static void raw_func()
        {
            ++g_n;
        }
    };

    g_n = 0;
    {
        boost::scope::scope_final< void (&)() > guard(local::raw_func);
    }
    BOOST_TEST_EQ(g_n, 1);
}

void check_throw()
{
    int n = 0;
    try
    {
        boost::scope::scope_final< normal_func > guard{ normal_func(n) };
        throw std::runtime_error("error");
    }
    catch (...) {}
    BOOST_TEST_EQ(n, 1);

    n = 0;
    try
    {
        boost::scope::scope_final< throw_on_copy_func > guard{ throw_on_copy_func(n) };
        BOOST_ERROR("An exception is expected to be thrown by throw_on_copy_func");
    }
    catch (...) {}
    BOOST_TEST_EQ(n, 1);

    n = 0;
    try
    {
        boost::scope::scope_final< throw_on_move_func > guard{ throw_on_move_func(n) };
    }
    catch (...)
    {
        BOOST_ERROR("An exception is not expected to be thrown by throw_on_move_func (copy ctor should be used)");
    }
    BOOST_TEST_EQ(n, 1);

    n = 0;
    bool scope_ended = false, exception_thrown = false;
    try
    {
        boost::scope::scope_final< throw_on_call_func > guard{ throw_on_call_func(n) };
        scope_ended = true;
    }
    catch (...)
    {
        exception_thrown = true;
    }
    BOOST_TEST_EQ(n, 1);
    BOOST_TEST(scope_ended);
    BOOST_TEST(exception_thrown);
}

void check_deduction()
{
#if !defined(BOOST_NO_CXX17_DEDUCTION_GUIDES)
    int n = 0;
    {
        boost::scope::scope_final guard{ normal_func(n) };
        BOOST_TEST_TRAIT_SAME(decltype(guard), boost::scope::scope_final< normal_func >);
    }
    BOOST_TEST_EQ(n, 1);

    n = 0;
    {
        boost::scope::scope_final guard([&n] { ++n; });
    }
    BOOST_TEST_EQ(n, 1);

    n = 0;
    {
        BOOST_SCOPE_FINAL [&n] { ++n; };
        BOOST_SCOPE_FINAL [&n] { ++n; };
    }
    BOOST_TEST_EQ(n, 2);
#endif // !defined(BOOST_NO_CXX17_DEDUCTION_GUIDES)
}

int main()
{
    check_normal();
    check_throw();
    check_deduction();

    return boost::report_errors();
}