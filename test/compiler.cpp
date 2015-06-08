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
#include "dynamo/parser/parser.hpp"
#include "dynamo/compiler.hpp"

#define BOOST_TEST_MODULE Compiler
#include <boost/test/unit_test.hpp>

static void test_expression(std::string expression, bool expect_should_compile)
{
	dynamo::error_handler<						 
		std::string::const_iterator
	> error_handler(std::cout);	

	dynamo::parse::parser parser(error_handler);
	boost::optional<dynamo::ast::statement_list> ast;
	dynamo::compiler compiler(error_handler);
	boost::optional<dynamo::vm::byte_code> code;
	ast = parser.parse(expression);
	BOOST_CHECK(ast);
	if(ast)
	{
		code = compiler.compile(*ast);
		bool compiled = false;
		if(code)
		{
			compiled = true;
		}
		
		BOOST_CHECK_EQUAL(compiled, expect_should_compile);
	}
}


BOOST_AUTO_TEST_CASE( intrinsics )
{
	test_expression("div(mul(1,1),2);", true);
	test_expression("mul(1, 2, 3);", false);
}
