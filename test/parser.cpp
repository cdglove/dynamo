// ****************************************************************************
// evalulater/test/parser.cpp
//
// Test parser functionality
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#include "evalulater/parser/parser.hpp"
#include "evalulater/error_handler.hpp"

#define BOOST_TEST_MODULE Parser
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( unary_parse )
{
	evalulater::diagnostic_sink diagnostic(std::cout);
	evalulater::parser::parser parser(diagnostic);
	BOOST_CHECK(parser.parse("1;"));
	BOOST_CHECK(parser.parse("-1;"));
	BOOST_CHECK(parser.parse("+1;"));
	BOOST_CHECK(parser.parse("v;"));
	BOOST_CHECK(parser.parse("-v;"));
	BOOST_CHECK(parser.parse("+v;"));
}

BOOST_AUTO_TEST_CASE( binary_parse )
{
	evalulater::diagnostic_sink diagnostic(std::cout);
	evalulater::parser::parser parser(diagnostic);
	BOOST_CHECK(parser.parse("1 + 1;"));
	BOOST_CHECK(parser.parse("1-1;"));
	BOOST_CHECK(parser.parse("1 * 1;"));
	BOOST_CHECK(parser.parse("1/1;"));
	BOOST_CHECK(parser.parse("a = 10;"));
	BOOST_CHECK(parser.parse("1/1*(2*2);"));
	BOOST_CHECK(parser.parse("(1/1)*(2*2);"));
	BOOST_CHECK(parser.parse("(1/1)*2*2;"));
	BOOST_CHECK(parser.parse("((1/1)*(2*2));"));
	BOOST_CHECK(parser.parse("a = 10;"));
	BOOST_CHECK(parser.parse("a = b;"));
	BOOST_CHECK(parser.parse("foo = boo;"));
}

BOOST_AUTO_TEST_CASE( failed_parse )
{
	evalulater::diagnostic_sink diagnostic(std::cout);
	evalulater::parser::parser parser(diagnostic);
	BOOST_CHECK(!parser.parse("1 + 1"));
	BOOST_CHECK(!parser.parse("1 1;"));
	BOOST_CHECK(!parser.parse("1 */ 1;"));
	BOOST_CHECK(!parser.parse("(1/1(;"));
	BOOST_CHECK(!parser.parse("1foo = 2boo;"));
}
