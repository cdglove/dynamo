// ****************************************************************************
// dynamo/test/compiler.cpp
//
// Test compiler functionality
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#include "dynamo/vm/byte_code.hpp"
#include "dynamo/parse/parser.hpp"
#include "dynamo/compiler.hpp"
#include "dynamo/diagnostic/sink.hpp"

#define BOOST_TEST_MODULE Compiler
#include <boost/test/unit_test.hpp>

enum Index
{
	NO_INDEX,
	USE_INDEX,
};

static void test_expression(std::string expression, bool expect_should_compile, Index use_idx)
{
	dynamo::diagnostic_sink sink(std::cout);	

	dynamo::parse::string_parser parser(sink);
	boost::optional<dynamo::ast::statement_list> ast;
	ast = parser.parse(expression);

	BOOST_CHECK(ast);
	if(ast)
	{
		dynamo::compiler compiler(sink);
		boost::optional<dynamo::vm::byte_code> code;

		if(use_idx)
		{
			code = compiler.compile(*ast, parser.get_indexed_source());
		}
		else
		{
			code = compiler.compile(*ast);
		}

		bool compiled = false;
		if(code)
		{
			compiled = true;
		}
		
		BOOST_CHECK_EQUAL(compiled, expect_should_compile);
	}
}

static void all_tests(Index use_idx)
{
	test_expression("div(mul(1,1),2);", true, use_idx);
	test_expression("mul(1, 2, 3);", false, use_idx);
}

BOOST_AUTO_TEST_CASE( intrinsics )
{
	all_tests(USE_INDEX);
	all_tests(NO_INDEX);
}
