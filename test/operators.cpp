// ****************************************************************************
// dynamo/test/operators.cpp
//
// Test operator functionality
// 
// Copyright Chris Glover 2015
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// ****************************************************************************
#include "dynamo/vm/machine.hpp"
#include "dynamo/vm/byte_code.hpp"
#include "dynamo/parser/parser.hpp"
#include "dynamo/compiler.hpp"
#include "dynamo/linker.hpp"

#define BOOST_TEST_MODULE Operators
#include <boost/test/unit_test.hpp>

static void test_expression(std::string expression, float expected_result)
{
	dynamo::error_handler<						 
		std::string::const_iterator
	> error_handler(std::cout);	

	dynamo::parse::parser parser(error_handler);
	boost::optional<dynamo::ast::statement_list> ast;
	dynamo::compiler compiler(error_handler);
	boost::optional<dynamo::vm::byte_code> code;
	dynamo::linker linker(error_handler);
	boost::optional<dynamo::vm::executable> exe;
	dynamo::vm::machine vm;

	ast = parser.parse(expression);
	BOOST_CHECK(ast);
	if(ast)
	{
		code = compiler.compile(*ast);
		BOOST_CHECK(code);
		if(code)
		{
			exe = linker.link(*code);
			BOOST_CHECK(exe);
			if(exe)
			{
				vm.execute(*exe);
				BOOST_CHECK_EQUAL(vm.top(), expected_result);
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( unary_operators )
{
	test_expression("1;", 1);
	test_expression("-1;", -1);
	test_expression("!1;", 0);
	test_expression("!0;", 1);
	test_expression("-(1);", -1);
}

BOOST_AUTO_TEST_CASE( binary_operators )
{
	test_expression("1*1;", 1);
	test_expression("2*2;", 4);
	test_expression("4/2;", 2);
	test_expression("1+1;", 2);
	test_expression("4-2;", 2);
}

BOOST_AUTO_TEST_CASE( intrinsics )
{
	test_expression("mul(2,2);", 4);
	test_expression("div(4,2);", 2);
	test_expression("add(1,1);", 2);
	test_expression("sub(4,2);", 2);
	test_expression("pow(4,2);", 16);
	test_expression("abs(-19);", 19);
	test_expression("not(19);",  0);
	test_expression("not(0);",   1);
	test_expression("div(mul(1,1),2);", 0.5f);
}
