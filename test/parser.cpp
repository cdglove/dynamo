// ****************************************************************************
// dynamo/test/parser.cpp
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
#include "dynamo/parse/parser.hpp"
#include "dynamo/error_handler.hpp"

#define BOOST_TEST_MODULE Parser
#include <boost/test/unit_test.hpp>

typedef std::string::const_iterator iterator_type;

BOOST_AUTO_TEST_CASE( unary_parse )
{
	dynamo::diagnostic_sink diagnostic(std::cout);
	dynamo::parse::parser parser(diagnostic);
	BOOST_CHECK(parser.parse("1;"));
	BOOST_CHECK(parser.parse("-1;"));
	BOOST_CHECK(parser.parse("+1;"));
	BOOST_CHECK(parser.parse("v;"));
	BOOST_CHECK(parser.parse("-v;"));
	BOOST_CHECK(parser.parse("+v;"));
	BOOST_CHECK(parser.parse(";"));
}

BOOST_AUTO_TEST_CASE( binary_parse )
{
	dynamo::diagnostic_sink diagnostic(std::cout);
	dynamo::parse::parser parser(diagnostic);
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

BOOST_AUTO_TEST_CASE( intrinsic_parse )
{
	dynamo::diagnostic_sink diagnostic(std::cout);
	dynamo::parse::parser parser(diagnostic);
	BOOST_CHECK(parser.parse("abs(1);"));
	BOOST_CHECK(parser.parse("pow(1,2);"));
	BOOST_CHECK(parser.parse("add(1,2);"));
	BOOST_CHECK(parser.parse("sub(1,2);"));
	BOOST_CHECK(parser.parse("mul(1,2);"));
	BOOST_CHECK(parser.parse("div(1,2);"));
	BOOST_CHECK(parser.parse("not(0);"));
	BOOST_CHECK(parser.parse("div(mul(2,2),not(0));"));
}

BOOST_AUTO_TEST_CASE( failed_parse )
{
	dynamo::diagnostic_sink diagnostic(std::cout);
	dynamo::parse::parser parser(diagnostic);
	BOOST_CHECK(!parser.parse("1 + 1"));
	BOOST_CHECK(!parser.parse("1 1;"));
	BOOST_CHECK(!parser.parse("1 */ 1;"));
	BOOST_CHECK(!parser.parse("(1/1(;"));
	BOOST_CHECK(!parser.parse("1foo = 2boo;"));
}
